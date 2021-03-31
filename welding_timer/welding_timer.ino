/*
 * Welding Timer 焊接機 V0.1
 * Release Date: 2021-03-30
 * By 羽山秋人 (https://3wa.tw)
 * GitHub: https://github.com/shadowjohn/mywelding_timer
 * 
 * 接腳參考
   #define CLK D0   (TM1637 4 Digital Display)
   #define DIO D1   (TM1637 4 Digital Display)
   #define RELAY D2 (接焊接繼電器)
   #define BTN D3   (焊接按鈕)
   #define A0       (可變電阻)
*/
//get VCC Voltage
// From : https://www.reddit.com/r/esp8266/comments/9ubypi/how_do_i_use_the_espgetvcc_correctly/
//ADC_MODE(ADC_VCC)  
//int batterylevel;  

//數位模組 Grove_4Digital_Display
#include <TM1637.h>

#define CLK D0 //pins definitions for TM1637 and can be changed to other ports
#define DIO D1 //給 4 Digit-Display TM1637 使用
#define RELAY D2 //繼電器接角
#define BTN D3 //焊接按鈕
long steps = 0;
long steps_btn = 0;
int analogPin = A0;
int weldingTime = 50;
float R1 = 1000; //R1 是參考電阻 (1KΩ)
float R2 = 0; //記錄 可變電阻值，如 0Ω~10000Ω (0~10K)

TM1637 tm1637(CLK,DIO); //定義顯示器變數
//int sleep_step = 50; //最少，最多1000(ms) 50 100 150 200 ...
bool isUpClick = true; //是否曾放開按鈕
bool isBooting = true;
bool isWelding = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);   
  pinMode(RELAY,OUTPUT);  
  pinMode(BTN,INPUT); 
  digitalWrite(RELAY,LOW); //一開始不work
  // 4 Digit-Display
  tm1637.init();  
  tm1637.set(2); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;，數值越大，越亮
  //回歸零
  tm1637.display(0,-1); //-1 for blank
  tm1637.display(1,-1); //-1 for blank
  tm1637.display(2,-1); //-1 for blank
  tm1637.display(3,0);
}
void updateR2(){
  //取得目前可變電阻姿態，將電阻值存到 R2
  //batterylevel = ESP.getVcc();  
  //本來想用ESP.getVcc 來取得電壓，但一寫就當，幹
  int raw = analogRead(analogPin);
  float Vin = 5.0;//batterylevel /1000.0; //5.0;
  if(raw) 
  {
    float buffer= raw * Vin;
    //Serial.print("Vin:");
    //Serial.println(Vin);
    //Serial.print("raw:");
    //Serial.println(raw);  
    
    float Vout = (buffer)/1350.0; //1350 是常數嗎@_@?
    buffer= (Vin/Vout) -1;
    R2 = R1 * buffer;
    //Serial.print("Vout: ");
    //Serial.println(Vout);
    //Serial.print("R2: ");
    //Serial.println(R2);
    //delay(200);
    R2 = R2 - (int(R2) % 50); //55->50  132->100
  }
  
}
void diaplayOnLed(int show_rpm,bool fill_zero=false)
{
  //將轉速，變成顯示值
  //只顯示 萬千百十
  //如果要顯示 千百十個，就不用除了
  //太多數位有點眼花
  weldingTime = show_rpm;
  if(!isBooting)
  {    
    int lowV=2200;
    int maxV=7400;
    
    show_rpm = (show_rpm < lowV)?lowV:show_rpm;
    show_rpm = (show_rpm > maxV)?maxV:show_rpm;
    weldingTime = map(show_rpm, lowV, maxV, 50, 1000); 
    if( weldingTime != 50 )  
    {
      weldingTime = weldingTime -(weldingTime%100);
    }
  }
  weldingTime = (weldingTime<=50)?50:weldingTime;
  
  String rpm_str = String(weldingTime); //  /10
  if(rpm_str.length()<=3)
  {
    if(fill_zero==true)
    {
      rpm_str = lpad(rpm_str,4,"0"); // 變成如 "0000"
    }
    else
    {
      rpm_str = lpad(rpm_str,4,"X"); // 變成如 "XXX0"
    }
    
  }
  Serial.print("\nAfter lpad:");
  Serial.println(rpm_str);
  for(int i=0;i<4;i++)
  { 
      if(rpm_str[i]=='X')
      {
        tm1637.display(i,-1); //-1 代表 blank 一片黑    
      }
      else
      {
        // Serial.println(rpm_str[i]);
        // 腦包直接轉回 String 再把單字轉 int
        // From : https://www.arduino.cc/en/Tutorial.StringToIntExample
        tm1637.display(i,String(rpm_str[i]).toInt());    
      }
  }
}
String lpad(String temp , byte L , String theword){
  //用來補LED左邊的空白
  byte mylen = temp.length();
  if(mylen > (L - 1))return temp.substring(0,L-1);
  for (byte i=0; i< (L-mylen); i++) 
    temp = theword + temp;
  return temp;
}
void runTest(){
  //開機後，0000~9999 一次
  for(int i=0;i<=9999;i+=1111)
  {
    diaplayOnLed(i,true);
    delay(300);  
  }
  delay(300);    
  diaplayOnLed(0,false);
}

void loop() {
  /*batterylevel = ESP.getVcc();  
  if(batterylevel <= 2170){  
    Serial.print("\nBatterylevel: ");
    Serial.println(batterylevel);
    ESP.deepSleep(0);  
  }  
  Serial.print("\nBatterylevel:");
  Serial.println(batterylevel);
  */
  if(isBooting)
  {
    //開機自動run 0000~9999
    runTest();
    isBooting=false;
  }
  
  //updateR2();
  steps++;
  steps%=40000;
  if(steps==0)
  {
    //當step 為整數0，才detect 一次電阻顯示到 LED 就好
    updateR2();
    diaplayOnLed(int(R2),false); //顯示到 LED
  } 
  //偵測按鈕是否按下
  //Serial.print("\ndigitalRead(BTN):");
  //Serial.println(digitalRead(BTN));  
  //Serial.print("\nmillis()-steps_btn:");  
  //計數到 25000 才能執行一次焊接
  //25000是在作防按鈕彈跳
  steps_btn++;
  steps_btn = (steps_btn>=25000)?25000:steps_btn;
  
  //Serial.print("\nsteps_btn:");
  //Serial.println(steps_btn);
  //25000 是用來去除彈跳的寫法
  if( digitalRead(BTN) == HIGH && isUpClick == true && steps_btn >= 25000 )
  {
    //按下按鈕後，開始焊接
    isUpClick = false;
    isWelding = true; 
    digitalWrite(RELAY,HIGH); //焊接開始
    delay(weldingTime); 
    digitalWrite(RELAY,LOW);
    //Serial.println("Done Welding...");  
    //Serial.println("Done Welding...");  
    //Serial.println("Done Welding...");  
    isWelding = false;    
    steps_btn = 0;
  }
  if(digitalRead(BTN)==LOW)
  {
    //放開按鈕，計數器收置
    isUpClick = true;   
    steps_btn = 0;   
  }  
}
