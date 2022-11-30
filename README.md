# PVOutput-Power-One-Aurora-ABB-Blackpill

Arduino library to poll an Power-One (ABB) Aurora inverter and upload the readed values to PVoutput.org.

The sketch is written for a Blackpill SOC in conjunction with external flash W25Q128, RS-485 duplex module and an ethernet W5500 module.<br>
Some features:<br>
- Polls every 10 seconds the Power One Aurora for most recent yield and status. When PVI is sleeping it polls once a minute with a short time to keep     webserver performance for clients.<br>
- Uploads every 5 minutes to pvoutput.org.<br>
- At end of daylight it sends two mails containing dayyield, weather and status updates.<br>
- Before midnight update to pvoutput for pluvio updates.<br>
- Send email alerts for different uncommon conditions.<br>
- Retrieving weather from weerlive.nl<br>
- Retreaving rain information from buienradar.nl<br>
- Using NTP.<br>
- Automatic switching between summer/winter time.<br>
- registering dynamic DNS at freedns.com.<br>
- Detecting WAN IP changes and reregistering at freedns.<br>
- Controls 3 fans for inverter cooling preventing power degradation at hot days.<br>
- Includes a webserver for remote control and statusinformation.<br>
- Acts as a honeypot and reports to abuseipdb.<br>
- Uses wolfSSL for TLS connections.<br>
- Has different loglevels for monitoring and debugging.<br>
- Has TFTP server for up- and download whitelist IP en blacklist HTTP commands if honeypot functionality is desired.
<br>
Rename userdefs_empty.h to userdefs.h and make all the necessary keys personal.<br>
SPI0 is used for flash and ethernet.<br>
<br>
All connections from specific module → Black pill<br>
Wiring diagram: for relay board, all the relays on the board has the same function, so pick your favorite positions.<br>
Relay_fanC    → PB6<br>
Relay_Power_R → PB7<br>
Relay_fanB    → PB8<br>
Relay_Power   → PB9<br>
Relay_C_DIST  → PB12<br>
Relay_fanA    → PB13<br>
<br>
keyswitch     → PA0  // keyswitch is PA0, but is forced low at activation<br>
<br>
RS-485 module:<br>
GND → GND → G<br>
TXD → TX2 → PA2 UART used for RS-485<br>
RXD → RX2 → PA3  UART used for RS-485<br>
Vcc → 5V<br>
<br>
Ethernet module<br>
5V → 5V<br>
GND → G<br>
SCS → PA15 (ETHERNET_SS)<br>
MISO → A6<br>
MOSI → A7<br>
SCLK → A5<br>
RST → Not Connected<br>
INT Not Connected<br>
<br>
flash module:<br>
Vcc → 3,3V<br>
CS  → FLASH_SS → PA4<br>
DO  → A6<br>
GND → G → GND<br>
CLK → A5<br>
DI  → A7<br>
<br>
A flash module soldered to the back of the Black pill could be used
<br>
The system is based on many different code snippets from around. AFAIK no license infringements for personal use.<br>
The sketch needs about 400kB of ROM and the necessary memory for TLS connections, a Black pill fulfills this, a Blue pill has insufficient space to accomodate the requirements.<br>
<br>

![blackpill-v2](https://user-images.githubusercontent.com/10601859/204104731-bc914b1f-df20-4c2a-9d31-30f6142fd4bf.jpg)
![Blackpill_Pinout1](https://user-images.githubusercontent.com/10601859/204104740-60a7a9cd-f710-499a-9736-66a5dcf68e12.png)
![W5500](https://user-images.githubusercontent.com/10601859/204104969-a928f4c0-71a8-410d-a407-b95739595a6f.jpg)
![w5500back](https://user-images.githubusercontent.com/10601859/204110043-442cb5fb-81ec-4ecf-85bd-cdf6a9f45011.jpeg)
![RS485](https://user-images.githubusercontent.com/10601859/204104552-89688f90-0441-424a-b63f-f716f95fc0c2.jpg)
![W25Q32-W25Q64-W25Q128-Norflash-Opslag-Module-32Mbit-64Mbit-128Mbit-Flash-Opslag-Controle-Module-Spi-interface-STM32](https://user-images.githubusercontent.com/10601859/204105194-f2666641-6d25-42c5-8856-bac904f237fd.jpg)
![8relay](https://user-images.githubusercontent.com/10601859/204109138-024d6533-d693-4059-b235-392ba3f33888.jpg)

