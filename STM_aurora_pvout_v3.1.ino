/*
  ---------------------------------------
  Aurora Power One to PVOUTPUT.ORG
  ---------------------------------------
*/
//======================================
//For testing delete the /* below to disable (production) for production please outcomment the next line definition with a /*
//======================================
/*
  #define USE_TEST_LAN_IP true
  #define OPTION_NO_PVOUTPUT true
  #define OPTION_NO_WEATHER true
  #define OPTION_NO_PLUVIO true
  #define OPTION_NO_PVI_POLL true
  #define OPTION_NO_ABUSEIPDB true
  #define OPTION_NO_EMAIL true
  /*
*/

#define PVIVersion "V3.1 "
#define Release "R126"
#define COMPILEDATE __DATE__
#define COMPILETIME __TIME__

#define OPTION_REPORT_ALL_2_ABUSEIPDB true  // report every breach every 20 minutes one at a time
#define OPTION_FLASH true
#define USE_DHCP true

/*
  COMPILE IN ARDUINO VERSION 1.8.19
  v3.1 migration to wolfSSL
  v3.0 temperature changed from class to tab

  V2.8 and higher compile with 1.8.13 or 1.8.19 with modified ethernet library (in Arduino version library map)
  adapted for 1.8.6 nightly 2018/09/07 03:25
  tested with 1.8.7 with ethernet lib 1.1.2, version2 shows
  8 sockets and hangs. test started on 30-sep-2018 ethernet 2 lib with max_num_sock defined in sketch
  tested with 1.8.13
  Creation Date: 25.8.2014
  Author: Bert Havinga after an idea from Heinz Pieren
  18-10-2015 peakpower calculation changed to unsigned int, update dns to first of month
  21 aug 2020 adapted to STM32
  FLASH on SPI 1
  RS485 on Serial2(PA3, PA2)
  RS485 direction pin is deprecated due to RS485 module with autodirection
  Relay_fanA PB6
  Relay_fanB PB7
  Relay_fanC PB8

  spi flash w25q32 is mounted together with ethernet on SPI1
  On 411CE the SPIClass SPITwo(PB15, PB14, PB10); // MOSI, MISO, SCLK
  But we use this as IO
*/



//===================================
#define WELCOME "\r\nAurora Power One STM32 PVOUTPUT.ORG uploader "
#define NAME "B. Havinga"
#define RELAY_ON 0
#define RELAY_OFF 1
#define Relay_fanA PB13
#define Relay_fanB PB8
#define Relay_fanC PB6
#define Relay_Power PB9
#define Relay_C_DIST PB12
#define Relay_Power_R PB7


#define keyswitch PA0  // keyswitch is PA0, but is forced low at activation and usage of UART2 in testboard PB12 has extern switch (unreliable)
#define TX2 PA2
#define RX2 PA3
#define FLASH_SS PA4
#define ETHERNET_SS PA15

#define longRS485Wait 900
#define shortRS485Wait 300
#ifndef Buienradar_h
#define Buienradar_h
#endif
#include "Arduino.h"
#include "userdefs.h"
#include <Ethernet.h>
#include <Dns.h>
#include <FlashMini.h>
#include <wolfssl.h>
#include <wolfssl/ssl.h>
#include <TimeLib.h>
#include <SPIMemory.h>
#include <string.h>
//#ifdef WOLFSSL_TRACK_MEMORY
//#include <wolfssl/wolfcrypt/mem_track.h>
//#endif


#if defined (USE_LOGGING) || defined (USE_PWRLOGGING)
#include <SD.h>
#endif
#define MAX_SOCK_NUM 4

#if defined(ARCH_STM32)|| defined(ARDUINO_ARCH_ESP32) || defined(STM32F4xx)
#define SUSPCMDSIZE 384
#define SUSPARRAYDEPTH 25
char suspicious_string[SUSPARRAYDEPTH][SUSPCMDSIZE]; // the suspicious content
byte suspicious_ips[SUSPARRAYDEPTH][4]; // The IP of the sender
int suspicious_perc[SUSPARRAYDEPTH]; // The percentage of suspicion
int suspicious_length[SUSPARRAYDEPTH]; // the length of the suspicious content
time_t suspicious_time[SUSPARRAYDEPTH]; // the time the datagram is received
int suspicious_index = 0; // the actual entry in the array
int last_suspicious_index = 0; // the actual last reported array entry
int suspicious_total = 0;

#define UNKWNCMDARRAYDEPTH 10
char unknownCmd_string[UNKWNCMDARRAYDEPTH][SUSPCMDSIZE]; // the unknown command
int unknownCmd_index = 0; // the actual entry in the array
byte unknownCmd_ips[UNKWNCMDARRAYDEPTH][4]; // The IP of the sender
bool unknownCmd_wkwnips[UNKWNCMDARRAYDEPTH];  // from well known IP
int unknownCmd_perc[UNKWNCMDARRAYDEPTH]; // The percentage of suspicion
int unknownCmd_length[UNKWNCMDARRAYDEPTH]; // the length of the suspicious content
time_t unknownCmd_time[UNKWNCMDARRAYDEPTH]; // the time the datagram is received
bool unknownCmd_sema = false; // semaphore <> 0 is unknown command queued
byte unknownCmd_queued[UNKWNCMDARRAYDEPTH]; // 0= not queued, 1 = queued, 2 = submitted
int unknownCmd_total = 0;
#endif

unsigned long G_SocketConnectionTimers[MAX_SOCK_NUM]; //system millis
unsigned int Sock_DisCon[MAX_SOCK_NUM];  //aantal keren gerest
char lastSock_DisCon[MAX_SOCK_NUM][24];
// global variables
byte lastMonth;
byte lastDayReset = 0;
byte lastHour;
byte lastMinute;
byte lastSecond;
byte iMonth;
byte iDay;
byte iHour;
byte iMinute;
byte iSecond;
byte strd_iMonth = 0;
byte lastiMinuteShdw = 0;
byte lastiHourShdw = 0;
byte lanmode;
byte macBuffer[6];  // create a buffer to hold the MAC address

unsigned long wCLength = 0;
bool AlreadyAdded = false;
bool min_serial = true;  // log messages for USB Serial port
bool ethrlog = false;
bool iplog = false;
bool maillogging = false;
bool pvoutputlog = false;
bool showbufferremaining = false;
bool wolfssllog = false;
bool pluvio_ena = true;
bool AlreadyDailySend = false;
bool AlreadyMonthSend = false;
bool udpdump = false;
bool HTTPlog = false;
bool WolfSSLrwmail = false;
bool exc_plv_err = false;
bool InExlog = false;
bool invlogging = false;
bool brlogging = false;
bool abuslogging = false;
bool mailSendErrorflag = false;
bool mailRcvErrorflag = false;
bool SerInpRcdv = false;
bool suspicious_mail = true;
bool unknownCmd_mail = true;
bool tftpReqEna = false;
unsigned long upTime = 0;               // the amount of hours the Arduino is running

int ChkSumErrCnt = 0;
int errorwolfsslwantread = 0;
int errorwolfsslwantwrite = 0;
int errorHTMLnocharavlbl = 0;
int tmpChkSumErrCnt;
int nr_flsh_error = 0;
int PeakPowerObserved;  // used to determine the moment of peakpower this day
int PeakPowerInObserved = 0;
//int uplPeakPowerObserved; //used to determine if time for peakpower to upload needs new time
int pwrPeak = 0;
int prevPwrPeak = 0;
int rprt2abuseipdb = 0;
int rprt2abuseipdberr = 0;
int shadow = 0;
int timeDiff;
int unkcmdfrmwhl = 0;
//int EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx, EthernetClient wtlsclient);
//int EthernetReceive(WOLFSSL* ssl, char* reply, int sz, void* ctx, EthernetClient wtlsclient);
int pluvio_last_RXdata_size;
int pluvio_max_RXdata_size = 0; //max observed packet size received form pluvio buienradar
int pluvio_min_RXdata_size = 4096; //min observed packet size received form pluvio buienradar
int pluvio_last_HTTP_headersize;
int pluvio_min_HTTP_headersize = 4096; //min observed html header size received form pluvio buienradar
int pluvio_max_HTML_headersize = 0; //max observed html header size received form pluvio buienradar
int ab_last_HTML_headersize;
int ab_last_RXdata_size;
int lastsocket18close = 0;
int abuse_well_known_hits = 0;
int abuse_well_known_scans = 0;
unsigned int currentInvtime; // current time
uint16_t totalnets = 0;
uint16_t totalHTTPcmd = 0;
uint32_t dayYield[93];
unsigned long oldtime;
unsigned long newtime;
unsigned int peakpower5min = 0; // value for peakpower in 5 minute lap
unsigned long MaxcmltvPwr = 0; // used for uploading
float actTension = 0.0;
float prevTension = 0.0;
unsigned int GridPower = 0;
unsigned int PrevGridPower = 0;
unsigned int totalToday = 0;
uint32_t maxtotalToday = 0;
unsigned long cmltvPwr;
float actInvTemp = 0.0;
float startInvTemp = 0.0;
float maxInvTemp = 0.0;
float actBstTemp = 0.0;
float actDc1Voltage = 0.0;
float actDc1Current = 0.0;
float actDc2Voltage = 0.0;
float actDc2Current = 0.0;
float rendement = 0.0;
float Riso = 0.0;
float invRiso = 0.0;
float BrCorrFact = 1.0;
//int PowerGeneratedDayOfMonth[31];
int Power1;
int Power2;
int PVIPolled = 0;
int luchtvochtigheid;
int luchtDrukMin3 = 1020;
//int lastGridPower; // used to surpress comm errors in case the PVI is sleeping
unsigned long uploadsOk = 0;
unsigned long totalUploadsOk = 0;
unsigned long uploadErr = 0;
unsigned long totalUploadsErr = 0;
/*
  int crcErrGridVoltage = 0;
  int crcErrGridPower = 0;
  int crcErrgetTempBst = 0;
  int crcErrgetTempInv = 0;
  int crcErrgetDc1Voltage = 0;
  int crcErrgetDc1Current = 0;
  int crcErrgetDc2Voltage = 0;
  int crcErrgetDc2Current = 0;
  int crcErrgetRiso = 0;
  int crcErrgetTime = 0;
  int crcErrgetEnergyDay = 0;
  int crcErrgetEnergyTotal = 0;
  int crcErrgetGridPowerPeakDay = 0;
*/
unsigned long    minRxTime = 300;
unsigned long    maxRxTime = 0;
bool rxTimeflag = false;
int cntrespTimeout = 0;
int tmpcntrespTimeout;
// int rxReadCnt = 0;
unsigned long receiveMaxRS485Wait = shortRS485Wait;  //at start use the short. in the loop determine the long or short timeout for PVI packet receive
int todayIPCnt = 0;
int dynDNSCnt = 0;
unsigned int socket0x18close = 0;
unsigned int fanAOnCount = 0;
unsigned int fanBOnCount = 0;
unsigned int fanCOnCount = 0;
unsigned int fanAOnTotalCount;
unsigned int fanBOnTotalCount;
unsigned int fanCOnTotalCount;
unsigned long runTimefanA;
unsigned long runTimefanB;
unsigned long runTimefanC;
unsigned int runTimeThisDayfanA = 0;
unsigned int runTimeThisDayfanB = 0;
unsigned int runTimeThisDayfanC = 0;
unsigned int runTimeThisCyclefanA = 0;
unsigned int runTimeThisCyclefanB = 0;
unsigned int runTimeThisCyclefanC = 0;
const unsigned int fanApowerOn = 1200;
const unsigned int fanBpowerOn = 1800;
const unsigned int fanCpowerOn = 2000;
const unsigned int fanApowerOff = 1000;
const unsigned int fanBpowerOff = 1200;
const unsigned int fanCpowerOff = 1100;
float tempFanAOff = 36.5; // fan A off
float tempFanBOff = 38.0; // fan B off
float tempFanCOff = 37.0; // fan C off
float tempFanAOn = 38.0; // fan A on
float tempFanBOn = 39.0; // fan B on
float tempFanCOn = 40.0; // fan C on
char webData[96];
// char wc[66];  //temp array for writing to flash
time_t resetRxTime = 0;
time_t sketchStartTime = 0;
time_t pluvio_max_RXdata_time = 0;
time_t pluvio_min_RXdata_time = 0;
time_t pluvio_min_HTTP_header_time = 0;
time_t pluvio_max_HTTP_header_time = 0;
unsigned long abuse_array_scan_start_time = 0;
unsigned long abuse_array_scan_run_time = 0;
char hPaTrend = '-';
char TimestartInvTemp[6];
char TimePeakPower[6];
char TimePeakPowerIn[6];
//char uplTimePeakPower[6];  // the time used for addoutput rounded at 5 minutes, pvoutput rounds itself
char TimePeakTemp[6];  // time inverter temp max
char ab_lastresponse[32] = "None";
char maillog[512];
char dynDNSresponse[64];
char mailerrortype[64];
char ob[8];
char lastarrtime[8];
char looptime[32];
char msgchararray[64];
char lastsocket18closetime[24];
char firstShadowTime[] = "not today";
char lastShadowTime[] = "not today";
char flasherror[256];
char c_poll_tijd[12];
char mailsubj[128];
char temptxtbuff[1024];
char mailcontent[80];
char mailbrstate[80];
String weatherCondition1300 = "";           // Weathercondition at 13:00
String flash_error = "";

bool ab_rslt;
bool br_rslt;
bool GetTemp_Ok;
bool force_temperature;
//  bool min_serial;
bool showbuffer;
bool resetmaxpluvio;
unsigned int tick = 0;
unsigned int nosockavail = 0;
unsigned int todayCnt;
unsigned int todayCnt_br;
unsigned int todayErrCnt;
unsigned int todayErrCnt_br;
unsigned int totalErrCnt_br = 0;
unsigned int todayErrCnt_ab = 0;
unsigned int totalErrCnt_ab = 0;
unsigned int MaxFailErrCnt_br;
unsigned int pluviosucc_br;
unsigned int visibility;
int lv_phi;
int hk;
int windkrachtBft;
float MinTemp;                                // Minimum Temperature
float MaxTemp;                                // Maximum Temperature
float prevActual;  // the previous temperature from the metering meteo station
float last_neerslagintensiteit;
float max_neerslagintensiteit;
float gdFactor;
float pluvio_br;
int arrpluvio_br[24];
int arrhour_br[24];
int arrminute_br[24];
float totexpectedPluvio_br = 0.0;
float totPluvio_br;
float actual;
float windsnelheidMS;
float luchtdruk;
time_t tmp_tijd;
char abErrTime_ab[32];

char windrichting[8];
char c_tijd_Temp[16];
char c_tijd_Pluv[16];
char br_tijd[16];
char meetStation[16];
char weatherType[64];
char icon_temperature[16];
char TimeMinTemp[16];
char TimeMaxTemp[16];
char weatherProcTime[16];
char pluviosuccTime_br[16];
char weatherErrTime_br[24];
char pluvioProcTime_br[16];
char pluvioErrTime_br[32];
time_t lastpluvioTime_br;
time_t maxpluvioTime_br;
char nextpluvioTime_br[16];
char samenvatting[384];
//  float average;
char* weatherStation;

#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

IPAddress UDPSendTo(192, 168, 0, 255);

#if defined (USE_LOGGING) || defined (USE_PWRLOGGING)
File   logFile;
#endif

EthernetServer server(555);  // port changed from 80 to 555
EthernetUDP Udp;
EthernetUDP UDPclient;
EthernetUDP TFTPclient;

//-----------------------------------------------------------------------------
// SPI instance: use 2 or 3 for black F4, because the on-board LED conflicts with SPI1_MISO(PA6)
#ifdef OPTION_FLASH
SPIFlash flash(FLASH_SS);
#endif
HardwareSerial Serial2(RX2, TX2);

//-----------------------------------------------------------------------
// Update Inverter
//-----------------------------------------------------------------------
void updatePVI()
{
#ifdef OPTION_NO_PVI_POLL
  actBstTemp = 0.0;
  actInvTemp = 0.0;
  actDc1Voltage = 0.0;
  actDc2Voltage = 0.0;
  actDc1Current = 0.0;
  actDc2Current = 0.0;
  Power1 = 0;
  Power2 = 0;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Inverter polling is disabled by build option.", hour(), minute(), second());    // temptxtbuff is max 1024 char
  textlog(temptxtbuff, false);
  //  return;
#endif

  if (!min_serial || (upTime == 0) ) {
    if ((lastSecond == 5) ||  (todayCnt == 0) || (lastSecond == 30)  || (upTime == 0) || SerInpRcdv) {
      SerInpRcdv = false;
      time_t poll_tijd = now();
      sprintf(c_poll_tijd, "%02d:%02d:%02d", hour(poll_tijd), minute(poll_tijd), second(poll_tijd));  // c_poll_tijd is max 12 char
      sprintf(temptxtbuff, "\r\n%s polling PVI: Begin ", c_poll_tijd);  // temptxtbuff is max 1024 char
      textlog(temptxtbuff, false);
    } else {
      textStringLog("Begin ", false);
    }
  }

#ifndef OPTION_NO_PVI_POLL
  getGridVoltage();  // Sends the command get current power feeding to grid
  //  Serial.print(F("spanning: "));
  //  Serial.println(actTension);
  if (actTension > 0.0) {
    PVIPolled++;
    delay(60);
    getGridPowerPeakDay();  // Sends the command get the grid power peak current day (collects pwrPeak)
    delay(60);
    getGridPower(); // Sends the command get current power feeding to grid
    if (GridPower > peakpower5min) {
      peakpower5min = GridPower;
    }
    delay(60);
    getTempBst();
    delay(60);
    getTempInv();
    delay(60);
    getDc1Voltage();
    delay(60);
    getDc1Current();
    delay(60);
    getDc2Voltage();
    delay(60);
    getDc2Current();
    delay(60);
    getRiso();
    if ( Riso > 0.0 ) {
      invRiso = Riso;
    }
    delay(60);
    getEnergyDay();  // Sends the command get the cumulated energy current day
    delay(60);
    getEnergyTotal();  // Sends the command get the total cumulated
    if (PVIPolled == 90) // check time 15 minutes after wake/start
    {
      setinvTime();
    }

    if (MaxcmltvPwr < cmltvPwr)
      MaxcmltvPwr = cmltvPwr; // store for off line use
    if (maxtotalToday < totalToday )
      maxtotalToday = totalToday; // store for off line use
    float Pwr1 = actDc1Voltage * actDc1Current;
    float Pwr2 = actDc2Voltage * actDc2Current;
    Power1 = (int)Pwr1;
    Power2 = (int)Pwr2;
    int inputPower = Power1 + Power2;
    // mark only the max inputpower if the input is max 20% > inputpower and the output power is gt 50W, 1-12-2017 modified to 15. This prevents startup peakrecording.
    // verwijderd uit de onderste regel van if ((unsigned int)(pwrPeak * 10) > (unsigned int)(inputPower * 10)) &&
    if ( (((unsigned int)(pwrPeak * 12) > (unsigned int)(inputPower * 10)) && (inputPower > PeakPowerInObserved) && (GridPower > 15)) ||
         (((inputPower > PeakPowerInObserved) && (GridPower > 0) && (inputPower < 50))) )
    {
      PeakPowerInObserved = Power1 + Power2;
      sprintf(TimePeakPowerIn, "%02d:%02d", hour(), minute());  // max is 6 char
    }
    if ((Power1 != 0) && (Power2 != 0)) {
      rendement =  (float)GridPower / (float)((Pwr1 + Pwr2) / (float)100.0);
    }
    else {
      rendement = 0;
    }
    //  if (prevPwrPeak == pwrPeak && pwrPeak > PeakPowerObserved) { // if pwrPeak is two cycles the same, the reading is valid
    // was ook geen verbetering, daarom nu direct gekoppeld aan rendement
    if ( (prevPwrPeak == pwrPeak) && (pwrPeak > PeakPowerObserved) && (rendement < 100.0)) { // if pwrPeak is two cycles the same, the reading is valid
      PeakPowerObserved = pwrPeak;
      sprintf(TimePeakPower, "%02d:%02d", hour(), minute());  // max is 6 char
    }
    prevPwrPeak = pwrPeak;
    if ((startInvTemp == 0.0) && (actInvTemp > 5.0)) {
      startInvTemp = actInvTemp;
      sprintf(TimestartInvTemp, "%02d:%02d", hour(), minute());  // TimestartInvTemp max is 6
    }
    if (actInvTemp > maxInvTemp)
    {
      maxInvTemp = actInvTemp;
      sprintf(TimePeakTemp, "%02d:%02d", hour(), minute());  // max is 6 char
    }
  }
  if ((prevTension > 0.0) && (actTension == 0.0)) {
    actBstTemp = 0.0;
    actInvTemp = 0.0;
    actDc1Voltage = 0.0;
    actDc2Voltage = 0.0;
    actDc1Current = 0.0;
    actDc2Current = 0.0;
    Power1 = 0;
    Power2 = 0;
  }
#endif
  if (!min_serial || (upTime == 0)) {
    //                                               overdag              ||                       's avonds tbv pluvio en weer                                                                           ||
    //if ( ((lastSecond == 55) && (receiveMaxRS485Wait == longRS485Wait)) || ( (receiveMaxRS485Wait == shortRS485Wait) && ( ((lastMinute % 5) == 3) || (((lastMinute % 5) == 0)) && (lastSecond == 0) ) ) || ((receiveMaxRS485Wait == shortRS485Wait) && brlogging) || upTime == 0 || todayCnt == 0 ) {
    //                                             overdag              ||    's avonds tbv pluvio en weer         || start
    //    if ( ((lastSecond == 55) && (receiveMaxRS485Wait == longRS485Wait)) || (receiveMaxRS485Wait == shortRS485Wait) || upTime == 0 || todayCnt == 0 ) {
    //      textStringLog("End\r\n", false);
    //    }
    //    else {
    textStringLog("End ", false);
    //    }
  }
}

void StoreFlashError(String sectorname, long sectoraddr) {
  char cobuff[256];
  /*
    Serial.print("type schrijffout: ");
    Serial.println(sectorname);
    Serial.print("sectoraddr: ");
    Serial.println(sectoraddr, 16);
  */
  int slt = sectorname.length();
  if (slt > 238 ) {
    slt = 238; // leave space for ERROR and address
  }
  sectorname.toCharArray(cobuff, slt + 1); // cobuff is max 256 char
  //  cobuff[slt] = '\0'; //toCHarArray sets terminator
  sprintf(flasherror, "\r\nERROR: %s:0x%08lx", cobuff, sectoraddr);  // flasherror is max 256 char
  //  Serial.print("flasherror: ");
  //  Serial.println(flasherror);
  // char flsh_err[80];
  // sprintf(flsh_err, "\r\nERROR, %s", flasherror);
  // textlog(flsh_err, true);
  textlog(flasherror, true);
  nr_flsh_error++;
}

void StoreDayYield( byte dy, byte mnth) {
  // compute the index for the month
  byte monthIndex = (mnth - 1) % 3;
  unsigned int err;
  int idxpos = 0;
  if (!min_serial || (upTime == 0)) {
    idxpos = dy - 1 + (monthIndex * 31);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Yield written to RAM array address: %d, value: %u -> ", hour(), minute(), second(), idxpos, maxtotalToday);  // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  // add the harvest of the day
  dayYield[(dy - 1) + (monthIndex * 31)] = maxtotalToday;
  textStringLog("maxtotalToday stored in RAM array", false);

#ifdef OPTION_FLASH
  uint32_t workaddr;
  uint32_t val2store;
  err = 0;
  workaddr = monthBase_addr + (((monthIndex * 31) + dy - 1) * EEPROM_PAGE_SIZE);
  if (!flash.eraseSector( workaddr )) {
    flash_error = "eraseSector monthBase_addr FAILED! ";
    StoreFlashError(flash_error, monthBase_addr);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: sector erasure for daily yield", hour(), minute(), second());
    textlog(temptxtbuff, false);
  }
  delay(25);
  val2store = dayYield[dy - 1 + (monthIndex * 31)];
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Yield from today %uWh stored in dayYield array to flash address = 0x%08lx day: %d month: %d calculated index: %d", hour(), minute(), second(), val2store, workaddr, dy, mnth, monthIndex);  // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  delay(25);
  if (!flash.writeULong( workaddr, val2store)) {
    flash_error = F("WriteULong monthBase_addr FAIL");
    StoreFlashError(flash_error, workaddr);
  }
  delay(25);
#endif
}

void storeWeatherCondition1300() {
  // store the weathertype from 13:00
  wCLength = weatherCondition1300.length();
  if (wCLength > 63)
  {
    char chararraystring[64];
    wCLength = 63;
    weatherCondition1300.toCharArray(chararraystring, wCLength + 1);
    //   chararraystring[wCLength]= '\0'; //toCHarArray sets terminator
    weatherCondition1300 = String(chararraystring);
    //    wCLength = 60; //maximize to the end of flash
    //    weatherCondition1300[60] = '\0';
  }
#ifdef OPTION_FLASH
  flash.eraseSector(wCLength_addr);
  if (!flash.writeLong(wCLength_addr, wCLength)) {
    flash_error = F("WriteULong wCLength_addr FAIL");
    StoreFlashError(flash_error, wCLength_addr);
  }
  delay(3);
  flash.eraseSector(weatherCondition1300_addr);
  delay(25);
  if (!flash.writeStr(weatherCondition1300_addr, weatherCondition1300)) {
    flash_error = F("WriteStr weatherCondition1300_addr FAIL");
    StoreFlashError(flash_error, weatherCondition1300_addr);
  }
  delay(3);
#endif
}

void ReadWeatherCondition1300() {
#ifdef OPTION_FLASH
  char RWCcobuff[16];
  wCLength = flash.readLong(wCLength_addr);
  textStringLog("Length of restored weather condition @13:00 = ", false);
  itoa(wCLength, RWCcobuff, 10);
  textlog(RWCcobuff, false);
  if ((wCLength != 0) && (wCLength != 0xFFFFFFFF)) {
    flash.readStr(weatherCondition1300_addr, weatherCondition1300);
    textStringLog("\r\nweather condition @13:00 restored to: ", false);
    char costr[128];
    strcpy(costr, weatherCondition1300.c_str());  //costr is max 128 char
    textlog(costr, false);
    textStringLog("\r\nstringlength from function: ", false);
    itoa(weatherCondition1300.length(), RWCcobuff, 10);
    textlog(RWCcobuff, false);
  }
  else {
    textStringLog("\r\nweather condition @13:00 is not restored", false);
  }
#endif
}

void StoreDayTotal()  // userdefs.h contains the memorymap
{
#ifdef OPTION_FLASH
  flash.eraseSector(shadow_addr);
  delay(25);
  if (!flash.writeLong(shadow_addr, shadow)) {
    flash_error = F("writeLong shadow_addr FAIL");
    StoreFlashError(flash_error, shadow_addr);
  }
  delay(3);
  flash.eraseSector(firstShadowTime_addr);
  delay(25);
  if (!flash.writeAnything(firstShadowTime_addr, firstShadowTime)) {
    flash_error = F("writeAnything firstShadowTime_addr FAIL");
    StoreFlashError(flash_error, firstShadowTime_addr);
  }
  delay(3);
  flash.eraseSector(lastShadowTime_addr);
  delay(25);
  if (!flash.writeAnything(lastShadowTime_addr, lastShadowTime)) {
    flash_error = F("writeAnything lastShadowTime_addr FAIL");
    StoreFlashError(flash_error, lastShadowTime_addr);
  }
  delay(3);
  flash.eraseSector(fanAOnCount_addr);
  delay(25);
  if (!flash.writeLong(fanAOnCount_addr, fanAOnCount)) {
    flash_error = F("writeLong fanAOnCount_addr FAIL");
    StoreFlashError(flash_error, fanAOnCount_addr);
  }
  delay(3);
  flash.eraseSector(fanBOnCount_addr);
  delay(25);
  if (!flash.writeLong(fanBOnCount_addr, fanBOnCount)) {
    flash_error = F("writeLong fanBOnCount_addr FAIL");
    StoreFlashError(flash_error, fanBOnCount_addr);
  }
  delay(3);
  flash.eraseSector(fanCOnCount_addr);
  delay(25);
  if (!flash.writeLong(fanCOnCount_addr, fanCOnCount)) {
    flash_error = F("writeLong fanCOnCount_addr FAIL");
    StoreFlashError(flash_error, fanCOnCount_addr);
  }
  delay(3);
  flash.eraseSector(runTimeThisDayfanA_addr);
  delay(25);
  if (!flash.writeLong(runTimeThisDayfanA_addr, runTimeThisDayfanA)) {
    flash_error = F("writeLong runTimeThisDayfanA_addr FAIL");
    StoreFlashError(flash_error, runTimeThisDayfanA_addr);
  }
  delay(3);
  flash.eraseSector(runTimeThisDayfanB_addr);
  delay(25);
  if (!flash.writeLong(runTimeThisDayfanB_addr, runTimeThisDayfanB)) {
    flash_error = F("writeLong runTimeThisDayfanB_addr FAIL");
    StoreFlashError(flash_error, runTimeThisDayfanB_addr);
  }
  delay(3);
  flash.eraseSector(runTimeThisDayfanC_addr);
  delay(25);
  if (!flash.writeLong(runTimeThisDayfanC_addr, runTimeThisDayfanC)) {
    flash_error = F("writeLong runTimeThisDayfanC_addr FAIL");
    StoreFlashError(flash_error, runTimeThisDayfanC_addr);
  }
  delay(3);
  flash.eraseSector(TimeMinTemp_addr);
  delay(25);
  if (!flash.writeAnything(TimeMinTemp_addr, TimeMinTemp)) {
    flash_error = F("writeAnything TimeMinTemp_addr FAIL");
    StoreFlashError(flash_error, TimeMinTemp_addr);
  }
  delay(3);
  flash.eraseSector(TimeMaxTemp_addr);
  delay(25);
  if (!flash.writeAnything(TimeMaxTemp_addr, TimeMaxTemp)) {
    flash_error = F("writeAnything TimeMaxTemp_addr FAIL");
    StoreFlashError(flash_error, TimeMaxTemp_addr);
  }
  delay(3);
  flash.eraseSector(TimePeakTemp_addr);
  delay(25);
  if (!flash.writeAnything(TimePeakTemp_addr, TimePeakTemp)) {
    flash_error = F("writeAnything TimePeakTemp_addr FAIL");
    StoreFlashError(flash_error, TimePeakTemp_addr);
  }
  delay(3);
  flash.eraseSector(TimestartInvTemp_addr);
  delay(25);
  if (!flash.writeAnything(TimestartInvTemp_addr, TimestartInvTemp)) {
    flash_error = F("writeAnything TimePeakTemp_addr FAIL");
    StoreFlashError(flash_error, TimePeakTemp_addr);
  }
  delay(3);
  flash.eraseSector(saveFlag_addr);
  delay(25);
  if (!flash.writeByte(saveFlag_addr, 0xC5)) { //flag for saving this day
    flash_error = F("writeByte saveFlag_addr FAIL");
    StoreFlashError(flash_error, saveFlag_addr);
  }
  delay(3);
  flash.eraseSector(startInvTemp_addr);
  delay(25);
  if (!flash.writeFloat(startInvTemp_addr, startInvTemp)) {
    flash_error = F("writeFloat startInvTemp_addr FAIL");
    StoreFlashError(flash_error, startInvTemp_addr);
  }
  delay(3);
  flash.eraseSector(maxInvTemp_addr);
  delay(25);
  if (!flash.writeFloat(maxInvTemp_addr, maxInvTemp)) {
    flash_error = F("writeFloat maxInvTemp_addr FAIL");
    StoreFlashError(flash_error, maxInvTemp_addr);
  }
  delay(3);
  flash.eraseSector(runTimefanA_addr);
  delay(25);
  if (!flash.writeLong(runTimefanA_addr, runTimefanA)) { // store the total fan runtime
    flash_error = F("writeLong runTimefanA_addr FAIL");
    StoreFlashError(flash_error, runTimefanA_addr);
  }
  delay(3);
  flash.eraseSector(runTimefanB_addr);
  delay(25);
  if (!flash.writeLong(runTimefanB_addr, runTimefanB)) {
    flash_error = F("writeLong runTimefanB_addr FAIL");
    StoreFlashError(flash_error, runTimefanB_addr);
  }
  delay(3);
  flash.eraseSector(runTimefanC_addr);
  delay(25);
  if (!flash.writeLong(runTimefanC_addr, runTimefanC)) {
    flash_error = F("writeLong runTimefanC_addr FAIL");
    StoreFlashError(flash_error, runTimefanC_addr);
  }
  delay(3);
  flash.eraseSector(maxtotalToday_addr);
  delay(25);
  if (!flash.writeLong(maxtotalToday_addr, maxtotalToday)) {
    flash_error = F("writeLong maxtotalToday_addr FAIL");
    StoreFlashError(flash_error, maxtotalToday_addr);
  }
  delay(3);
  flash.eraseSector(PeakPowerInObserved_addr);
  delay(25);
  if (!flash.writeLong(PeakPowerInObserved_addr, PeakPowerInObserved)) {
    flash_error = F("writeLong PeakPowerInObserved_addr FAIL");
    StoreFlashError(flash_error, PeakPowerInObserved_addr);
  }
  delay(3);
  flash.eraseSector(TimePeakPowerIn_addr);
  delay(25);
  if (!flash.writeAnything(TimePeakPowerIn_addr, TimePeakPowerIn)) {
    flash_error = F("writeAnything TimePeakPowerIn_addr FAIL");
    StoreFlashError(flash_error, TimePeakPowerIn_addr);
  }
  delay(3);
  flash.eraseSector(PeakPowerObserved_addr);
  delay(25);
  if (!flash.writeLong(PeakPowerObserved_addr, PeakPowerObserved)) {
    flash_error = F("writeLong PeakPowerObserved_addr FAIL");
    StoreFlashError(flash_error, PeakPowerObserved_addr);
  }
  delay(3);
  flash.eraseSector(TimePeakPower_addr);
  delay(25);
  if (!flash.writeAnything(TimePeakPower_addr, TimePeakPower)) {
    flash_error = F("writeAnything TimePeakPower_addr FAIL");
    StoreFlashError(flash_error, TimePeakPower_addr);
  }
  delay(3);
  flash.eraseSector(MaxcmltvPwr_addr);
  delay(25);
  if (!flash.writeLong(MaxcmltvPwr_addr, MaxcmltvPwr)) {
    flash_error = F("writeLong MaxcmltvPwr_addr FAIL");
    StoreFlashError(flash_error, MaxcmltvPwr_addr);
  }
  delay(3);
  flash.eraseSector(invRiso_addr);
  delay(25);
  if (!flash.writeFloat(invRiso_addr, invRiso)) {
    flash_error = F("writeFloat invRiso_addr FAIL");
    StoreFlashError(flash_error, invRiso_addr);
  }
  delay(3);
  flash.eraseSector(MaxTemp_addr);
  delay(25);
  if (!flash.writeFloat(MaxTemp_addr, MaxTemp)) {
    flash_error = F("writeFloat MaxTemp_addr FAIL");
    StoreFlashError(flash_error, MaxTemp_addr);
  }
  delay(3);
  flash.eraseSector(MinTemp_addr);
  delay(25);
  if (!flash.writeFloat(MinTemp_addr, MinTemp)) {
    flash_error = F("writeFloat MinTemp_addr FAIL");
    StoreFlashError(flash_error, MinTemp_addr);
  }
  delay(3);
  flash.eraseSector(prevActual_addr);
  delay(25);
  if (!flash.writeFloat(prevActual_addr, prevActual)) {
    flash_error = F("writeFloat prevActual_addr FAIL");
    StoreFlashError(flash_error, prevActual_addr);
  }
  delay(3);
  flash.eraseSector(luchtvochtigheid_addr);
  delay(25);
  if (!flash.writeLong(luchtvochtigheid_addr, luchtvochtigheid)) {
    flash_error = F("writeLong luchtvochtigheid_addr FAIL");
    StoreFlashError(flash_error, luchtvochtigheid_addr);
  }
  delay(3);
  flash.eraseSector(AlreadyAdded_addr);
  delay(25);
  if (!flash.writeByte(AlreadyAdded_addr, AlreadyAdded)) {
    flash_error = F("writeByte AlreadyAdded_addr FAIL");
    StoreFlashError(flash_error, AlreadyAdded_addr);
  }
  delay(3);
  flash.eraseSector(AlreadyDailySend_addr);
  delay(25);
  if (!flash.writeByte(AlreadyDailySend_addr, AlreadyDailySend)) {
    flash_error = F("writeByte AlreadyDailySend_addr FAIL");
    StoreFlashError(flash_error, AlreadyDailySend_addr);
  }
  delay(3);
  flash.eraseSector(AlreadyMonthSend_addr);
  delay(25);
  if (!flash.writeByte(AlreadyMonthSend_addr, AlreadyMonthSend)) {
    flash_error = F("writeByte AlreadyMonthSend_addr FAIL");
    StoreFlashError(flash_error, AlreadyMonthSend_addr);
  }
  delay(3);
  flash.eraseSector(totPluvio_br_addr);
  delay(25);
  if ( !(isnan(totPluvio_br) || isinf(totPluvio_br)) ) {
    if (!flash.writeFloat(totPluvio_br_addr, totPluvio_br)) {
      flash_error = F("writeFloat totPluvio_br_addr FAIL");
      StoreFlashError(flash_error, totPluvio_br_addr);
    }
  }
  else {
    flash_error = F("writeFloat totPluvio_br_addr FAIL, ERROR totPluvio_br contains not a number!");
    textStringLog("ERROR totPluvio_br contains not a number!\7", true);
  }
  delay(3);
  flash.eraseSector(last_neerslagintensiteit_addr);
  delay(25);
  if (!flash.writeFloat(last_neerslagintensiteit_addr, last_neerslagintensiteit)) {
    flash_error = F("writeFloat last_neerslagintensiteit_addr FAIL");
    StoreFlashError(flash_error, last_neerslagintensiteit_addr);
  }
  delay(3);
  flash.eraseSector(lastpluvioTime_br_addr);
  delay(25);
  if (!flash.writeLong(lastpluvioTime_br_addr, lastpluvioTime_br)) {
    flash_error = F("writeLong lastpluvioTime_br_addr FAIL");
    StoreFlashError(flash_error, lastpluvioTime_br_addr);
  }
  delay(3);
  flash.eraseSector(max_neerslagintensiteit_addr);
  delay(25);
  if (!flash.writeFloat(max_neerslagintensiteit_addr, max_neerslagintensiteit)) {
    flash_error = F("writeFloat max_neerslagintensiteit_addr FAIL");
    StoreFlashError(flash_error, max_neerslagintensiteit_addr);
  }
  delay(3);
  flash.eraseSector(maxpluvioTime_br_addr);
  delay(25);
  if (!flash.writeLong(maxpluvioTime_br_addr, maxpluvioTime_br)) {
    flash_error = F("writeLong maxpluvioTime_br_addr FAIL");
    StoreFlashError(flash_error, maxpluvioTime_br_addr);
  }
  delay(3);
  storeWeatherCondition1300();
#endif
}

void RestoreDayTotal() {
#ifdef OPTION_FLASH
  textStringLog(" = += += += RestoreDayTotal from FLASH = += += += ", true);
  byte strd_saveFlag;
  strd_saveFlag = flash.readByte(saveFlag_addr);
  if (strd_saveFlag != 0xC5) {
    textStringLog(" = -= -= -Complete restore skipped. -= -= -= Checking Already Added to PVoutput", true);

    //restore the flag if already updated output
    AlreadyAdded = flash.readByte(AlreadyAdded_addr);
    //restore the flag if already daily mail send
    AlreadyDailySend = flash.readByte(AlreadyDailySend_addr);
    //restore the flag if already output for this month is send
    AlreadyMonthSend = flash.readByte(AlreadyMonthSend_addr);

    if (!AlreadyAdded) {
      if (iHour >= 13) {
        ReadWeatherCondition1300();
        textStringLog("partial restore... Weather from 13:00 restored", true);
      }
    }
    return;
  }
  textStringLog("Restore started.", true);
  shadow = flash.readLong(shadow_addr);  // 3725
  fanAOnCount = flash.readULong(fanAOnCount_addr);
  fanBOnCount = flash.readULong(fanBOnCount_addr);
  fanCOnCount = flash.readULong(fanCOnCount_addr);

  runTimeThisDayfanA = flash.readULong(runTimeThisDayfanA_addr);
  runTimeThisDayfanB = flash.readULong(runTimeThisDayfanB_addr);
  runTimeThisDayfanC = flash.readULong(runTimeThisDayfanC_addr);

  flash.readAnything(TimeMinTemp_addr, TimeMinTemp); //3739,40, 41, 42, 43, 3744, this is the time for minweatherTemp
  flash.readAnything(TimeMaxTemp_addr, TimeMaxTemp); //3745,6,7,8,9,3750, this is the time for max weather Temp
  flash.readAnything(TimePeakTemp_addr, TimePeakTemp); //3751,2,3, 4,5,3756, this is the time for peak InvTemp
  flash.readAnything(TimestartInvTemp_addr, TimestartInvTemp); //3628,9,0, 1,2,3633, this is the time for InvTemp @ wakeup
  flash.readAnything(firstShadowTime_addr, firstShadowTime);
  flash.readAnything(lastShadowTime_addr, lastShadowTime);

  maxtotalToday = flash.readULong(maxtotalToday_addr); // 4000 & 4001
  totalToday = maxtotalToday;
  if ( !isnan(flash.readFloat(maxInvTemp_addr))) {
    maxInvTemp = flash.readFloat(maxInvTemp_addr);
  }
  else {
    maxInvTemp = 0.0;
    textStringLog("ERROR: reading maxInvTemp from EEPROM", true);
  }
  if ( !isnan(flash.readFloat(startInvTemp_addr))) {
    startInvTemp = flash.readFloat(startInvTemp_addr);
  }
  else {
    startInvTemp = 0.0;
    textStringLog("ERROR: reading startInvTemp from EEPROM", true);
  }
  PeakPowerObserved = flash.readULong(PeakPowerObserved_addr); // 4002 & 4003
  PeakPowerInObserved = flash.readULong(PeakPowerInObserved_addr); // 3792 & 3793

  // restore the time for the PeakPower
  // we'll need to write the string contents
  // plus the string terminator byte (0x00)
  flash.readAnything(TimePeakPower_addr, TimePeakPower);
  flash.readAnything(TimePeakPowerIn_addr, TimePeakPowerIn);
  // restore for the webpage

  // restore the total generated power yield
  MaxcmltvPwr = flash.readULong(MaxcmltvPwr_addr); // 4010, 11, 12, 13

  // restore the isolation
  if ( !isnan(flash.readFloat(invRiso_addr))) {
    invRiso = flash.readFloat(invRiso_addr);    // 4014, 15, 16, 17
  }
  else {
    invRiso = 0.0;
    textStringLog("ERROR: reading invRiso from EEPROM", true);
  }
  // read maxtemp
  if ( !isnan(flash.readFloat(MaxTemp_addr))) {
    MaxTemp = flash.readFloat(MaxTemp_addr);    // 4018, 19, 20, 21
  }
  else {
    MaxTemp = -255.0;
    textStringLog("ERROR: reading MaxTemp from EEPROM", true);
  }
  // read mintemp
  if ( !isnan(flash.readFloat(MinTemp_addr))) {
    MinTemp = flash.readFloat(MinTemp_addr);  // 4022, 23, 24, 25
  }
  else {
    MinTemp = 255.0;
    textStringLog("ERROR: reading MinTemp from EEPROM", true);
  }
  // read mintemp

  if ( !isnan(flash.readFloat(prevActual_addr))) {
    prevActual = flash.readFloat(prevActual_addr);  // 3623, 24, 25, 26
  }
  else {
    prevActual = 0.0;
    textStringLog("ERROR: reading prevActual from EEPROM", true);
  }
  // read humidity
  luchtvochtigheid = flash.readULong(luchtvochtigheid_addr); // 4026, 4027

  // restore rainlevel
  if ( !isnan(flash.readFloat(totPluvio_br_addr)) || !isinf(flash.readFloat(totPluvio_br_addr))) {
    totPluvio_br = flash.readFloat(totPluvio_br_addr);  // 4029, 30, 31, 32
  }
  else {
    totPluvio_br = 0.0;
    textStringLog("ERROR: reading totPluvio_br from EEPROM", true);
  }
  if ( !isnan(flash.readFloat(last_neerslagintensiteit_addr))) {
    last_neerslagintensiteit = flash.readFloat(last_neerslagintensiteit_addr); // 3638 39 40 41
  }
  else {
    last_neerslagintensiteit = 0.0;
    textStringLog("ERROR: reading last_neerslagintensiteit from EEPROM", true);
  }
  flash.readAnything(lastpluvioTime_br_addr, lastpluvioTime_br); // 3642 43 44 45

  if ( !isnan(flash.readFloat(max_neerslagintensiteit_addr))) {
    max_neerslagintensiteit = flash.readFloat(max_neerslagintensiteit_addr); // 3646 47 48 49
  }
  else {
    max_neerslagintensiteit = 0.0;
    textStringLog("ERROR: reading max_neerslagintensiteit from EEPROM", true);
  }
  if (flash.readULong(maxpluvioTime_br_addr) != 0xFFFFFFFF) {
    flash.readAnything(maxpluvioTime_br_addr, maxpluvioTime_br);
  }
  else {
    maxpluvioTime_br = 0;
    textStringLog("ERROR: reading maxpluvioTime_br from EEPROM", true);
  }
  ReadWeatherCondition1300();
  /* if we have gridtension then already email send is always false */
  receiveMaxRS485Wait = shortRS485Wait; // we need this in getGridVoltage to suppress message
  Serial2.setTimeout(receiveMaxRS485Wait);
#ifdef OPTION_NO_PVI_POLL
  getGridVoltage();
  textStringLog("Actual gridvoltage: ", false);
  dtostrf( actTension, 3, 2, temptxtbuff );
  textlog(temptxtbuff, true);
#endif
  if (actTension == 0.0) {
    textStringLog("\r\nStatus for AlreadyDailySend & AlreadyMonthSend read from EEPROM", true);
    //restore the flag if already daily mail send
    AlreadyDailySend = flash.readByte(AlreadyDailySend_addr);
    //restore the flag if already output for this month is send
    AlreadyMonthSend = flash.readByte(AlreadyMonthSend_addr);
  }
  else {
    char c_tens[8];
    dtostrf( actTension, 3, 2, c_tens );  // c_tens is max 8 char
    sprintf(temptxtbuff, "due to actual gridtension of %sVolt, AlreadyDailySend & AlreadyMonthSend forced to false", c_tens); // temptxtbuff is maax 1024
    textlog(temptxtbuff, true);
    AlreadyDailySend = false;
    AlreadyMonthSend = false;
  }

  textStringLog("Restore ready", true);
#else
  textStringLog("Nothing to restore, no flash", true);
  //==================================================================================================
  /*  MinTemp = 10.30;
    sprintf(TimeMinTemp, "09:26");
    MaxTemp = 14.40;
    sprintf(TimeMaxTemp, "16:16");
    luchtvochtigheid = 87;
    totPluvio_br = 09.23;
    weatherCondition1300 = "Geheel bewolkt";
    invRiso = 17.27;
    shadow = 0;
    totalToday  = 2921;
    maxtotalToday = totalToday;
    MaxcmltvPwr = 26021;
    AlreadyAdded = true;
    AlreadyDailySend = true;
    AlreadyMonthSend = true;
    //==================================================================================================
  */
#endif
}

void eraseDayYield() {
#ifndef OPTION_FLASH
  textStringLog(" = += += += No FLASH detected erase day yield not possible, erasing RAM array = += += += ", true);
  dayYield[day() - 1 + ((month() % 3) * 31)] = 0;
  //init the DayYield array in RAM
#else
  byte strd_saveFlag;
  strd_saveFlag = flash.readByte(saveFlag_addr);
  if (strd_saveFlag != 0xFF) {
    if (!flash.eraseSector(saveFlag_addr)) {
      flash_error = F("saveFlag_addr");
      StoreFlashError(flash_error, saveFlag_addr); //flag for saving this day
    }
  }
  unsigned int strd_maxtotalToday0;
  strd_maxtotalToday0 = flash.readByte(maxtotalToday_addr);
  if (strd_maxtotalToday0 != 0xFF) {
    if (!flash.eraseSector(maxtotalToday_addr)) {
      flash_error = F("maxtotalToday_addr");
      StoreFlashError(flash_error, maxtotalToday_addr);
    }
  }
#endif
}

String msgEncode(const char *src) {
  const char *hexarray = "0123456789ABCDEF";
  String target = "";
  int charcnt = 0;
  if (maillogging || pvoutputlog) {
    textlog("\r\nString contains before conversion: ", false);
  }
  while ((*src != '\0') && (charcnt < 255))
  {
    if (maillogging || pvoutputlog) {
      Serial.print(*src);
    }
    if (('a' <= *src && *src <= 'z') || ('A' <= *src && *src <= 'Z') || ('0' <= *src && *src <= '9') || *src == '-' || *src == '_' || *src == '.' || *src == '~')
    {
      target += *src;
      charcnt++;
    }
    else
    {
      target += '%';
      target += hexarray[*src >> 4];
      target += hexarray[*src & 0xf];
      charcnt += 3;
    }
    src++;
  }
  if (maillogging || pvoutputlog) {
    Serial.print("\r\nConverted string: ");
    Serial.print(target);
  }
  return target;
}

String string2HTML(String source)
{
  return msgEncode(source.c_str());
}

void updateApiKey()
{
#ifndef OPTION_FLASH
  textStringLog(" = += += += No FLASH detected, DNS update API key not possible = += += += ", true);
#else
  char uAKcobuff[64];
  //  Serial.println(DYNDNS_API_KEY);
  textStringLog("Using SPI Flash address: ", false);
  itoa(DYNDNS_API_KEY_addr, uAKcobuff, 16); // uAKcobuff is max 64 char
  textlog(uAKcobuff, true);
  flash.eraseSector(DYNDNS_API_KEY_addr);
  if (!flash.writeStr(DYNDNS_API_KEY_addr, DYNDNS_API_KEY)) {
    flash_error = F("writeStr DYNDNS_API_KEY_addr FAIL");
    StoreFlashError(flash_error, DYNDNS_API_KEY_addr);
  }
  textStringLog("APIkey updated for DYNDNS", false);
  textStringLog("new stored string: ", false);
  String tmpstr;
  flash.readStr(DYNDNS_API_KEY_addr, tmpstr);
  strcpy(uAKcobuff, tmpstr.c_str());
  textlog(uAKcobuff, false);
  sprintf(uAKcobuff, " length DNS key is: %d ", tmpstr.length());
  textlog(uAKcobuff, true);
#endif
}

//-----------------------------------------------------------------------
// Init stuff
//-----------------------------------------------------------------------

//one time setup after wiped storage
void restore_counters(byte force)
//  force==false read counters from flash
//  force==true  read counters store counters
{
  textStringLog("Forcing fan counters...", true);
#ifdef OPTION_FLASH
  if (force == 0) {
    runTimefanA = flash.readULong(runTimefanA_addr);
    runTimefanB = flash.readULong(runTimefanB_addr);
    runTimefanC = flash.readULong(runTimefanC_addr);
    fanAOnTotalCount = flash.readULong(fanAOnTotalCount_addr);
    fanBOnTotalCount = flash.readULong(fanBOnTotalCount_addr);
    fanCOnTotalCount = flash.readULong(fanCOnTotalCount_addr);
    if ( !isnan(flash.readFloat(BrCorrFact_addr))) {
      BrCorrFact = flash.readFloat(BrCorrFact_addr);
    }
    else {
      BrCorrFact = 1.0;
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: reading setBrCorrFact from EEPROM, set to default: 1.00\r\n", hour(), minute(), second());
      textlog(temptxtbuff, false);
    }
  }
  else {
    flash.eraseSector(runTimefanA_addr);
    if (!flash.writeULong(runTimefanA_addr, runTimefanA)) {
      flash_error = F("writeULong runTimefanA_addr FAIL");
      StoreFlashError(flash_error, runTimefanA_addr);
    }
    delay(3);
    flash.eraseSector(runTimefanB_addr);
    if (!flash.writeULong(runTimefanB_addr, runTimefanB)) {
      flash_error = F("writeULong runTimefanB_addr FAIL");
      StoreFlashError(flash_error, runTimefanB_addr);
    }
    delay(3);
    flash.eraseSector(runTimefanC_addr);
    if (!flash.writeULong(runTimefanC_addr, runTimefanC)) {
      flash_error = F("writeULong runTimefanC_addr FAIL");
      StoreFlashError(flash_error, runTimefanC_addr);
    }
    delay(3);
    flash.eraseSector(fanAOnTotalCount_addr);
    if (!flash.writeULong(fanAOnTotalCount_addr, fanAOnTotalCount)) {
      flash_error = F("writeULong fanAOnTotalCount_addr FAIL");
      StoreFlashError(flash_error, fanAOnTotalCount_addr);
    }
    delay(3);
    flash.eraseSector(fanBOnTotalCount_addr);
    if (!flash.writeULong(fanBOnTotalCount_addr, fanBOnTotalCount)) {
      flash_error = F("writeULong fanBOnTotalCount_addr FAIL");
      StoreFlashError(flash_error, fanBOnTotalCount_addr);
    }
    delay(3);
    flash.eraseSector(fanCOnTotalCount_addr);
    if (!flash.writeULong(fanCOnTotalCount_addr, fanCOnTotalCount)) {
      flash_error = F("writeULong fanCOnTotalCount_addr FAIL");
      StoreFlashError(flash_error, fanCOnTotalCount_addr);
    }
    delay(3);
    min_serial = false;
    flash.eraseSector(min_serial_addr);
    if (!flash.writeByte(min_serial_addr, min_serial)) {
      flash_error = F("writeULong min_serial_addr FAIL");
      StoreFlashError(flash_error, min_serial_addr);
    }
    showbuffer = false;
    flash.eraseSector(showbuffer_addr);
    if (!flash.writeByte(showbuffer_addr, showbuffer)) {
      flash_error = F("writeULong showbuffer_addr FAIL");
      StoreFlashError(flash_error, showbuffer_addr);
    }
    delay(3);
    ethrlog = false;
    flash.eraseSector(ethrlog_addr);
    if (!flash.writeByte(ethrlog_addr, ethrlog)) {
      flash_error = F("writeULong ethrlog_addr FAIL");
      StoreFlashError(flash_error, ethrlog_addr);
    }
    delay(3);
    iplog = false;
    flash.eraseSector(iplog_addr);
    if (!flash.writeByte(iplog_addr, iplog)) {
      flash_error = F("writeULong iplog_addr FAIL");
      StoreFlashError(flash_error, iplog_addr);
    }
    delay(3);
    maillogging = false;
    flash.eraseSector(maillogging_addr);
    if (!flash.writeByte(maillogging_addr, maillogging)) {
      flash_error = F("writeULong maillogging_addr FAIL");
      StoreFlashError(flash_error, maillogging_addr);
    }
    delay(3);
    pvoutputlog = false;
    flash.eraseSector(pvoutputlog_addr);
    if (!flash.writeByte(pvoutputlog_addr, pvoutputlog)) {
      flash_error = F("writeULong pvoutputlog_addr FAIL");
      StoreFlashError(flash_error, pvoutputlog_addr);
    }
    delay(3);
    showbufferremaining = false;
    flash.eraseSector(showbufferremaining_addr);
    if (!flash.writeByte(showbufferremaining_addr, showbufferremaining)) {
      flash_error = F("writeULong showbufferremaining_addr FAIL");
      StoreFlashError(flash_error, showbufferremaining_addr);
    }
    delay(3);
    udpdump = false;
    flash.eraseSector(udpdump_addr);
    if (!flash.writeULong(udpdump_addr, udpdump)) {
      flash_error = F("writeULong udpdump_addr FAIL");
      StoreFlashError(flash_error, udpdump_addr);
    }
    delay(3);
    HTTPlog = false;
    flash.eraseSector(HTTPlog_addr);
    if (!flash.writeULong(HTTPlog_addr, HTTPlog)) {
      flash_error = F("writeULong HTTPlog_addr FAIL");
      StoreFlashError(flash_error, HTTPlog_addr);
    }
    delay(3);
    WolfSSLrwmail = false;
    flash.eraseSector(wolfSSLMail_addr);
    if (!flash.writeByte(wolfSSLMail_addr, WolfSSLrwmail)) {
      flash_error = "writeByte wolfSSLMail_addr FAIL";
      StoreFlashError(flash_error, wolfSSLMail_addr);
    }
    delay(3);
    flash.eraseSector(firstShadowTime_addr);
    if (!flash.writeAnything(firstShadowTime_addr, firstShadowTime)) {
      flash_error = F("writeAnything firstShadowTime_addr FAIL");
      StoreFlashError(flash_error, firstShadowTime_addr);
    }
    delay(3);
    flash.eraseSector(lastShadowTime_addr);
    if (!flash.writeAnything(lastShadowTime_addr, lastShadowTime)) {
      flash_error = F("writeAnything lastShadowTime_addr FAIL");
      StoreFlashError(flash_error, lastShadowTime_addr);
    }
    delay(3);
    flash.eraseSector(pluvio_ena_addr);
    if (!flash.writeAnything(pluvio_ena_addr, pluvio_ena)) {
      flash_error = F("writeAnything pluvio_ena_addr FAIL");
      StoreFlashError(flash_error, pluvio_ena_addr);
    }
    delay(3);
    flash.eraseSector(invlogging_addr);
    if (!flash.writeAnything(invlogging_addr, invlogging)) {
      flash_error = F("writeAnything invlogging_addr FAIL");
      StoreFlashError(flash_error, invlogging_addr);
    }
    delay(3);
    flash.eraseSector(brlogging_addr);
    if (!flash.writeAnything(brlogging_addr, brlogging)) {
      flash_error = F("writeAnything brlogging_addr FAIL");
      StoreFlashError(flash_error, brlogging_addr);
    }
    delay(3);
    flash.eraseSector(abuslogging_addr);
    if (!flash.writeAnything(abuslogging_addr, abuslogging)) {
      flash_error = F("writeAnything abuslogging_addr FAIL");
      StoreFlashError(flash_error, abuslogging_addr);
    }
    delay(3);
    flash.eraseSector(suspicious_mail_addr);
    if (!flash.writeAnything(suspicious_mail_addr, suspicious_mail)) {
      flash_error = F("writeAnything suspicious_mail_addr FAIL");
      StoreFlashError(flash_error, suspicious_mail_addr);
    }
    delay(3);
    flash.eraseSector(unknownCmd_mail_addr);
    if (!flash.writeAnything(unknownCmd_mail_addr, unknownCmd_mail)) {
      flash_error = F("writeAnything unknownCmd_mail_addr FAIL");
      StoreFlashError(flash_error, unknownCmd_mail_addr);
    }
    delay(3);
    flash.eraseSector(BrCorrFact_addr);
    if (!flash.writeFloat(BrCorrFact_addr, BrCorrFact)) {
      flash_error = F("writeFloat BrCorrFact_addr FAIL");
      StoreFlashError(flash_error, BrCorrFact_addr);
    }
    delay(3);
    flash.eraseSector(InExlog_addr);
    if (!flash.writeByte(InExlog_addr, InExlog)) {
      flash_error = F("writeByte InExlog_addr FAIL");
      StoreFlashError(flash_error, InExlog_addr);
    }
    delay(3);
  }
#endif
}

void check_fan_hysteris() {
  float strd_TempFan;
  char cfhcobuff[16];
  // negeer indien kleiner dan 35 of groter dan 50
  textStringLog("\r\nChecking fanswitch hysteresis", false);
#ifdef OPTION_FLASH
  tempFanAOn = flash.readFloat(tempFanAOn_addr);
  strd_TempFan = tempFanAOn;
  textStringLog("\r\nStored temperature to switch on fan A: ", false);
  String fpstring = String(strd_TempFan, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(cfhcobuff, 6); // cfhcobuff is max 16 char
  textlog(cfhcobuff, false);
#else
  strd_TempFan = tempFanAOn;
#endif
  if (isnan(strd_TempFan) || (strd_TempFan < 30.0) || (strd_TempFan > 50.0))
  {
    tempFanAOn = 38.0; // fan A on temp
#ifdef OPTION_FLASH
    flash.eraseSector(tempFanAOn_addr);
    if (!flash.writeFloat(tempFanAOn_addr, tempFanAOn)) {
      flash_error = F("writeFloat tempFanAOn_addr FAIL");
      StoreFlashError(flash_error, tempFanAOn_addr);
    }
#endif
    textStringLog("Fan A fan \"on\" temperature forced to 38.0", true);
  }
#ifdef OPTION_FLASH
  tempFanBOn = flash.readFloat(tempFanBOn_addr);
  strd_TempFan = tempFanBOn;
  textStringLog("\r\nStored temperature to switch on fan B: ", false);
  fpstring = String(strd_TempFan, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(cfhcobuff, 6); // cfhcobuff is max 16 char
  textlog(cfhcobuff, false);
#else
  strd_TempFan = tempFanBOn;
#endif
  if (isnan(strd_TempFan) || (strd_TempFan < 30.0) || (strd_TempFan > 50))
  {
    tempFanBOn = 39.0; // fan B on temp
#ifdef OPTION_FLASH
    flash.eraseSector(tempFanBOn_addr);
    if (!flash.writeFloat(tempFanBOn_addr, tempFanBOn)) {
      flash_error = F("writeFloat tempFanBOn_addr FAIL");

      StoreFlashError(flash_error, tempFanBOn_addr);
    }
    delay(3);
#endif
    textStringLog("Fan B fan \"on\" temperature forced to 39.0", true);
  }
#ifdef OPTION_FLASH
  tempFanCOn = flash.readFloat(tempFanCOn_addr);
  strd_TempFan = tempFanCOn;
  textStringLog("\r\nStored temperature to switch on fan C: ", false);
  fpstring = String(strd_TempFan, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(cfhcobuff, 6); // cfhcobuff is max 16 char
  textlog(cfhcobuff, false);
#else
  strd_TempFan = tempFanCOn;
#endif
  if (isnan(strd_TempFan) || (strd_TempFan < 30.0) || (strd_TempFan > 50))
  {
    tempFanCOn = 40.0; // fan C on temp
#ifdef OPTION_FLASH
    flash.eraseSector(tempFanCOn_addr);
    if (!flash.writeFloat(tempFanCOn_addr, tempFanCOn)) {
      flash_error = F("writeFloat tempFanCOn_addr FAIL");

      StoreFlashError(flash_error, tempFanCOn_addr);
    }
    delay(3);
#endif
    textStringLog("Fan C fan \"on\" temperature forced to 40.0", true);
  }
#ifdef OPTION_FLASH
  tempFanAOff = flash.readFloat(tempFanAOff_addr);
  strd_TempFan = tempFanAOff;
  textStringLog("\r\nStored temperature to switch off fan A: ", false);
  fpstring = String(strd_TempFan, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(cfhcobuff, 6); // cfhcobuff is max 16 char
  textlog(cfhcobuff, false);
#else
  strd_TempFan = tempFanAOff;
#endif
  if ((strd_TempFan < 30.0) || (strd_TempFan > 50) || isnan(strd_TempFan))
  {
    tempFanAOff = 36.5; // fan A off temp
#ifdef OPTION_FLASH
    flash.eraseSector(tempFanAOff_addr);
    if (!flash.writeFloat(tempFanAOff_addr, tempFanAOff)) {
      flash_error = F("writeFloat tempFanAOff_addr FAIL");
      StoreFlashError(flash_error, tempFanAOff_addr);
    }
    delay(3);
#endif
    textStringLog("Fan A fan \"off\" temperature forced to 36.5", true);
  }
#ifdef OPTION_FLASH
  tempFanBOff = flash.readFloat(tempFanBOff_addr);
  strd_TempFan = tempFanBOff;
  textStringLog("\r\nStored temperature to switch off fan B: ", false);
  fpstring = String(strd_TempFan, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(cfhcobuff, 6); // cfhcobuff is max 16 char
  textlog(cfhcobuff, false);
#else
  strd_TempFan = tempFanBOff;
#endif
  if (isnan(strd_TempFan) || (strd_TempFan < 30.0) || (strd_TempFan > 50))
  {
    tempFanBOff = 38.0; // fan B off temp
#ifdef OPTION_FLASH
    flash.eraseSector(tempFanBOff_addr);
    if (!flash.writeFloat(tempFanBOff_addr, tempFanBOff)) {
      flash_error = F("writeFloat tempFanBOff_addr FAIL");
      StoreFlashError(flash_error, tempFanBOff_addr);
    }
    delay(3);
#endif
    textStringLog("Fan B fan \"off\" temperature forced to 38.0", true);
  }
#ifdef OPTION_FLASH
  tempFanCOff = flash.readFloat(tempFanCOff_addr);
  strd_TempFan = tempFanCOff;
  textStringLog("\r\nStored temperature to switch off fan C: ", false);
  fpstring = String(strd_TempFan, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(cfhcobuff, 6); // cfhcobuff is max 16 char
  textlog(cfhcobuff, false);
#else
  strd_TempFan = tempFanCOff;
#endif
  if ((strd_TempFan < 30.0) || (strd_TempFan > 50) || isnan(strd_TempFan))
  {
    tempFanCOff = 37.0; // fan C off temp
#ifdef OPTION_FLASH
    flash.eraseSector(tempFanCOff_addr);
    if (!flash.writeFloat(tempFanCOff_addr, tempFanCOff)) {
      flash_error = F("writeFloat tempFanCOff_addr FAIL");
      StoreFlashError(flash_error, tempFanCOff_addr);
    }
    delay(3);
#endif
    textStringLog("Fan C fan \"off\" temperature forced to 37.0", true);
  }
}

void PowerupFanA() {
  sprintf(temptxtbuff, "\r\n%s Preparing power supply for fans, fan A: preparing ", looptime); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  delay(50);
  digitalWrite(Relay_Power_R, RELAY_ON);
  delay(500);
  textlog("-> snubber resistor enabled ", false);
  delay(50);
  digitalWrite(Relay_C_DIST, RELAY_ON);
  delay(500);
  textlog("-> snubber capacitor enabled ", false);
  delay(50);
  digitalWrite(Relay_fanA, RELAY_ON);
  delay(500);
  textlog("-> fan A enabled", false);
  delay(50);
  digitalWrite(Relay_Power, RELAY_ON);// set the Relay ON
  delay(1500);
  sprintf(temptxtbuff, "\r\n%s power enabled -> Fan A: running ", looptime); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  delay(150);
  digitalWrite(Relay_Power_R, RELAY_OFF);// set the Relay ON
  delay(750);
  textlog("-> snubber resistor disabled ", false);
  delay(150);
  digitalWrite(Relay_C_DIST, RELAY_OFF);
  delay(900);
  textlog("-> snubber capacitor disabled", false);
  runTimeThisCyclefanA = 0;
  fanAOnCount++;
}

void setup() {
  maillog[0] = '\0';
  int el = 0;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  while (!Serial) {
    delay(100);
    el++;
    if (el == 20) break;
  }
  //  Serial.begin( 28800 );  // console terminal for log info
  //  Serial.begin( 57600 );  // console terminal for log info
  Serial.begin( 115200 );  // console terminal for log info
  Serial2.begin(19200);    // to aurora PVI
  Serial.println("Remember 31 mai 2018 A.L. H - d H, 3 december 2019 Cleo");
  Serial.print("--- Build: ");
  Serial.print(F(COMPILEDATE));
  Serial.print(F("  "));
  Serial.print(F(COMPILETIME));
#if defined(ARDUINO)
  Serial.print(F(" with ARDUINO version: "));
  Serial.println(ARDUINO);
#endif
  init_Temperature();
  for (uint8_t sock = 0; sock < MAX_SOCK_NUM; sock++) {
    sprintf(lastSock_DisCon[sock], "Never");
  }
  sprintf(lastsocket18closetime, "Never");
#ifdef OPTION_FLASH
  Serial.println("init flash");
  flash.setClock(10000000);
  flash.begin();
  getIDSPImem();
  Serial.println();
  if (eepromEnd_addr > flash.getCapacity()) {
    Serial.print("ERROR! SPI flash size error, flash size too small.");
  }
  Serial.print("Detected flashsize in bytes: ");
  Serial.println(flash.getCapacity());
  Serial.print(F("Flash size used: "));
  Serial.println(eepromEnd_addr);
#endif
  // store rainlevel buienradar ap
  weatherCondition1300.reserve(64);
  //  restore_counters();
  // to rewritten to SPI bus memory

#ifdef OPTION_FLASH
  min_serial = flash.readByte(min_serial_addr);
  showbuffer = flash.readByte(showbuffer_addr);
  ethrlog = flash.readByte(ethrlog_addr);
  iplog = flash.readByte(iplog_addr);
  maillogging = flash.readByte(maillogging_addr);
  pvoutputlog = flash.readByte(pvoutputlog_addr);
  showbufferremaining = flash.readByte(showbufferremaining_addr);
  udpdump = flash.readByte(udpdump_addr);
  HTTPlog = flash.readByte(HTTPlog_addr);
  WolfSSLrwmail = flash.readByte(wolfSSLMail_addr);
  pluvio_ena = flash.readByte(pluvio_ena_addr);
  invlogging = flash.readByte(invlogging_addr);
  brlogging = flash.readByte(brlogging_addr);
  abuslogging = flash.readByte(abuslogging_addr);
  suspicious_mail = flash.readByte(suspicious_mail_addr);
  unknownCmd_mail = flash.readByte(unknownCmd_mail_addr);
  wolfssllog = flash.readByte(wolfSSLdbg_addr);
  InExlog = flash.readByte(InExlog_addr);
  if (udpdump == true) {
    Serial.println("Forcing UDPdump off/disabled");
    flash.eraseSector(udpdump_addr);
    udpdump = false;
    if (!flash.writeByte(udpdump_addr, udpdump)) {
      Serial.println("Error forcing UDPdump off");
    }
  }
#endif

  //---( THEN set pins as outputs )----
  pinMode(Relay_fanA, OUTPUT);
  pinMode(Relay_fanB, OUTPUT);
  pinMode(Relay_fanC, OUTPUT);
  pinMode(Relay_Power, OUTPUT);
  pinMode(Relay_C_DIST, OUTPUT);
  pinMode(Relay_Power_R, OUTPUT);
  pinMode(keyswitch, INPUT_PULLUP);
  //-------( Initialize Pins so relays are inactive at reset)----
  digitalWrite(Relay_Power, RELAY_OFF);
  digitalWrite(Relay_Power_R, RELAY_OFF);
  digitalWrite(Relay_fanA, RELAY_OFF);
  digitalWrite(Relay_fanB, RELAY_OFF);
  digitalWrite(Relay_fanC, RELAY_OFF);
  digitalWrite(Relay_C_DIST, RELAY_OFF);

  Serial.print(F(WELCOME));
  Serial.print(' ');
  Serial.print(F(PVIVersion));
  Serial.println(Release);
  Serial.print(F("Compile time: "));
  Serial.print(F(COMPILEDATE));
  Serial.print(' ');
  Serial.println(F(COMPILETIME));
  Serial.println("Blue LED on during setup or KEY pressed.");
  Serial.print(F("extended Serial logging: "));
  min_serial ? Serial.println("on") : Serial.println("off");
  Serial.print(F("showbuffer: "));
  showbuffer ? Serial.println("on") : Serial.println("off");
  Serial.print(F("ethrlog logging: "));
  ethrlog ? Serial.println("on") : Serial.println("off");
  Serial.print(F("iplog logging: "));
  iplog ? Serial.println("on") : Serial.println("off");
  Serial.print(F("maillog logging: "));
  maillogging ? Serial.println("on") : Serial.println("off");
  Serial.print(F("pvoutputlog logging: "));
  pvoutputlog ? Serial.println("on") : Serial.println("off");
  Serial.print(F("showbufferremaining: "));
  showbufferremaining ? Serial.println("on") : Serial.println("off");
  Serial.print(F("udpdump: "));
  udpdump ? Serial.println("on") : Serial.println("off");
  Serial.print(F("HTTPlog: "));
  HTTPlog ? Serial.println("on") : Serial.println("off");
  Serial.print(F("WolfSSLrwmail: "));
  WolfSSLrwmail ? Serial.println("on") : Serial.println("off");
  showbuffer ?  Serial.println(F("Weather showbuffer: on")) : Serial.println(F("Weather showbuffer: off"));
  Serial.print(F("Reading pluvio from buienradar.nl: "));
  pluvio_ena ?  Serial.println("enabled") : Serial.println("disabled");
  Serial.print(F("Inverter logging: "));
  invlogging ?  Serial.println("enabled") : Serial.println("disabled");
  Serial.print(F("buienradar.nl logging: "));
  brlogging ?  Serial.println("enabled") : Serial.println("disabled");
  Serial.print(F("AbuseIPDB logging: "));
  abuslogging ?  Serial.println("enabled") : Serial.println("disabled");
  Serial.print(F("suspicious_mailing: "));
  suspicious_mail ?  Serial.println("enabled") : Serial.println("disabled");
  Serial.print(F("unknownCmd_mailing: "));
  unknownCmd_mail ?  Serial.println("enabled") : Serial.println("disabled");
  Serial.print(F("wolfSSL logging: "));
  Serial.println(wolfssllog ? F("enabled") : F("disabled"));
  Serial.print(F("Import/Export logging: "));
  Serial.println(InExlog ? F("enabled") : F("disabled"));

#ifdef OPTION_NO_PVOUTPUT
  Serial.println(F("WARNING: This version is compiled with definition OPTION_NO_PVOUTPUT is true, so no output is uploaded to PVORG."));
#endif


  int virgin;
#ifdef OPTION_FLASH
  flash.readAnything(fanAOnTotalCount_addr, virgin);
#endif
  if ( virgin == -1 ) {
    Serial.println(F("Erased EEPROM detected. After router startup, the EEPROM will be initialized!"));
  }
  delay(250);
  // empty serial buffer
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.println(F("Waiting 1 minute for router startup"));
  bool quickstart = false;
  for (byte i = 0; i < 60; i++) {
    for (byte i2 = 0; i2 < 5; i2++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
    if (Serial.available() > 0) {
      // read the incoming byte(s):
      while ( Serial.available() ) {
        Serial.read();
      }
      quickstart = true;
      digitalWrite(LED_BUILTIN, LOW);
      break;
    }
    Serial.print(".");
  }
#ifdef OPTION_FLASH
  virgin = flash.readULong(fanAOnTotalCount_addr);
  Serial.print(F("\r\nFLASH virgin value readed: 0x"));
  Serial.print(virgin, HEX);
  if ( virgin == -1 ) {
    Serial.println(F("\r\ninitializing FLASH...!"));
    restore_counters(0xFF);
    unsigned long membase = monthBase_addr;
    for ( int re = 0 ; re < 93; re++) {
      if (!flash.eraseSector(membase)) {
        flash_error = F("membase");

        StoreFlashError(flash_error, membase);
      }
      membase += EEPROM_PAGE_SIZE;
    }
    Serial.println(F(" Completed"));
  }
  else {
    Serial.println(F(" -> not virgin"));
  }
  // check de api update key
  String strd_DynDNS;
  byte lenstrd_DynDNS;
  Serial.print(F("Reading length from address: "));
  Serial.println(DYNDNS_API_KEY_addr);
  flash.readAnything(DYNDNS_API_KEY_addr, lenstrd_DynDNS);
  Serial.print(F("Length of stored dynDNS API Key: "));
  Serial.println(lenstrd_DynDNS);
  // check if we have a stored API key
  if ( lenstrd_DynDNS != 40) {
    Serial.print(F("Empty or corrupted key found, writing dynDNS API key..."));
    updateApiKey();
    Serial.println(F(" done"));
    String tmpstr;
    flash.readStr(DYNDNS_API_KEY_addr, tmpstr);
    Serial.print(F("New stored dynDNS API Key: "));
    Serial.println(tmpstr);
  }
  else {
    Serial.println(F("Fetching stored dynDNS API key..."));
    flash.readStr(DYNDNS_API_KEY_addr, strd_DynDNS);
    Serial.println(F("Checking stored dynDNS API key..."));
    for ( int i = 0; i < 39; i++) {
      if ( strd_DynDNS[i] != DYNDNS_API_KEY[i]) {
        // empty buffer
        while (Serial.available() > 0) {
          Serial.read();
        }
        Serial.println(F("\r\nERROR! -> DNS API update key stored in FLASH  is different from defined key. Please Check!"));
        Serial.println(F("Hit any key within 5 seconds to store defined default key in FLASH"));
        // read incoming serial data:
        Serial.setTimeout(1000);
        for (int j = 0; j < 5; j++ ) {
          delay(1000);
          if ( Serial.available() > 0 ) {
            updateApiKey();
            // empty buffer
            while (Serial.available() > 0) {
              Serial.read();
            }
            break;
          }
        }
        Serial.println(F("APIkey NOT updated for DYNDNS in FLASH"));
        break;
      }
    }
  }
#endif
  restore_counters(0x00);  // read th counters from the fans
  // start the Ethernet and UDP:
  // Sprinkle some magic pixie dust. (disable SD SPI on Arduino for the Ethernetshield)
  // pinMode(4, OUTPUT);
  // digitalWrite(4, HIGH);
  // print your local hardware address:
  Serial.println("init ethernet");
  // set the SS pin
  Ethernet.init(ETHERNET_SS);
  Serial.print(F("Ethernet using "));
  Serial.print(ETHERNET_SS);
  Serial.println(F(" for CS"));
  digitalWrite(LED_BUILTIN, HIGH);
#if defined (USE_DHCP) && defined (USE_TEST_LAN_IP)
  Serial.println ("Ethernet using DHCP in test LAN configured");
  if (Ethernet.begin(test_LAN_mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
#elif USE_DHCP
  Serial.println("Ethernet using DHCP in production LAN configured");
  if (Ethernet.begin(LAN_mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
#elif USE_TEST_LAN_IP
  if (digitalRead(keyswitch) != LOW) {
    Ethernet.begin(test_LAN_mac, test_LAN_ip, test_LAN_dnsserver, test_LAN_gateway, subnet);
    Serial.println(F("Ethernet using LAN test configuration"));
    lanmode = 1;
  }
  else {
    Ethernet.begin(LAN_mac, LAN_ip, LAN_dnsserver, LAN_gateway, subnet);
    Serial.println(F("Ethernet using LAN configuration"));
    lanmode = 0;
  }
#else
  if (digitalRead(keyswitch) == LOW) {
    Ethernet.begin(test_LAN_mac, test_LAN_ip, test_LAN_dnsserver, test_LAN_gateway, subnet);
    Serial.println(F("Ethernet using LAN test configuration"));
    lanmode = 1;
  }
  else {
    Ethernet.begin(LAN_mac, LAN_ip, LAN_dnsserver, LAN_gateway, subnet);
    Serial.println(F("Ethernet using LAN configuration"));
    lanmode = 0;
  }
#endif
  while (Serial.available() > 0) {
    Serial.read();  // clear serial input buffer
  }
  Serial.println(F("For switching between LAN test and LAN IP-configuration press KEY switch within 4 seconds and keep pressed till next message or press any key"));
  Serial.print(F("Status of keyswitch: "));
  Serial.println(digitalRead(keyswitch) ? "not pressed" : "pressed");
  for ( int lcip = 0; lcip < 20 ; lcip++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    if ((digitalRead(keyswitch) == LOW) || (Serial.available() > 0)) {
      if (Serial.available() > 0) {
        Serial.print(F("Key pressed -> "));
      }
      Serial.println(F("Network type switched"));
      switchEthNetw();
      break;
    }
  }
  while (Serial.available() > 0) {
    Serial.read();  // clear serial input buffer
  }
  Serial.print(F("Status of keyswitch: "));
  Serial.println(digitalRead(keyswitch) ? "not pressed" : "pressed");
  Serial.print(F("Using MAC: "));
  Ethernet.MACAddress(macBuffer); // fill the buffer
  sprintf(webData, "%02X-%02X-%02X-%02X-%02X-%02X", macBuffer[0], macBuffer[1], macBuffer[2], macBuffer[3], macBuffer[4], macBuffer[5]);  //webdata is max 96
  Serial.println(webData);

  Serial.println(F("Starting Ethernet"));
  // start the Ethernet connection:
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet shield was not found."));
    Serial.println(F("program execution terminated."));
    while (1) {};
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100) {
    Serial.println(F("W5100 Ethernet controller detected."));
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200) {
    Serial.println(F("W5200 Ethernet controller detected."));
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500) {
    Serial.println(F("W5500 Ethernet controller detected."));
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
#ifdef OPTION_FLASH
  udpdump = flash.readByte(udpdump_addr);
#endif
  if (udpdump) {
    //    IPAddress segm = Ethernet.localIP();
    // test_LAN_broadcast
    //SendTo[2] = segm[2];
    UDPSendTo = test_LAN_logger;
    Serial.print(F("Creating UDP socket Using UDP address: "));
    Serial.print(UDPSendTo);
    Serial.print(" listening port: ");
    Serial.print(localPort);
    Serial.print(" sending to port: ");
    Serial.println(Port);
    UDPclient.begin(localPort);
  }
  textStringLog("MAX_SOCK_NUM is defined to: ", false);
  char stvalbuff[16];
  itoa(MAX_SOCK_NUM, stvalbuff, 10);  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  textStringLog("IP addres: ", false);
  IPAddress segm = Ethernet.localIP();
  String scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(stvalbuff, scobuff.c_str());  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  textStringLog("Subnetmask: ", false);
  segm = Ethernet.subnetMask();
  scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(stvalbuff, scobuff.c_str());  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  textStringLog("Default gateway: ", false);
  segm = Ethernet.gatewayIP();
  scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(stvalbuff, scobuff.c_str());  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  textStringLog("DNS-server: ", false);
  segm = Ethernet.dnsServerIP();
  scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(stvalbuff, scobuff.c_str());  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  // set connect timeout parameters
  //Initialise the connected socket arrays
  //  G_SocketCheckTimer = millis();
  for (byte l_sock = 0; l_sock < MAX_SOCK_NUM; l_sock++) {
    G_SocketConnectionTimers[l_sock] = 0;
    //    G_SocketConnectionTimes[l_sock] = 0;
  }
  //W5100.setRetransmissionTime(2000); // 200ms per try
  //  Ethernet.setRetransmissionCount(8);

  //  set the time is mandatory
  bool hak = false;
  int dtm;
  char inpchr = '\0';
  textStringLog("Request for actual time...", false);
  while ((timeStatus() != timeSet) && (hak == false)) {
    UpdateTime();
    if (timeStatus() == timeSet) {
      break;
    }
    dtm = 0;
    inpchr = '\0';
    textStringLog("E is break, C is change IP address", false);
    digitalWrite(LED_BUILTIN, HIGH);
    while (dtm < 10) {
      if (Serial.available() > 0) {
        inpchr = Serial.read();
        bitClear(inpchr, 5);  // create upper char
        if (inpchr == 'E') {
          hak = true;
          while (Serial.available() > 0) {
            Serial.read();
          }
          digitalWrite(LED_BUILTIN, LOW);
          break;
        }
        if (inpchr == 'C') {
          while (Serial.available() > 0) {
            Serial.read();
          }
          switchEthNetw();
        }
      }
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      dtm++;
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
#ifdef OPTION_FLASH
  textStringLog("\r\nhandling the stored month on address: 0x", false);
  itoa(iMonth_addr, stvalbuff, 16);  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  strd_iMonth = flash.readByte(iMonth_addr);
#endif
  textStringLog("Actual stored month: ", false);
  itoa(strd_iMonth, stvalbuff, 10);  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  if (month() != strd_iMonth) {
    char c = 'N';
    textStringLog("Potential ERROR, stored month is not current month!", true);
    textStringLog("Stored month number: ", false);
    itoa(strd_iMonth, stvalbuff, 10);  // stvalbuff is max 16 char
    textlog(stvalbuff, true);
    if ((strd_iMonth > 12) || (strd_iMonth == 0)) {
      textStringLog("Modified to current month: ", false);
      strd_iMonth = month();
      itoa(strd_iMonth, stvalbuff, 10);  // stvalbuff is max 16 char
      textlog(stvalbuff, true);
      c = 'F';
    }
    if (c == 'N') {
      Serial.println(F("Do you want to correct?"));
      Serial.println("f = force to previous month ");
      Serial.println("y = change to this month");
      Serial.print("n or any other key to skip. (7 sec to default -> ");
      Serial.print(c);
      Serial.println(F("'.)"));
      int tl = 0;
      while (tl < 28) {
        if (Serial.available()) {
          c = Serial.read();  //gets one byte from serial buffer
          bitClear(c, 5);  // create upper char
          Serial.print (F("Readed char from input: "));
          Serial.println(c);
          break;
        }
        delay(250);
        tl++;
      }
    }
    if ((c == 'Y') || (c == 'F')) {
      if (c == 'F') {
        strd_iMonth = month() - 1; // force a previous month change
      }
      else {
        strd_iMonth = month(); // force the current month change
      }
      textStringLog("Due to a month change, force month change. Prior month set to: ", false);
      itoa(strd_iMonth, stvalbuff, 10);  // stvalbuff is max 16 char
      textlog(stvalbuff, true);
      // hier de nieuwe maand in spi memory plaatsen EEPROM
#ifdef OPTION_FLASH
      flash.eraseSector(iMonth_addr);
      if (!flash.writeByte(iMonth_addr, strd_iMonth)) {
        flash_error = F("writeByte iMonth_addr FAIL");
        StoreFlashError(flash_error, iMonth_addr);
      }
#endif
    }
    else {
      textStringLog("No alteration", true);
    }
  }
  while (Serial.available() > 0) {
    if (!min_serial) {
      Serial.print(F("emptying buffer: "));
      sprintf(ob, "%02x", Serial.read());  // ob is max 8 char
      Serial.print(ob);
      if ( Serial.available() == 0 ) {
        Serial.println();
      }
    }
  }
#ifdef DEBUG_WOLFSSL
  registerWolfSSLDebug();
#endif

  textStringLog("Sending DHCP parameters to: ", false);
  textStringLog(MAIL_TO, false);
  GetWANIPAddress(); //includes SendBootMail function
  server.begin();
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d HTTP listener started", hour(), minute(), second());
  textlog(temptxtbuff, false);
  if ((lastDayReset == 0) || (lastDayReset > 31)) {
    lastDayReset = day();
  }
  lastMinute = minute();
  lastHour = hour();
  sprintf(TimePeakPower, "%02d:%02d", 0, 0);  // max is 6 char
  sprintf(TimePeakPowerIn, "%02d:%02d", 0, 0);  // max is 6 char
  sprintf(TimePeakTemp, "%02d:%02d", 0, 0);  // max is 6 char
  // Mintemp and maxtemp must be set before restoreday in case of eeprom restore this values will be overwritten, preventing the overwrite in begin_temperature
  MinTemp = 255.0;
  MaxTemp = -255.0;
  textStringLog("requesting stored value for pluvio and weather", true);
  RestoreDayTotal(); // check if we haved saved status, in that case restore todays values
  textStringLog("current value of MinTemp: ", false);
  String fpstring = "";
  fpstring = String(MinTemp, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  textStringLog("current value of MaxTemp: ", false);
  fpstring = String(MaxTemp, 2); //necessary to convert the float to char array, %f does not function
  fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
  textlog(stvalbuff, true);
  textStringLog("requesting actual pluvio and weather", false);
  //update temperature
  br_rslt = false;
  min_serial = min_serial;
  hk = 0;
  if (MinTemp == 255.0) {
    force_temperature = true;
  }
  else {
    force_temperature = false;
  }
  BR_Loop(16);
  Begin_Temperature(); // reset the weathercounters, force off
  char pvicntbuf[32];
  do {
    tmpcntrespTimeout = cntrespTimeout;
    tmpChkSumErrCnt = ChkSumErrCnt;
    updatePVI();
    itoa(cntrespTimeout, pvicntbuf, 10);
    textStringLog("\r\ncntrespTimeout: ", false);
    textlog(pvicntbuf, true);
    itoa(tmpcntrespTimeout, pvicntbuf, 10);
    textStringLog("tmpcntrespTimeout: ", false);
    textlog(pvicntbuf, true);
    itoa(ChkSumErrCnt, pvicntbuf, 10);
    textStringLog("ChkSumErrCnt: ", false);
    textlog(pvicntbuf, true);
    itoa(tmpChkSumErrCnt, pvicntbuf, 10);
    textStringLog("tmpChkSumErrCnt: ", false);
    textlog(pvicntbuf, true);
    textStringLog("Status of (tmpcntrespTimeout != cntrespTimeout): ", false);
    (tmpcntrespTimeout != cntrespTimeout) ? textStringLog("true", true) : textStringLog("false", true);
    textStringLog("Status of (tmpChkSumErrCnt != ChkSumErrCnt): ", false);
    (tmpChkSumErrCnt != ChkSumErrCnt) ? textStringLog("true", true) : textStringLog("false", true);
    textStringLog("Status of (ChkSumErrCnt < 5): ", false);
    (ChkSumErrCnt < 5) ? textStringLog("true", true) : textStringLog("false", true);
    textStringLog("Status of ((tmpcntrespTimeout != cntrespTimeout) || (tmpChkSumErrCnt != ChkSumErrCnt)) && (ChkSumErrCnt < 5): ", false);
    ((tmpcntrespTimeout != cntrespTimeout) || (tmpChkSumErrCnt != ChkSumErrCnt)) && (ChkSumErrCnt < 5) ? textStringLog("true", false) : textStringLog("false", false);
  }
  while (((tmpcntrespTimeout != cntrespTimeout) || (tmpChkSumErrCnt != ChkSumErrCnt)) && (ChkSumErrCnt < 5)) ; // poll the pvi until we have valid values for all counters, mark the powersupply (many crc errors)
  if (actTension > 0)
  {
    receiveMaxRS485Wait = longRS485Wait;
    Serial2.setTimeout(receiveMaxRS485Wait);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d RS485 timeout period switched to long.", hour(), minute(), second()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  else {
    receiveMaxRS485Wait = shortRS485Wait;
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d RS485 timeout period switched to short.", hour(), minute(), second()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Actual grid input power: %d + %d = %dWatt", hour(), minute(), second(), Power1, Power2, Power1 + Power2); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d RS485 timeout: %dms", hour(), minute(), second(), receiveMaxRS485Wait); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  check_fan_hysteris();
  if (quickstart == false) {
    sprintf(looptime, "%02d:%02d:%02d", hour(), minute(), second()); // looptime is max 32 char
    sprintf(temptxtbuff, "\r\n%s Power on fan test", looptime); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    textStringLog("\r\nfan A off: ", false);
    float strd_tempFan;
    strd_tempFan = tempFanAOff;
    fpstring = String(strd_tempFan, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
    textlog(stvalbuff, false);
    textStringLog(" on: ", false);
    strd_tempFan = tempFanAOn;
    fpstring = String(strd_tempFan, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
    textlog(stvalbuff, false);
    textStringLog(" testing...", false);
    PowerupFanA();
    textStringLog("\r\nfan B off: ", false);
    strd_tempFan = tempFanBOff;
    fpstring = String(strd_tempFan, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
    textlog(stvalbuff, false);
    textStringLog(" on: ", false);
    strd_tempFan = tempFanBOn;
    fpstring = String(strd_tempFan, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
    textlog(stvalbuff, false);
    textStringLog(" testing...", false);
    delay(1000);
    digitalWrite(Relay_fanB, RELAY_ON);
    delay(1000);
    fanBOnCount++;
    textStringLog("\r\nfan C off: ", false);
    strd_tempFan = tempFanCOff;
    fpstring = String(strd_tempFan, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
    textlog(stvalbuff, false);
    textStringLog(" on: ", false);
    strd_tempFan = tempFanCOn;
    fpstring = String(strd_tempFan, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(stvalbuff, 6);  // stvalbuff is max 16 char
    textlog(stvalbuff, false);
    textStringLog(" testing...", false);
    delay(1000);
    digitalWrite(Relay_fanC, RELAY_ON);
    delay(1000);
    fanCOnCount++;
    textStringLog("\r\nFans will be switched off by normal programm execution", true);
  }
#if defined (USE_LOGGING) || defined (USE_PWRLOGGING)
  textStringLog("trying writing SD", true);
  SetupSD();
  writePowerLogFile();
#endif
  if ((lastHour >= 13) && (weatherCondition1300.length() == 0)) {
    weatherCondition1300 = String(weatherType);
    textStringLog("\r\nweatherCondition1300 forced at boot", false);
  }
  textStringLog("\r\nChecking DNS....", true);
  CheckIpPv();
  sketchStartTime = now();
  // Check power value, if non zero, check if necessary and reset the already uploaded varable
#ifdef OPTION_FLASH
  AlreadyAdded = flash.readByte(AlreadyAdded_addr);
#endif
  if ((Power1 + Power2 > 0 ) && (AlreadyAdded == true))
  {
    textStringLog("AlreadyAdded FORCED to False", true);
#ifdef OPTION_FLASH
    flash.eraseSector(AlreadyAdded_addr);
    if (!flash.writeByte(AlreadyAdded_addr, false)) {
      flash_error = F("writeByte AlreadyAdded_addr FAIL");
      StoreFlashError(flash_error, AlreadyAdded_addr);
    }
    delay(3);
    AlreadyAdded = flash.readByte(AlreadyAdded_addr);
    //store the flag if already daily mail send
    flash.eraseSector(AlreadyDailySend_addr);
    if (!flash.writeByte(AlreadyDailySend_addr, false)) {
      flash_error = F("writeByte AlreadyDailySend_addr FAIL");
      StoreFlashError(flash_error, AlreadyDailySend_addr);
    }
    delay(3);
    //store the flag if already output for this month is send
    flash.eraseSector(AlreadyMonthSend_addr);
    if (!flash.writeByte(AlreadyMonthSend_addr, false)) {
      flash_error = F("writeByte AlreadyMonthSend_addr FAIL");
      StoreFlashError(flash_error, AlreadyMonthSend_addr);
    }
    delay(3);
    //store the flag if already daily mail send
    AlreadyDailySend = flash.readByte(AlreadyDailySend_addr);
    //store the flag if already output for this month is send
    AlreadyMonthSend = flash.readByte(AlreadyMonthSend_addr);
#endif
  }
  textStringLog("\r\nPVOutput AlreadyAdded status: ", false);
  if (AlreadyAdded) {
    textStringLog("True", false);
  }
  else {
    textStringLog("False", false);
  }
  totalnets = countIPtableEntries();
  totalHTTPcmd = countHTTPtableEntries();
  textStringLog("\r\nMail send Daily output status: ", false);
  if (AlreadyDailySend) {
    textStringLog("Done for today.", true);
  }
  else {
    textStringLog("Not send.", true);
  }
  todayCnt++;  // erase kind of bootflag
  todayCnt_br++;
  //  StoreDayYield(day(), month());


  //==================================================================================================
  // update with current metrics of the day due to lack of eeprom
  //==================================================================================================
  //===============================================================================
  // update with current metrics of the day due to lack of eeprom
  //===============================================================================
  /*  fanAOnTotalCount = 10669;
    fanBOnTotalCount = 10982;
    fanCOnTotalCount = 7407;
    runTimefanA = (unsigned long)11264 * 60;
    runTimefanA += 14;
    runTimefanB = (unsigned long)8169 * 60;
    runTimefanB += 36;
    runTimefanC = (unsigned long)7911 * 60;
    runTimefanC += 57;
    //===============================================================================
    Serial.println(F("=+=+=+=RestoreDayTotal from program memory due to absent SPI bus memory=+=+=+="));
    // add the historic yield from the last days
    MinTemp = 7.60;
    sprintf(TimeMinTemp, "07:46");

    MaxTemp = 11.50;
    sprintf(TimeMaxTemp, "16:56");
    luchtvochtigheid = 78;
    totPluvio_br = 10.77;
    weatherCondition1300 = "Geheel bewolkt";
    invRiso = 20.82;
    shadow = 0;
    MaxcmltvPwr = 26126;
    AlreadyAdded = true;
    AlreadyDailySend = true;
    AlreadyMonthSend = true;


    uint32_t strAddr = 0x00027000;
    uint32_t _d = 2325;
    while (!flash.eraseSector(strAddr));
    //  maxtotalToday = 2325;
    byte dy = 1;
    byte monthIndex = 1;
    dayYield[dy - 1 + (monthIndex * 31)] = 2325;
    //  StoreDayYield(1, 1);
    //  delay(50);
    Serial.println("writing from dayyield");
    _d = dayYield[dy - 1 + (monthIndex * 31)];
    flash.writeULong(strAddr, _d);
    Serial.print("read from address 0x27000: ");
    _d = flash.readULong(strAddr);
    Serial.println(_d);
    strAddr = 0x00046000;
    while (!flash.eraseSector(strAddr));
    _d = 47114711;
    Serial.println("Writing 2325 to 0x46000");
    flash.writeULong(strAddr, _d);
    _d = flash.readULong(strAddr);
    Serial.print(F("Read unit32: "));
    Serial.println(_d);
    Serial.print(F("From address: 0x"));
    Serial.println(strAddr, 16);
    maxtotalToday = 4712;
    StoreDayYield(1, 2);


    maxtotalToday = 473;
    StoreDayYield(1, 2);
    maxtotalToday = 7926;
    StoreDayYield(2, 2);
    maxtotalToday = 568;
    StoreDayYield(3, 2);
    //    lines below could be deleted after first imaging only for 3.1 R12
    if (firstShadowTime[1] == 255) {
    sprintf(firstShadowTime, "not today");
    sprintf(lastShadowTime, "not today");
    }

    // test for reporting to AbuseIPDB
    abuslogging = true;
    wolfssllog = true;
    textlog("\r\nPreparing suspicious array", false);
    String ncavlbl;

    suspicious_ips[suspicious_index][0] = 185;  //185.156.72.48  106.75.73.226 167.94.138.46
    suspicious_ips[suspicious_index][1] = 156;
    suspicious_ips[suspicious_index][2] = 72;
    suspicious_ips[suspicious_index][3] = 48;
    ncavlbl = "no characters available, wanted connection? no 185";
    suspicious_length[suspicious_index] = ncavlbl.length();
    ncavlbl.toCharArray(suspicious_string[suspicious_index], suspicious_length[suspicious_index] + 1);
    sprintf(temptxtbuff, "\r\nstore in suspicious array %s, length is: %d", suspicious_string[suspicious_index], suspicious_length[suspicious_index]); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
    suspicious_perc[suspicious_index] = 0;
    suspicious_time[suspicious_index] = now();
    suspicious_index++;

    suspicious_ips[suspicious_index][0] = 106;  //185.156.72.48  106.75.73.226 167.94.138.46
    suspicious_ips[suspicious_index][1] = 75;
    suspicious_ips[suspicious_index][2] = 73;
    suspicious_ips[suspicious_index][3] = 226;
    ncavlbl = "characters available, wanted connection de eerste van de 106 die moet het langste z";
    suspicious_length[suspicious_index] = ncavlbl.length();
    ncavlbl.toCharArray(suspicious_string[suspicious_index], suspicious_length[suspicious_index] + 1);
    sprintf(temptxtbuff, "\r\nstore in suspicious array %s, length is: %d", suspicious_string[suspicious_index], suspicious_length[suspicious_index]); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
    suspicious_perc[suspicious_index] = 0;
    suspicious_time[suspicious_index] = now();
    suspicious_index++;

    suspicious_ips[suspicious_index][0] = 106;  //185.156.72.48  106.75.73.226 167.94.138.46
    suspicious_ips[suspicious_index][1] = 75;
    suspicious_ips[suspicious_index][2] = 73;
    suspicious_ips[suspicious_index][3] = 226;
    ncavlbl = "characters available, wanted connection van de 106 die is het kortste zijn, maar nu oet di!";
    suspicious_length[suspicious_index] = ncavlbl.length();
    ncavlbl.toCharArray(suspicious_string[suspicious_index], suspicious_length[suspicious_index] + 1);
    sprintf(temptxtbuff, "\r\nstore in suspicious array %s, length is: %d", suspicious_string[suspicious_index], suspicious_length[suspicious_index]); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
    suspicious_perc[suspicious_index] = 0;
    suspicious_time[suspicious_index] = now();
    suspicious_index++;
    suspicious_ips[suspicious_index][0] = 106;  //185.156.72.48  106.75.73.226 167.94.138.46
    suspicious_ips[suspicious_index][1] = 75;
    suspicious_ips[suspicious_index][2] = 73;
    suspicious_ips[suspicious_index][3] = 226;
    ncavlbl = "characters available, wanted connection van de 106 die is het kortste zijn, maa!";
    suspicious_length[suspicious_index] = ncavlbl.length();
    ncavlbl.toCharArray(suspicious_string[suspicious_index], suspicious_length[suspicious_index] + 1);
    sprintf(temptxtbuff, "\r\nstore in suspicious array %s, length is: %d", suspicious_string[suspicious_index], suspicious_length[suspicious_index]); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
    suspicious_perc[suspicious_index] = 0;
    suspicious_time[suspicious_index] = now();
    suspicious_index++;
    int i;
    for (i=0; i<23; i++) {
    suspicious_ips[suspicious_index][0] = 167;  //185.156.72.48  106.75.73.226 167.94.138.46
    suspicious_ips[suspicious_index][1] = 94;
    suspicious_ips[suspicious_index][2] = 138;
    suspicious_ips[suspicious_index][3] = i;
    ncavlbl = "characters available, wanted connection van de 167 die is het kortste zijn,  !";
    suspicious_length[suspicious_index] = ncavlbl.length();
    ncavlbl.toCharArray(suspicious_string[suspicious_index], suspicious_length[suspicious_index] + 1);
    sprintf(temptxtbuff, "\r\nstore in suspicious array %s, length is: %d", suspicious_string[suspicious_index], suspicious_length[suspicious_index]); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
    suspicious_perc[suspicious_index] = 0;
    suspicious_time[suspicious_index] = now();
    suspicious_index++;
    }
    suspicious_ips[suspicious_index][0] = 167;  //185.156.72.48  106.75.73.226 167.94.138.46
    suspicious_ips[suspicious_index][1] = 94;
    suspicious_ips[suspicious_index][2] = 138;
    suspicious_ips[suspicious_index][3] = i-1;
    ncavlbl = "characters available, wanted connection van de 167 die is het kortste zijn,  Ijnhkgfkhgfkhgfkghfkghfkgkgf!";
    suspicious_length[suspicious_index] = ncavlbl.length();
    ncavlbl.toCharArray(suspicious_string[suspicious_index], suspicious_length[suspicious_index] + 1);
    sprintf(temptxtbuff, "\r\nstore in suspicious array %s, length is: %d", suspicious_string[suspicious_index], suspicious_length[suspicious_index]); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
    suspicious_perc[suspicious_index] = 0;
    suspicious_time[suspicious_index] = now();
    suspicious_index++;

    suspicious_ips[suspicious_index][0] = 196;  //185.156.72.48  106.75.73.226 167.94.138.46
    suspicious_ips[suspicious_index][1] = 75;
    suspicious_ips[suspicious_index][2] = 73;
    suspicious_ips[suspicious_index][3] = 226;
    ncavlbl = "no characters, unwanted connection196?";
    suspicious_length[suspicious_index] = ncavlbl.length();
    ncavlbl.toCharArray(suspicious_string[0], suspicious_length[suspicious_index] + 1);
    sprintf(temptxtbuff, "\r\nstore in suspicious array %s, length is: %d", suspicious_string[suspicious_index], suspicious_length[suspicious_index]); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
    suspicious_perc[suspicious_index] = 0;
    suspicious_time[suspicious_index] = now();
    suspicious_index++;
    checkforIPabuse();
    //last_suspicious_index = 3;
    //checkforIPabuse();
  */
  randomSeed(analogRead(0));
  //  show_Flash_Row2Serial(eepromEnd_addr);
  //  convertIPtable();
  //  show_Flash_Row2Serial(eepromEnd_addr+EEPROM_PAGE_SIZE);
  textlog("\r\nSetup complete, LED meaning: low intensity is idle in loop, LED slow flashing during IP switching, LED on during function execution", true);
  //  attachInterrupt(digitalPinToInterrupt(keyswitch), switchEthNetw, FALLING);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  //  char logMessage[] = "=============test fprintf to stderr=================";
  //  printf("%s\n", logMessage);
}

//-----------------------------------------------------------------------
// Main Loop
//-----------------------------------------------------------------------

void loop() {
  // get the actual time
  byte i;
  iMonth = month();
  iDay = day();
  iHour = hour();
  iMinute = minute();
  iSecond = second();

  // reset counters when todays day is different from the last day the counters were reset
  if (iDay != lastDayReset)
  {
    // erase the tftp buffer if it contains data
    if (flash.readByte(eepromEnd_addr) != 0xFF) {
      if (!flash.eraseSector(eepromEnd_addr)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Flash erase error... for address 0x08X = eepromEnd_addr", hour(), minute(), second(), eepromEnd_addr); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        flash_error = F("Erase eepromEnd_addr FAIL");
        StoreFlashError(flash_error, eepromEnd_addr);
      }
    }
    // if the day changed, maybe the month also changed
    // if the month has changed, reset the yield from 3 month ago
#ifdef OPTION_FLASH
    strd_iMonth = flash.readByte(iMonth_addr);
#endif
    lastMonth = strd_iMonth;
    // check a logical month change
    if ( ( ( (lastMonth + 1) == iMonth) && (timeStatus() == timeSet)) || ( (lastMonth != iMonth) && (lastMonth == 12) && (iMonth == 1) && (timeStatus() == timeSet) ) )
    {
      textlog(DateTime(now()), false);
      textStringLog(" Month change detected. Month readed from FLASH: ", false);
      char lpcobuff[16];
      itoa(lastMonth, lpcobuff, 10); // lpcobuff is max 16 char
      textlog(lpcobuff, false);
      textStringLog(" (strd_iMonth). Writing current month from NTP to FLASH: ", false);
      itoa(iMonth, lpcobuff, 10); // lpcobuff is max 16 char
      textlog(lpcobuff, true);
#ifdef OPTION_FLASH
      flash.eraseSector(iMonth_addr);
      if (!flash.writeByte(iMonth_addr, iMonth)) {
        flash_error = F("writeByte iMonth_addr FAIL");
        StoreFlashError(flash_error, AlreadyMonthSend_addr);
      }
      delay(3);
#endif
      strd_iMonth = iMonth; //iMonth_addr
      // write the current month also in the index
      byte monthIndex = (iMonth - 1) % 3;
      textStringLog("New  month index: ", false);
      itoa(monthIndex, lpcobuff, 10); // lpcobuff is max 16 char
      textlog(lpcobuff, true);
      int idy = 0;
      textStringLog("wiping 31 positions in RAM array for index: ", false);
      itoa(monthIndex, lpcobuff, 10);
      textlog(lpcobuff, false);
      textStringLog(" of 3 indices", true);
      while (idy < 31) {
        dayYield[idy + (monthIndex * 31)] = 0;
        idy++;
      }
      /*
        #ifdef OPTION_FLASH
            Serial.print(F("Store new month to monthIndexBase_addr in FLASH: "));
            flash.eraseSector(monthIndexBase_addr);
            flash.writeByte(monthIndexBase_addr, iMonth);
            delay(3);
            Serial.println(monthIndexBase_addr);
            //erase the content from 3 month ago
      */
#ifdef OPTION_FLASH
      unsigned int wipeaddress = monthBase_addr + ((monthIndex * 31) * EEPROM_PAGE_SIZE);
      textStringLog("Wipe 31 entries on from baseaddress: 0x", false);
      itoa(wipeaddress, lpcobuff, 16); // lpcobuff is max 16 char
      textlog(lpcobuff, true);
      for (int i = 0; i < 31; i++) {
        flash.eraseSector(wipeaddress); //EEPROM.put(monthBase_addr + (((i - 1) * 2) + (monthIndex * 62)), 0); // first quartermonth=monthBase_addr-3861, 2nd 3862-3923, 3rd 3924-3985
        wipeaddress += EEPROM_PAGE_SIZE;
      }
#endif
      if (CheckSocketConnections() >= 1) {
        int retr = 0;
        bool resp = false;
        do {
          resp = updateDNS();
          retr++;
        } while ((resp == false) && (retr < 3));
      }
      else {
        nosockavail++;
      }
    }
#ifdef USE_LOGGING
    // create new logfile
    CloseLogFile();
    OpenLogFile();
#endif
    lastDayReset = iDay;
    sprintf(temptxtbuff, "\r\n%s Counters reset", DateTime(now()));
    textlog(temptxtbuff, false);
    AlreadyAdded = false;
    AlreadyDailySend = false;
    AlreadyMonthSend = false;
    exc_plv_err = false;
    mailSendErrorflag = false;
    mailRcvErrorflag = false;
    errorwolfsslwantread = 0;
    errorwolfsslwantwrite = 0;
#ifdef OPTION_FLASH
    if (flash.readByte(saveFlag_addr) != 0xFF) {
      if (!flash.eraseSector(saveFlag_addr)) {
        flash_error = F("saveFlag_addr");
        StoreFlashError(flash_error, saveFlag_addr); // whipe flag for saving this day
      }
    }
    AlreadyAdded = flash.readByte(AlreadyAdded_addr);
    if (AlreadyAdded != false) {
      AlreadyAdded = false;
      if (!min_serial) {
        sprintf(temptxtbuff, "\r\n%s AlreadyAdded has been reset", DateTime(now()));
        textlog(temptxtbuff, false);
      }
      flash.eraseSector(AlreadyAdded_addr);
      if (!flash.writeByte(AlreadyAdded_addr, false)) {
        flash_error = F("writeByte AlreadyAdded_addr FAIL");
        StoreFlashError(flash_error, AlreadyAdded_addr);
      }
      delay(3);
      flash.eraseSector(AlreadyDailySend_addr);
      if (!flash.writeByte(AlreadyDailySend_addr, false)) {
        flash_error = F("writeByte AlreadyDailySend_addr FAIL");
        StoreFlashError(flash_error, AlreadyDailySend_addr);
      }
      delay(3);
      flash.eraseSector(AlreadyMonthSend_addr);
      if (!flash.writeByte(AlreadyMonthSend_addr, false)) {
        flash_error = F("writeByte AlreadyMonthSend_addr FAIL");
        StoreFlashError(flash_error, AlreadyMonthSend_addr);
      }
      delay(3);
    }
#endif
    ChkSumErrCnt = 0;
    //    uplPeakPowerObserved=0;
    PeakPowerObserved = 0;  // used to determine the moment of peakpower this day
    pwrPeak = 0;
    prevPwrPeak = 0;
    PeakPowerInObserved = 0;
    startInvTemp = 0.0;
    sprintf(TimestartInvTemp, "%02d:%02d", hour(), minute()); // now we live at 00:00, max is 6 char
    sprintf(TimePeakPower, "%02d:%02d", hour(), minute()); // now we live at 00:00,   // max is 6 char
    sprintf(TimePeakPowerIn, "%02d:%02d", hour(), minute()); // now we live at 00:00,  // max is 6 char
    sprintf(TimePeakTemp, "%02d:%02d", hour(), minute()); // now we live at 00:00,  // max is 6 char
    Power1 = 0;
    Power2 = 0;
    PrevGridPower = 0;
    MaxcmltvPwr = 0;
    totalToday = 0;
    maxtotalToday = 0;
    uploadErr = 0;
    uploadsOk = 0;
    invRiso = 0;
    PVIPolled = 0;
    weatherCondition1300 = "";
    wCLength = 0; // erase the weatherCondition1300 for restore, no mail send after restore
    totPluvio_br = 0.00;
    MinTemp = 255.0;
    MaxTemp = -255.0;
    Begin_Temperature();
    receiveMaxRS485Wait = shortRS485Wait; // Be sure not to send updates to pvoutput during the night
    byte strd_wCLength;
    float strd_last_neerslagintensiteit;
    time_t strd_lastpluvioTime_br;
#ifdef OPTION_FLASH
    strd_wCLength = flash.readByte(wCLength_addr);
    if (strd_wCLength != wCLength) {
      flash.eraseSector(wCLength_addr);
      if (!flash.writeByte(wCLength_addr, strd_wCLength)) {
        flash_error = F("writeByte wCLength_addr FAIL");
        StoreFlashError(flash_error, wCLength_addr);
      }
      delay(3);
    }
    strd_last_neerslagintensiteit = flash.readFloat(last_neerslagintensiteit_addr);
    if (strd_last_neerslagintensiteit != 0.0) {
      flash.eraseSector(last_neerslagintensiteit_addr);
      if (!flash.writeFloat(last_neerslagintensiteit_addr, 0.0)) {
        flash_error = F("writeFloat last_neerslagintensiteit_addr FAIL");
        StoreFlashError(flash_error, last_neerslagintensiteit_addr);
      }
      delay(3);
    }
    flash.readAnything(lastpluvioTime_br_addr, strd_lastpluvioTime_br);
    if (strd_lastpluvioTime_br != 0) {
      strd_lastpluvioTime_br = 0;
      flash.eraseSector(lastpluvioTime_br_addr);
      if (!flash.writeAnything(lastpluvioTime_br_addr, strd_lastpluvioTime_br)) {
        flash_error = F("writeAnything lastpluvioTime_br_addr FAIL");
        StoreFlashError(flash_error, lastpluvioTime_br_addr);
      }
      delay(3);
    }
    float strdpluviointens;
    strdpluviointens = flash.readFloat(max_neerslagintensiteit_addr);
    if (strdpluviointens != 0.0) {
      flash.eraseSector(max_neerslagintensiteit_addr);
      if (!flash.writeFloat(max_neerslagintensiteit_addr, 0.00)) {
        flash_error = F("writeFloat max_neerslagintensiteit_addr FAIL");
        StoreFlashError(flash_error, max_neerslagintensiteit_addr);
      }
      delay(3);
    }
    time_t strdmaxPluvTime;
    flash.readAnything(maxpluvioTime_br_addr, strdmaxPluvTime);
    if (strdmaxPluvTime != 0) {
      strdmaxPluvTime = 0;
      flash.eraseSector(maxpluvioTime_br_addr);
      if (!flash.writeAnything(maxpluvioTime_br_addr, strdmaxPluvTime)) {
        flash_error = F("writeAnything maxpluvioTime_br_addr FAIL");
        StoreFlashError(flash_error, maxpluvioTime_br_addr);
      }
      delay(3);
    }
#endif
    //reset error counters
    /*
        crcErrGridVoltage = 0;
        crcErrGridPower = 0;
        crcErrgetTempBst = 0;
        crcErrgetTempInv = 0;
        crcErrgetDc1Voltage = 0;
        crcErrgetDc1Current = 0;
        crcErrgetDc2Voltage = 0;
        crcErrgetDc2Current = 0;
        crcErrgetRiso = 0;
        crcErrgetTime = 0;
        crcErrgetEnergyDay = 0;
        crcErrgetEnergyTotal = 0;
        crcErrgetGridPowerPeakDay = 0;
    */
    rxTimeflag = false;
    //    rxReadCnt = 0;
    mailerrortype[0] = '\0';
    todayIPCnt = 0;
    maxInvTemp = 0.0;
    runTimeThisDayfanA = 0;
    runTimeThisDayfanB = 0;
    runTimeThisDayfanC = 0;
    fanAOnCount = 0;
    fanBOnCount = 0;
    fanCOnCount = 0;
    shadow = 0;
    sprintf(firstShadowTime, "not today");
    sprintf(lastShadowTime, "not today");
    /*
      #if defined(ARCH_STM32) || defined(ARDUINO_ARCH_ESP32) || defined(STM32F4xx)
        // email the suspicious HTML commands from yesterday
        if (suspicious_index != 0 ) {
          last_suspicious_index = 0;
          sprintf(mailsubj, "Today suspicious HTML commands received");
          if (!sendEmail(11)) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Error: Sending suspicious string email.", hour(), minute(), second()); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
        }
        if (unknownCmd_index != 0) {
          unknownCmd_sema = false;
          sprintf(mailsubj, "Today unknown HTML commands received");
          if (!sendEmail(16)) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Error: Sending unknown command email.", hour(), minute(), second()); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
        }
      #endif
    */
  }
  if (iMinute != lastMinute) {
    // handle the weather first
    if (((iMinute % 5) == 4) || ((iMinute % 10) == 6)) {
      //      if ( min_serial) {  // in ieder geval niet bij !min_serial
      //        textStringLog("", true);
      //      }
      if (SerialXltStat() == 0) {
        DisconnectSocketConnections(); // check if all sockets are occupied
      }
      else {
        CheckSocketConnections();
      }
    }
    if (brlogging) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Status of br_rslt: %s", hour(), minute(), second(), br_rslt ? "True -> " : "False -> "); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    BR_Loop(iMinute);
    if (iMinute % 20 == 18) {
      checkforIPabuse();
    }
  }
  // second has changed
  if (lastSecond != iSecond) {
    lastSecond = iSecond;
    if ((actTension > 0) || (GridPower > 0)) {
      if (lastSecond % 10 == 5) {
        updatePVI(); // if alive poll every 10 seconds
        // check for temperature
        bool fanStarted = false;
        float strdFanOn;
        sprintf(looptime, "%02d:%02d:%02d", hour(), minute(), second()); // looptime is max 32 char
        strdFanOn = tempFanAOn;
        if (((digitalRead(Relay_Power) == RELAY_OFF) && (fanStarted == false)) && ((actInvTemp > strdFanOn) || (GridPower > fanApowerOn))) {
          PowerupFanA();
#ifdef OPTION_FLASH
          fanAOnTotalCount = flash.readULong(fanAOnTotalCount_addr);
#endif
          fanAOnTotalCount++;
#ifdef OPTION_FLASH
          flash.eraseSector(fanAOnTotalCount_addr);
          if (!flash.writeULong(fanAOnTotalCount_addr, fanAOnTotalCount)) {
            flash_error = F("writeULong fanAOnTotalCount_addr FAIL");
            StoreFlashError(flash_error, fanAOnTotalCount_addr);
          }
          delay(3);
#endif
          fanStarted = true;
          // not needed anymore because only 1 fan is started in a 10 sec interval          delay(500); // give power supply time to recover
        }
        strdFanOn = tempFanBOn;
        if (((digitalRead(Relay_fanB) == RELAY_OFF) && (fanStarted == false)) && ((actInvTemp > strdFanOn) || (GridPower > fanBpowerOn)))
        {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Fan B: starting", hour(), minute(), second());  // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          delay(250);
          digitalWrite(Relay_fanB, RELAY_ON);// set the Relay ON
          delay(1000);
          runTimeThisCyclefanB = 0;
          fanBOnCount++;
#ifdef OPTION_FLASH
          fanBOnTotalCount = flash.readULong(fanBOnTotalCount_addr);
#endif
          fanBOnTotalCount++;
#ifdef OPTION_FLASH
          flash.eraseSector(fanBOnTotalCount_addr);
          if (!flash.writeULong(fanBOnTotalCount_addr, fanBOnTotalCount)) {
            flash_error = F("writeULong fanBOnTotalCount_addr FAIL");
            StoreFlashError(flash_error, fanBOnTotalCount_addr);
          }
          delay(3);
#endif
          fanStarted = true;
          // delay(500); // give power supply time to recover
        }
        strdFanOn = tempFanCOn;
        if (((digitalRead(Relay_fanC) == RELAY_OFF) && (fanStarted == false)) && ((actInvTemp > strdFanOn) || (GridPower > fanCpowerOn)))
        {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Fan C: starting", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          delay(250);
          digitalWrite(Relay_fanC, RELAY_ON);// set the Relay ON
          delay(1000);
          runTimeThisCyclefanC = 0;
          fanCOnCount++;
#ifdef OPTION_FLASH
          fanCOnTotalCount = flash.readULong(fanCOnTotalCount_addr);
#endif
          fanCOnTotalCount++;
#ifdef OPTION_FLASH
          flash.eraseSector(fanCOnTotalCount_addr);
          if (!flash.writeULong(fanCOnTotalCount_addr, fanCOnTotalCount)) {
            flash_error = F("writeULong fanCOnTotalCount_addr FAIL");
            StoreFlashError(flash_error, fanCOnTotalCount_addr);
          }
          delay(3);
#endif
          fanStarted = true;
        }
        if (fanStarted) {
          if (!min_serial && (upTime != 0)) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Polling PVI: ", hour(), minute(), second());  // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          delay(500); // give power supply time to recover
        }
      }
    }
    else {
      if (lastSecond == 30) {
        updatePVI(); // if we have last time no tension then poll every minute once instead of every 10 sec
      }
    }
    //check for serial characters on console
    if ( (Serial.available() > 0) && (!min_serial || showbuffer || brlogging || wolfssllog) ) {
      textStringLog("\r\n", false);
      SerInpRcdv = true;
    }
    while (Serial.available() > 0) {
      if (Serial.available() < 4) {
        char c;
        c = Serial.read();
        textStringLog("\r\nSerial received: ", false);
        if (((byte)c > 0x1F) &&  ((byte)c < 0x7F)) {
          char co[2];
          co[0] = c;
          co[1] = '\0';
          textlog(co, false);
          if ((c >= '-') &&  (c <= '@')) {
            SerInterpret(c);
          }
        }
        else {
          textStringLog(" 0x", false);
          //        Serial.println((byte) c, 16);
          sprintf(ob, "%02x\r\n", (byte) c); // ob is max 8 char
          textlog(ob, false);
        }
      }
      else {
        String SerBufferString = Serial.readString();
        if (showbufferremaining) {
          textStringLog("\r\nSerial received ignored: ", false);
          textStringLog(SerBufferString, true);
        }
      }
    }
    if (tftpReqEna) {
      tick++;
    }
  }
  // hour has changed
  // cannot simply check the change of an hour because 'updatetime' can also change the hour
  // therefore we also check that the minutes are 0
  if (iHour != lastHour && iMinute == 0) {
    //        busy(2);
    lastHour = iHour;
    //Save the humidity in the morning
    if (lastHour == 7)
    {
      luchtvochtigheid = lv_phi;
    }
    // memorize the weatherconditions at 13:00 hours
    if (lastHour == 13)
    {
      weatherCondition1300 = weatherType;
      storeWeatherCondition1300();
    }

    //update with the historical barometric information
    if (lastHour % 3 == 0)
    {
      // // to determine hPa changes, round float to int
      int templ = int(luchtdruk);
      if (int((luchtdruk - templ) * 10) >= 5) ++templ;
      (templ == luchtDrukMin3) ? hPaTrend = '-' : (templ > luchtDrukMin3) ? hPaTrend = '/' : hPaTrend = '\\'; //hPaTrend is max 1 char
      luchtDrukMin3 = templ;
    }
    if (CheckSocketConnections() >= 1) {
      GetWANIPAddress(); // check if the IP address is changed, if yes send mail
    }
    else {
      nosockavail++;
    }
  }

  // update every minute
  if (iMinute != lastMinute)
  {
    switch (Ethernet.maintain()) {
      case 1:
        //renewed fail
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: DHCP Error: renewed fail", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        break;
      case 2:
        //renewed success
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: DHCP Renewed IP success, my IP address: %d.%d.%d.%d.", hour(), minute(), second(), Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);    // temptxtbuff is max 1024 char
        textlog(temptxtbuff, false);
        break;
      case 3:
        //rebind fail
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: DHCP Error: rebind fail.", hour(), minute(), second());    // temptxtbuff is max 1024 char
        textlog(temptxtbuff, false);
        break;
      case 4:
        //rebind success
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: DHCP Rebind success, my IP address: %d.%d.%d.%d.", hour(), minute(), second(), Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);    // temptxtbuff is max 1024 char
        textlog(temptxtbuff, false);
        break;
      default:
        //nothing happened
        break;
    }
    upTime++;
    // update weather if needed, we do it only on +9,+19,+29,+39,+49,+59
    int shPwr1 = Power1 / 8;
    shPwr1 = (shPwr1 * 6) + 60; // correct the integer division
    if ( (Power2 > shPwr1) && (GridPower > 100) && (iHour > 12)) {
      if (lastMinute == lastiMinuteShdw) {
        if (shadow == 0) {
          sprintf(firstShadowTime, "%02d:%02d", lastiHourShdw, lastiMinuteShdw); // firstShadowTime is max 9 char
        }
        shadow++; // if lower string is in shade ( and not at startup)
        sprintf(lastShadowTime, "%02d:%02d", hour(), minute());  // lastShadowTime is max 9 char
      }
      lastiMinuteShdw = iMinute;
      lastiHourShdw = iHour;
    }
    lastMinute = iMinute;
    if (shadow == 120) {
      //like there is an error in 1 of the strings
      if (!sendEmail(6)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending solar panel shade string error email.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
    // moved from interval
    // update with the comments of the day if the PVI is stopping at the end of the day,
    // send a last packet with zero power to now when the inverter switched off
    // if ((iHour > 15) && ((Power1 + Power2) == 0) && (PrevGridPower == 0) && !AlreadyAdded)
    if ((iHour > 15) && (actTension == 0.0) && (PrevGridPower == 0) && !AlreadyAdded) {
      getGridVoltage();  // write the switching moment in the log
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Store day yield", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      StoreDayYield(day(), month());
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Checking Socket Connections ", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      delay(50);
      if (CheckSocketConnections() >= 1) {
        textStringLog(" -> Check Socket Connections completed", false);
        delay(50);
        SerialXltStat();
        AddToPvOutput();
#if defined (USE_LOGGING) || defined (USE_PWRLOGGING)
        SetupSD();
        writePowerLogFile();
#endif
        receiveMaxRS485Wait = shortRS485Wait; // short time out if we are asleep
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d RS485 timeout: %dms", hour(), minute(), second(), receiveMaxRS485Wait); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        Serial2.setTimeout(receiveMaxRS485Wait);
        //        AlreadyAdded = true;
        peakpower5min = 0;
        Power1 = 0;
        Power2 = 0;
      }
      else {
        nosockavail++;
      }
      if (AlreadyAdded && !AlreadyDailySend) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Sending mail with daily yield", hour(), minute(), second());
        textlog(temptxtbuff, false);
        if (sendEmail(2)) {
          AlreadyDailySend = true;
        }
        else {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail with daily yield", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      if (AlreadyAdded && !AlreadyMonthSend) {
        //        Serial.println(F("Sending mail with monthly power yield"));
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Sending mail with monthly power yield", hour(), minute(), second());
        textlog(temptxtbuff, false);
        if (sendEmail(3)) {
          AlreadyMonthSend = true;
        }
        else {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail with monthly power yield", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
    }
    // end insert frominterval
    // reset the rxtime values if we wake up
    if ((actTension > 0) && (rxTimeflag == false))
    {
      receiveMaxRS485Wait = longRS485Wait; // set the long time out if we are awake
      //      Serial.print(F("\r\nRS485 timeout: "));
      //      Serial.println(receiveMaxRS485Wait);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d RS485 timeout: %dms", hour(), minute(), second(), receiveMaxRS485Wait); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      Serial2.setTimeout(receiveMaxRS485Wait);
      minRxTime = receiveMaxRS485Wait;  // if it shorter then it is updated
      maxRxTime = 0;
      cntrespTimeout = 0;
      if (CheckSocketConnections() >= 1) {
        UpdateTime();
        if (!min_serial && (lastSecond > 5)) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Polling PVI: ", hour(), minute(), second());  // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        nosockavail++;
      }
      resetRxTime = now();
      rxTimeflag = true;
    }
    if ((todayErrCnt >= 30) && (exc_plv_err == false)) {
      //excessive errors for requesting pluvio, maybe there is a problem with the certificate
      //send an email
      if (sendEmail(10) != 1) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Error: Sending excessive pluvio readings mail.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      else {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Excessive pluvio readings mail send success", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        exc_plv_err = true;
      }
    }
    // update every 5 minutes to pvoutput.org or whatever is set in userdefs
    if ((lastMinute % UPDATEINTERVAL) == 0)
    {
#ifdef USE_LOGGING
      WriteDateToLog();
      //      for(byte i = 0; i < NUMSENSORS; i++)
      //      {
      //        sensors[i]->Status(&logFile);
      Status(&logFile);
      //        logFile << sensors[i]->Today << ";" << sensors[i]->Actual << ";" << endl;
      logFile << T1Today << ";" << Actual << F(";\r\n");
      //      }
      logFile << endl;
      logFile.flush();
#endif
      // update if power is > 0
      if (((PrevGridPower > 0) || (GridPower > 0) || ((Power1 + Power2) > 1)) && !AlreadyAdded) {
        if (CheckSocketConnections() >= 1 ) {
          SerialXltStat();
          SendToPvOutput();
        }
        else {
          nosockavail++;
        }
        peakpower5min = GridPower; // set the max output
        PrevGridPower = GridPower;
      }
    }
    // sync the time at fixed interval
    if (((lastHour == 23) && (iMinute == 57)) || ((lastHour == 7) && (iMinute == 2)) || ((lastHour == 10) && (iMinute == 2)) || ((lastHour == 13)  && (iMinute == 2)) || (timeStatus() != timeSet))
    {
      if (CheckSocketConnections() >= 1) {
        SerialXltStat();
        UpdateTime();
        if (lastHour == 10) {
          getTime();  // check the timeskew from the inverter and the realtime
        }
        if (!min_serial && (lastSecond > 5)) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Polling PVI: ", hour(), minute(), second());  // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        nosockavail++;
      }
    }
    // update on the last minute of the day the weather conditions  by an additional addoutput
    if ((lastHour == 23) && (iMinute == 59)) {
      if (DisconnectSocketConnections() >= 1) { // check if all sockets are occupied
        SerialXltStat();
        AddToPvOutput();
      }
      else {
        nosockavail++;
      }
    }
    // increment the fan runtime counters
    if (digitalRead(Relay_fanB) == RELAY_ON)
    {
      runTimefanA++;
      runTimeThisDayfanA++;
      runTimeThisCyclefanA++;
    }
    if (digitalRead(Relay_fanB) == RELAY_ON)
    {
      runTimefanB++;
      runTimeThisDayfanB++;
      runTimeThisCyclefanB++;
    }
    if (digitalRead(Relay_fanC) == RELAY_ON)
    {
      runTimefanC++;
      runTimeThisDayfanC++;
      runTimeThisCyclefanC++;
    }
    //flag for just powerdown one fan at a time and to reset the serial line
    bool shutdownFan = false;
    float strdfanOff ;
    sprintf(looptime, "%02d:%02d:%02d", hour(), minute(), second());  // looptime is max 32 char
    strdfanOff = tempFanCOff;
    if ((digitalRead(Relay_fanC) == RELAY_ON) && ((actInvTemp < strdfanOff) && (GridPower < fanCpowerOff)))
    {
      sprintf(temptxtbuff, "\r\n%s Fan C: off", looptime); // temptxtbuff is max 1024
      //      if (min_serial) {
      textlog(temptxtbuff, false);
      //      } else {
      //        textlog(temptxtbuff, true);
      //      }
      delay(1250); // give power supply time to recover
      digitalWrite(Relay_fanC, RELAY_OFF);// set the Relay OFF
      // store  runTimefanC);
      shutdownFan = true;
    }
    strdfanOff = tempFanBOff;
    if ((digitalRead(Relay_fanB) == RELAY_ON) && ((actInvTemp < strdfanOff) && (GridPower < fanBpowerOff)) && (shutdownFan == false))
    {
      // stop the second and third fan
      sprintf(temptxtbuff, "\r\n%s Fan B: off", looptime); // temptxtbuff is max 1024
      //      if (min_serial) {
      textlog(temptxtbuff, false);
      //      } else {
      //        textlog(temptxtbuff, true);
      //      }
      delay(1250); // give power supply time to recover
      digitalWrite(Relay_fanB, RELAY_OFF);// set the Relay OFF
      // store fan runtimeEEPROM.put(runTimefanB_addr, runTimefanB);
      shutdownFan = true;
    }
    strdfanOff = tempFanAOff;
    if ((digitalRead(Relay_Power) == RELAY_ON) && ((actInvTemp < strdfanOff) && (GridPower < fanApowerOff)) && (shutdownFan == false))
    {
      // stop the first fan
      sprintf(temptxtbuff, "\r\n%s Start procedure to powerdown fan A -> snubber capacitor enabled ", looptime); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      //      if (Serial) Serial.flush();
      delay(40); // give serial port time to flush
      digitalWrite(Relay_C_DIST, RELAY_ON);// set the Relay ON
      delay(1000);
      sprintf(temptxtbuff, "-> switch power supply off "); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      //      if (Serial) Serial.flush();
      delay(50);
      digitalWrite(Relay_Power, RELAY_OFF);// set the Relay OFF
      delay(1000);
      digitalWrite(Relay_C_DIST, RELAY_OFF);// set the Relay OFF
      delay(1000);
      digitalWrite(Relay_fanA, RELAY_OFF);
      delay(200);
      sprintf(temptxtbuff, "-> snubber capacitor decoupled -> fan A off"); // temptxtbuff is max 1024
      //      if (!min_serial) {
      textlog(temptxtbuff, false);
      //      } else {
      //        textlog(temptxtbuff, true);
      //      }
      shutdownFan = true;
    }
    if (shutdownFan) {
      delay(500); // give power supply time to recover
    }
  }
  // see if there are clients to serve
  ServeWebClients();
  //    busy(0);
  // give the ethernet shield some time to rest
  digitalWrite(LED_BUILTIN, HIGH);
  if (tftpReqEna) {
    if (tftpPoll() == 0) {
      tftpReqEna = false;
    }
  }
  else {
    delay(50);
  }
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(keyswitch, INPUT_PULLUP);
  int keysense = digitalRead(keyswitch);
  if (keysense == LOW) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    textStringLog("\r\nmainloop Keyswitch is LOW", true);
    switchEthNetw();
    while (digitalRead(keyswitch) == LOW) {}
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
