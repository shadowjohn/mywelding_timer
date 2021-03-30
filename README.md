# mywelding_timer
<h2>功能：</h2>
　　微波爐變壓器改裝可調時間點焊機
<br>
<br>
作者：羽山秋人 (https://3wa.tw)<br>
<br>
最初開發日期：2021-03-27 12:05<br>
最後更新日期：2021-03-30 14:10<br>
版本：V0.1<br>
<br>
<h2>說明：</h2>
　　羽山在 Youtube 上看到好多老外都用「微波爐變壓器」把細的線圈拆掉後，換上粗電線，繞個二圈，就可以作成 AC 110V to AC 1~2V 點焊機，拿來焊 18650 或其他鋰電池、鋰鐵電池、不銹鋼杯手柄，相當好用。<br>
　　前些日子買了一台中古的變壓器，試作成功，接下來要再作一個簡單的電路來精準控制焊接的時間，讓鎳片跟電池的焊接時間最佳化。
<br>
<br>
<h2>所需材料：</h2>
<ul>
  <li>微波爐變壓 800w x 1</li>
  <li>AC 110V TO DC 5V 變壓器(ESP8266供電用)</li>
  <li>20A 搖頭開關 x 1</li>  
  <li>Arduino ESP8266 x 1</li>
  <li>TM1637 4 Digital Display 四位元七段顯示器 x 1</li>
  <li>可變電阻 10KΩ x 1</li>
  <li>電阻 1KΩ 1/2w x 1</li>
  <li>電阻 330Ω 1/2w x 1</li>
  <li>電容 10㎌ 50V x 2</li>
  <li>LED 綠、紅 各一顆</li>
  <li>蜂嗚器 5V x 1</li>
  <li>Relay 5V x 1</li>
  <li>洞洞板 x 1</li>
  <li>微動開關 x 1</li>
</ul>
<br>
<br>
<h2>接腳參考：</h2>
<ul>
  <li>D0：TM1637 CLK</li>
  <li>D1：TM1637 DIO</li>
  <li>D2：RELAY 5V</li>
  <li>D3：微動開關 Input</li>
</ul>
<br>
<br>
<h2>電路圖：</h2>



