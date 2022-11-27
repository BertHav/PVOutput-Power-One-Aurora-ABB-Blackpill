/*

char* is a mutable pointer to a mutable character/string.

const char* is a mutable pointer to an immutable character/string. You cannot change the contents of the location(s) this pointer points to. 
Also, compilers are required to give error messages when you try to do so. For the same reason, conversion from const char * to char* is deprecated.

char* const is an immutable pointer (it cannot point to any other location) but the contents of location at which it points are mutable.

const char* const is an immutable pointer to an immutable character/string.

You missed out one more: char const *
char * give segmentation fault while running? 
So use const if I want the compiler to give error if I forgot and changed the data by mistake
It depends where char * is created. For example: char *s = "A string" puts "A string" in to the code section (RO memory) of your binary. 
Writing to this memory seg faults. But char *s = malloc(sizeof(char) * 10) allocates memory on the heap, and this memory section is writeable and hence doesn't seg fault on write. 
kan weg
 */


#ifndef userdefs
#define userdefs

//*****************************************************************
// If you want the logging data to be written to the SD card, remove // from the next line:
// also remove the // from the aurora_pvoutput.ino line "//#include <SD.h>"
//#define USE_LOGGING
//#define USE_PWRLOGGING
//*****************************************************************
// Mail variables. Uncomment the next line and a mail will be sent once a day or at events
#define USE_MAIL
// Change to your base64 encoded user
#define MAIL_USER "base64username"
// change to your base64 encoded password
#define MAIL_AUTH "base64password"
#define MAIL_TO "<email@address.com>" // fill in the destination mail address
#define MAIL_TTO "recipientname"
#define MAIL_FROM "<senderemailaddress>" // any valid mail address will do here
#define MAIL_SERVER "mail.smtp2go.com" // use the server address of your own provider
#define MAIL_PORT 2525
#define MAIL_FFROM "PowerOne "

// AbuseIPDB
#define ABUSEIPDB_KEY "insert-here-your-unique-id-from-abuseipdb.com"

// weerlive
#define WEERLIVE_KEY "xxxxxxxxxx"  // 10 Alphanumeric chars
#define GPS_LOC "DD.dddd,D.dddd"   // notation latitude, longitude DD.dddd, D.dddd 

// Free DNS variables
#define DYNDNS_SERVER "freedns.afraid.org" // use the server address
String DYNDNS_API_KEY = "your-api-key-from-freedns.afraid.org"; // 39 chars
//*****************************************************************
// Network variables
static byte test_LAN_mac[]        = { 0x08, 0x00, 0x2B, 0x03, 0xFE, 0xED }; //My DEC history ;)
static byte test_LAN_ip[]         = { 192, 168, 16, 56 };                    // IP of the Blackill 192.168.16.56
static byte test_LAN_dnsserver[]  = { 8, 8, 8, 8 };                         // use the address of your gateway { 192, 168, 1, 1 } if your router supports this
static byte test_LAN_gateway[]    = { 192, 168, 16, 1 };
static byte test_LAN_broadcast[]  = { 192, 168, 16, 255 };
static byte test_LAN_logger[]     = { 192, 168, 16, 128 };

static byte LAN_mac[]       = { 0x08, 0x00, 0x2B, 0x02, 0xFE, 0xED }; //My DEC history ;)
static byte LAN_ip[]        = { 192, 168, 16, 85 };                   // IP of Blackpill 192.168.16.85
static byte LAN_dnsserver[] = { 8, 8, 8, 8 };                        // use the address of your gateway { 192, 168, 1, 1 } if your router supports this
static byte LAN_gateway[]   = { 192, 168, 16, 1 };
static byte subnet[]         = { 255, 255, 255, 0 };
//UDP part for remote logging
unsigned int Port = 5555;
unsigned int localPort = 5556;

#define logMsgLen 150
char logMessage[logMsgLen];

#define bufferMax 256    // the length of the http commandbuffer 
#define DISCONNECT_TIMER 120
//*****************************************************************
#define FAVICONSIZE 318

//*****************************************************************
// You can find your api-key in the PvOutput settings page, under api-settings
#define PVOUTPUT_API_KEY "fill-in-here"

//*****************************************************************
// The update interval must match what you have set in the PvOutput settings
// PvOutput->Settings->System->Live settings->Status interval
// Default is 5 minutes
#define UPDATEINTERVAL 5

//*****************************************************************
#define NTP_SERVER "nl.pool.ntp.org"                             // If you are having problems with the time synchonisation, try a different NTP server

//*****************************************************************
// The actual time can be shifted to move the time of uploading to pvoutput and exosite
// This is to prevent missing uploads because everyone is uploading at exactly the same time
// Offset is in seconds, positive numbers will upload earlier
// A negative number will delay the upload by the amount of seconds set here
#define TIME_OFFSET 16

//*****************************************************************
// Sensor configuration
//*****************************************************************
char nrwthsttn[] = "specify-your-city";  // to receive weatherupdates
int SID = SID from pvoutput;

#define logMsgFieldDefs		"F01:"		// field definitions of log file

#define logMsgStart		"D01:Start"
#define logMsgData		"D02:Data"

#define logMsgShowData		"I01:ShowData"
#define logMsgTimeSync		"I02:TimeSync"
#define logMsgNoPVI		"I03:NoPVIfound"
#define logMsgGotoSleep	"I04:GotoSleep"
#define logMsgAwaked		"I05:Awaked"

#define logMsgNoSDcard		"I06:NoSDfound"
#define logMsgSDcard		"I07:SDfound"

#define logMsgNoIntConn	"I08:NoIntConn"
#define logMsgIntConn		"I09:IntConn"

#define logMsgFlushRecBuf	"E01:FlushRB x="
#define logMsgRecChkSumErr	"E02:RecChkSumErr"
#define logMsgRecTimeout	"E03:RecTimeout"

#define EEPROM_MAX_ADDR 16777216  // check your flashsize, remark the Blackpill has no flash mount on the bottom by standard

#define EEPROM_PAGE_SIZE 4096
//*****************************************************************
// flash memory mapping for restore
#define prevActual_addr 0
#define ethrlog_addr                  prevActual_addr+EEPROM_PAGE_SIZE
#define TimestartInvTemp_addr         ethrlog_addr+EEPROM_PAGE_SIZE
#define startInvTemp_addr             TimestartInvTemp_addr+EEPROM_PAGE_SIZE
#define last_neerslagintensiteit_addr startInvTemp_addr+EEPROM_PAGE_SIZE
#define lastpluvioTime_br_addr        last_neerslagintensiteit_addr+EEPROM_PAGE_SIZE
#define max_neerslagintensiteit_addr  lastpluvioTime_br_addr+EEPROM_PAGE_SIZE
#define maxpluvioTime_br_addr         max_neerslagintensiteit_addr+EEPROM_PAGE_SIZE
#define showbuffer_addr               maxpluvioTime_br_addr+EEPROM_PAGE_SIZE  // pluvio logging
#define min_serial_addr               showbuffer_addr+EEPROM_PAGE_SIZE  // log messages for USB Serial port
#define DYNDNS_API_KEY_addr           min_serial_addr+EEPROM_PAGE_SIZE // key = 39 +1 for termination
#define eepromOffset_addr             DYNDNS_API_KEY_addr+EEPROM_PAGE_SIZE  // address in Eeprom where we start to dump the content on request
#define totPluvio_htn_addr            eepromOffset_addr+EEPROM_PAGE_SIZE // tempory used for total pluvio buienradarapp 
#define runTimefanA_addr              totPluvio_htn_addr+EEPROM_PAGE_SIZE
#define runTimefanB_addr              runTimefanA_addr+EEPROM_PAGE_SIZE
#define runTimefanC_addr              runTimefanB_addr+EEPROM_PAGE_SIZE
#define shadow_addr                   runTimefanC_addr+EEPROM_PAGE_SIZE
#define fanAOnCount_addr              shadow_addr+EEPROM_PAGE_SIZE
#define fanBOnCount_addr              fanAOnCount_addr+EEPROM_PAGE_SIZE
#define fanCOnCount_addr              fanBOnCount_addr+EEPROM_PAGE_SIZE
#define runTimeThisDayfanA_addr       fanCOnCount_addr+EEPROM_PAGE_SIZE
#define runTimeThisDayfanB_addr       runTimeThisDayfanA_addr+EEPROM_PAGE_SIZE
#define runTimeThisDayfanC_addr       runTimeThisDayfanB_addr+EEPROM_PAGE_SIZE
#define TimeMinTemp_addr              runTimeThisDayfanC_addr+EEPROM_PAGE_SIZE
#define TimeMaxTemp_addr              TimeMinTemp_addr+EEPROM_PAGE_SIZE
#define TimePeakTemp_addr             TimeMaxTemp_addr+EEPROM_PAGE_SIZE
#define tempFanAOff_addr              TimePeakTemp_addr+EEPROM_PAGE_SIZE
#define tempFanBOff_addr              tempFanAOff_addr+EEPROM_PAGE_SIZE
#define tempFanCOff_addr              tempFanBOff_addr+EEPROM_PAGE_SIZE
#define tempFanAOn_addr               tempFanCOff_addr+EEPROM_PAGE_SIZE
#define tempFanBOn_addr               tempFanAOn_addr+EEPROM_PAGE_SIZE
#define tempFanCOn_addr               tempFanBOn_addr+EEPROM_PAGE_SIZE
#define saveFlag_addr                 tempFanCOn_addr+EEPROM_PAGE_SIZE
#define maxInvTemp_addr               saveFlag_addr+EEPROM_PAGE_SIZE
#define TimePeakPowerIn_addr          maxInvTemp_addr+EEPROM_PAGE_SIZE
#define PeakPowerInObserved_addr      TimePeakPowerIn_addr+EEPROM_PAGE_SIZE
#define fanAOnTotalCount_addr         PeakPowerInObserved_addr+EEPROM_PAGE_SIZE
#define fanBOnTotalCount_addr         fanAOnTotalCount_addr+EEPROM_PAGE_SIZE
#define fanCOnTotalCount_addr         fanBOnTotalCount_addr+EEPROM_PAGE_SIZE
#define monthBase_addr                fanCOnTotalCount_addr+EEPROM_PAGE_SIZE
// 1st month: 9984-10239, 2nd: 10240-10495, 3rd: 10496-10751
#define iMonth_addr                   monthBase_addr+(93*EEPROM_PAGE_SIZE)
#define monthIndexBase_addr           iMonth_addr+EEPROM_PAGE_SIZE // not used
//reserved for last months
#define maxtotalToday_addr            monthIndexBase_addr+EEPROM_PAGE_SIZE
#define PeakPowerObserved_addr        maxtotalToday_addr+EEPROM_PAGE_SIZE
#define TimePeakPower_addr            PeakPowerObserved_addr+EEPROM_PAGE_SIZE
#define MaxcmltvPwr_addr              TimePeakPower_addr+EEPROM_PAGE_SIZE
#define invRiso_addr                  MaxcmltvPwr_addr+EEPROM_PAGE_SIZE
#define MaxTemp_addr                  invRiso_addr+EEPROM_PAGE_SIZE
#define MinTemp_addr                  MaxTemp_addr+EEPROM_PAGE_SIZE
#define luchtvochtigheid_addr         MinTemp_addr+EEPROM_PAGE_SIZE
#define AlreadyAdded_addr             luchtvochtigheid_addr+EEPROM_PAGE_SIZE
#define totPluvio_br_addr             AlreadyAdded_addr+EEPROM_PAGE_SIZE
#define wCLength_addr                 totPluvio_br_addr+EEPROM_PAGE_SIZE
#define weatherCondition1300_addr     wCLength_addr+EEPROM_PAGE_SIZE
#define AlreadyDailySend_addr         weatherCondition1300_addr+EEPROM_PAGE_SIZE
#define AlreadyMonthSend_addr         AlreadyDailySend_addr+EEPROM_PAGE_SIZE
#define iplog_addr                    AlreadyMonthSend_addr+EEPROM_PAGE_SIZE
#define maillogging_addr              iplog_addr+EEPROM_PAGE_SIZE
#define pvoutputlog_addr              maillogging_addr+EEPROM_PAGE_SIZE
#define showbufferremaining_addr      pvoutputlog_addr+EEPROM_PAGE_SIZE
#define udpdump_addr                  showbufferremaining_addr+EEPROM_PAGE_SIZE
#define HTTPlog_addr                  udpdump_addr+EEPROM_PAGE_SIZE
#define firstShadowTime_addr          HTTPlog_addr+EEPROM_PAGE_SIZE
#define lastShadowTime_addr           firstShadowTime_addr+EEPROM_PAGE_SIZE
#define pluvio_ena_addr               lastShadowTime_addr+EEPROM_PAGE_SIZE
#define invlogging_addr               pluvio_ena_addr+EEPROM_PAGE_SIZE
#define brlogging_addr                invlogging_addr+EEPROM_PAGE_SIZE
#define wolfSSLdbg_addr               brlogging_addr+EEPROM_PAGE_SIZE
#define wolfSSLMail_addr              wolfSSLdbg_addr+EEPROM_PAGE_SIZE
#define abuslogging_addr              wolfSSLMail_addr+EEPROM_PAGE_SIZE
#define suspicious_mail_addr          abuslogging_addr+EEPROM_PAGE_SIZE
#define unknownCmd_mail_addr          suspicious_mail_addr+EEPROM_PAGE_SIZE
#define BrCorrFact_addr               unknownCmd_mail_addr+EEPROM_PAGE_SIZE
#define abusefilter1_addr             BrCorrFact_addr+EEPROM_PAGE_SIZE
#define abusefilter2_addr             abusefilter1_addr+EEPROM_PAGE_SIZE
#define abusefilter3_addr             abusefilter2_addr+EEPROM_PAGE_SIZE
#define abusefilter4_addr             abusefilter3_addr+EEPROM_PAGE_SIZE
#define abusefilter5_addr             abusefilter4_addr+EEPROM_PAGE_SIZE
#define abusefilter6_addr             abusefilter5_addr+EEPROM_PAGE_SIZE
#define abusefilter7_addr             abusefilter6_addr+EEPROM_PAGE_SIZE
#define abusefilter8_addr             abusefilter7_addr+EEPROM_PAGE_SIZE
#define abusefilter9_addr             abusefilter8_addr+EEPROM_PAGE_SIZE
#define abusefilter10_addr            abusefilter9_addr+EEPROM_PAGE_SIZE
#define abusefilter11_addr            abusefilter10_addr+EEPROM_PAGE_SIZE
#define abusefilter12_addr            abusefilter11_addr+EEPROM_PAGE_SIZE
#define HTTPblacklist1_addr           abusefilter12_addr+EEPROM_PAGE_SIZE
#define HTTPblacklist2_addr           HTTPblacklist1_addr+EEPROM_PAGE_SIZE
#define InExlog_addr                  HTTPblacklist2_addr+EEPROM_PAGE_SIZE
#define eepromEnd_addr                InExlog_addr+EEPROM_PAGE_SIZE

#define CRLF "\r\n"
//uint8_t FAVICON[FAVICONSIZE] PROGMEM
#define WEBDUINO_FAVICON_DATA {0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x28, 0x01, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, \
    0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, \
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, \
    0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, \
    0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x80, 0x80, \
    0x80, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, \
    0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, \
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x08, 0x66, 0x66, 0x80, \
    0x00, 0x00, 0x00, 0x08, 0x66, 0x66, 0x66, 0x66, 0x80, 0x00, 0x00, 0x66, \
    0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x08, 0x66, 0x66, 0x66, 0x66, 0x66, \
    0x66, 0x80, 0x06, 0x7F, 0xFF, 0x76, 0x67, 0xFF, 0xF7, 0x60, 0x87, 0xF6, \
    0x66, 0xF7, 0x7F, 0x66, 0x6F, 0x78, 0x6F, 0x66, 0x66, 0x6F, 0xF6, 0x6F, \
    0x66, 0xF6, 0x6F, 0x6F, 0xFF, 0x6F, 0xF6, 0xFF, 0xF6, 0xF6, 0x6F, 0x66, \
    0x66, 0x6F, 0xF6, 0x6F, 0x66, 0xF6, 0x67, 0xF6, 0x66, 0xF7, 0x7F, 0x66, \
    0x6F, 0x76, 0x86, 0x7F, 0xFF, 0x76, 0x67, 0xFF, 0xF7, 0x68, 0x06, 0x66, \
    0x66, 0x66, 0x66, 0x66, 0x66, 0x60, 0x08, 0x66, 0x66, 0x66, 0x66, 0x66, \
    0x66, 0x80, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x08, \
    0x66, 0x66, 0x66, 0x66, 0x80, 0x00, 0x00, 0x00, 0x08, 0x66, 0x66, 0x80, \
    0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xC0, 0x03, \
    0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, \
    0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x07, \
    0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00}

// icon definitions
//uint8_t NACHTMIST[NMISTSIZE] PROGMEM
#define WEBDUINO_BEWOLKT_DATA      "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nWtJREFUeNrsVsFxwjAQFJn8QzpwCaSCmAogFWC//E0qCOlAPP3CVECogHQQSnA6cAnZY9aZi5CNGYsP45250WCdtHcr3QljBgy4NYwuXZDneYRhDnuGjfn5ANtlWf\r\nYVnBiEQvIOe21xE+I3BHAIQkzSPWzCTxXsE/YDe6ACkZqbniMfnSETopiyxpyysA9sXDn+Qr6m/DL3BJ+yM7GSNFFnWENktC3BTqiMrCvgm3Yi9khaX5yjhNho2uF\r\nYlgxc8OgqU+Pe+a1JJbNVm1wNsIp4i0A22KNozBgOCc9IkPqcL6iAb49qqb5wd2pyUZdEH1JiJReQ5WUYxJZHeSJ1rBb1gg5cKRmxByzdjI2qw2BgENZR1Ut8Dew4\r\nRj7i0o3qmtDEG44JG0FILJzk/hFbdb57kMchGHm5Eie5k86lW97fc9eDV/f4kv27aurVx5rTFyEAJIEX3QXbXid5bWY9A6j4B6EwAwbcPH4FGAAyCH649/5tRwAAA\r\nABJRU5ErkJggg=="
#define WEBDUINO_BLIKSEM_DATA      "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nntJREFUeNrEVjFym1AQRRodgMplAmU6+QZwgUQqUgcqlTYnUHQCKaUqK3UKiC9gbiClSmnGpRtzA/ut5v2ZNQL0IWjyZ3a+GPT37b59ux/H+U9r1OfQdrv1sHnmeb\r\nFY5BcFBmCE7QY2rXmdwVYI4jAYMABdbCkssPi7gH8/96exJeiDAi1gMcwHgAQewjawku+XOLPulTEOCsgXUqrruQNY3FL3VJUhbKv9qObwXQOlGRzNLdh5hMme4//\r\nhWapxSCLdK1Chbic1Eyekt3UBSM4kfAyYSHPGjHSvKLUSSEvWL0oPOf0VdRlHCjTuC6qyNkLz6PuRrXgC/I271GU3wGCS2s5ZKrPuKNrjmnA3Svw5xDjkEBHLAPaD\r\n7ehSuH5dHxdDz2QGYQTnUcQnwO6F7oSsijFWreNwaDgXyLps6mMTUWSo0OvTr6c1LIW5PVtMK7rUwCv1Iq2AzrDdwmYNt5JNXy+NhsztNSYVhZpM1azWSngnV97r/\r\nVUAe4G9KksriZgZkZyMTPbvNc1kqwfL6u/XD2VNUsuaYP9UqeVgyqzuYwAHjDhntMffCCBhthF7811Ljj4/+73vY9ZWgH0AzUn5lPUWUFeVQa/YpvaTBlBPiazE84Hi0kK8raFYaF0iKKG/RObzrhmvK81/\r\noxxv4NhTSq0OoIDM/O5ENbKb6uxAc0EnknVMgUUtPuV9iGx3nanWHwIIxNyt1wzC4TdWrtrFUH4MDqCHzuKC8wNvEF+1yrtawrHULyetGjS0AW1tJ1K+Nx95CCiuDA79fSW0JhKQ7URro3qmKE8aBHgEBWDcdZROzlxlH0VYDRMrYj2H+GKxX6B6+i/\r\nn3wQYABjOAFXzWGn3AAAAAElFTkSuQmCC"
#define WEBDUINO_MIST_DATA         "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nXJJREFUeNrsVMFNw0AQtC0K8Iv30QAkHdgNQDog6SCuAFJBTAXQQSwaIB3Y0AB583IHMCtNpMW5i43NJUhkpdWdfN6bm93ZDYKT/XX7fD438FHf+KgnaIylFMd+ej\r\nBgmDCNuTeHBB5skSOVI3iyJ67uWJJ7+MqmhcgmGtbvBftH24Xh9UeFJYcXXG2gEnsHn9C/2ZmDTc0aTnGBAM0s4Nk+phKrPlU78a5UC2MloDFZ6nNDka3hG5xv1Pl\r\nKsZzh7KkTcAO8QmDKb3LZ0qFkeUAmD2TshLGFtVw/6F0BnHf41cqwFzBA52Sq2T1QC8L+Fq67IAX4ehAwVf7exogq3gpKan4xdIDoVli40kjlb+tpqIdBwDdqn7f8\r\nu2iM1W6pLl/fDJVsPE1K0UQ6vrqsmoyNR9CAcyGxigusl21pGmAyZDIwroNjWahq6y3FYLjT0yFBSzWXfZikN7e1U+w5s7Er1Ulj5P22oIqjCupk/8u+BBgAZuCJE\r\ny3A8qUAAAAASUVORK5CYII="
#define WEBDUINO_BUIEN_DATA        "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nkpJREFUeNrUVjFOw0AQdCweYHgASko68wKSkgqnoMapUpK8APKChDIVoaaIqSgTXoCpKDE8AOUHMIvmYDk4+ZI4Qpy0Otm+29mdm91zEPzRqK26cTweN9Vj3u12FxsDBlgd0xksgUXW5xx2gQAmlQIDNMV06bE0g3\r\nXKGAhXABWHA9g+nEvg2wLEjAOyMVspY1IqDo74qqnobAOwcOyT4FI+DrDu3BsYm3s8R/sMF8yyKGFnyqBlfcNF+Za1aYipp16JUJ6VcguPk+kr8SX04c4YoLJo6kOphybuMcXKl9CeucR1xlnAWquCKh9mSABTnv9\r\n3YIopVqJYrNmYhO6WsKbUnlI/X1SzC32UAEuksgHfEX1LYgv43/au43UG2evwMTKtNnREWDV47upcWgxp1cC/JRMyIgGeG3VjYexysnf9ksDeYNMlsFNb8aGlxICFP7OuPQMq34ZLZpuoUp2bMq2V3EAF7QYbRgA+\r\nV05aj8e7cwdYzHW6TL+13NASwYT1Z868zgvCZHmgrr4cgTQVGylsBou5PlGgP5pSrYSimGB3o53DiQpkpGqzz378xGMa9F5vM2YsQA+//Rx4NQtmJkAF6G3guadYaPP6NAJqYE1pu/VtIENLYJ9iAUimQEc+oD+uR\r\nUe2qe7jpDOyKiFT34NKgNVfyJwACQUoKq0jMGHjCplOlikzH6r7zKTD+1rK7QRAOY+gqYKr/vdWCcwEE/nU9LriClRjEWVLOZ2a810W1PeMA9VKA3XFXbDG+xu9V0F1nV3pf493AQYA+zTwWDEbt4UAAAAASUVORK5CYII="
#define WEBDUINO_HAGEL_DATA        "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAABuUlEQVRYR+2VjS0FURCFv9eBDlABKkAFqAAVoAJUgApQASpABagAFVCCfMkdudb+3H0r2UR2kpfdt\r\n3tn5syZObMzRrbZyPmZAEwMTAz8GwY2gE/gue9eGcLAKnAMbGdJBXEOXCRAnXjmBbAHXKboL8AtICB/i4mJzRIQpQAWgK2UwHur1tfrQ1am72RgNz0XRKuVALDaM8\r\nDgub0BJvBaNWdhJb3PAf462AXACm+S13Wi2j4Lyir3gasaAOGnj2cbrQuA1dnTpkRNgZeAV+ARUCFzAdDxviRITXTb9ZGG0DY1yrONgZj0U+Cka5hSe2yHitDC35Y\r\ntNymiDUD0sQ2AlZog2KqeVREHwE4G7EctbQCij86BFVQtKlxLCrFdVQAy57I6SvLsrQIltJ4m3UHMLYKH1usAdMqxSwVSbBCVIBMCUlpecwC24SkxIO2xomXxrrKu\r\ni1sQB/Pt5rN3wMA5AAG5hgXp7MSa9pvgOQHWWhcDuZNAIkkksloH0P/OhEC891nrBozAfQC0KTF0XyrZ71hDAOQqcE6CnUa66yoYAiDm4LDks/sXM1CwDPsfGcJA/\r\n2w1HhOA0Rn4AgJJYSGD5KhYAAAAAElFTkSuQmCC"
#define WEBDUINO_HALFBEWOLKT_DATA  "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nh9JREFUeNrsljFPwkAUx1tiNLpYFxOcymLiJCSOmtDJTWF1gS4yih/ACJ8AHJmAzUmKX4CuJibU1YXOLmI0ccT/M+/IebZSKG59yQte7+797r3371VNSyyxiDa53y6QR12/skR2Dz6GO1EWp5acuBF1YWrOcmbhDf\r\noNmKZs/YA9ZfhQbcO8GdPmKnwQAM/BLbXv+GnDaa0Zp8dN+CkHoqCeBMjy3/TM1U9eqAIl3tfBuCkH0hdQr8FZO5KozICy23QAmgPUU+PMBLdaLQEqKQD3aLOf3dt4NLi3dJA3ZV0R0ECV6zOg2ZCMprazOtLSa37\r\nm4OzWl6pCfS3TgQDORAYDaDKsx68IBb2sVCqOUoULnncwV1RaMuIYFuAuxvkpFGNdAdIpr5UMqT8WAo9DKjJguI01nYCshdIH0lYrJQVps/RNFofLbgdByfCcDnXDw5IyrR6+LrmnM7TGmRKgjoDNqCrnsr8KwcG7\r\n5+krAg35WQal/XWx6LxxxOUqij7OY4gxkjPcXR9qeeNOvL922JVZYKi3CJRLTsrdIgHS+Pkzpz19HHpiHAYWJ+3H+TqQDrhF37CH92NKJg+RTeh+/++vkybpQxZXFfCaCvZCVBnb+NYSPd7/Aea+kppNiKQaF4YY4\r\nvPnM7zDyi4GlVqIoIGNDb65FoGW+S4g60qZ+6FXpvQuL8NcVNL6sw3KifN8/xYWBFJmXUBryb+/iX0JMAAK4NUta3kU5gAAAABJRU5ErkJggg=="
#define WEBDUINO_HELDERENACHT_DATA "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nYRJREFUeNrsltFxgzAMhmkuAzBCJmjIBukETScobBBG6ASwAe4ETTZIJ4jbBUI38AiR78Sdq8i2XChP0Z3PARt90Y+QlWV3m8keUjafv74LmF5h2HlLlk8wXjbrRy\r\nPxtUwANgzMtS3uqSQ+FwLo3k4R6GC5VL1lBNrBVAp9WYnfpOBFAFp6oMZz7wnerx6VXABdwXRhlloYP/guXbNJdUhJVF/EDXOvAuc1zGuazalQFozR7shtBc4V/i7\r\nI2vEv3zEX8Y55f7VzTcF6KvAzuT5Ii8JYcExK81/gPALSEYWySSoXY5/kuoSEzKcAm4gCilnvpgAHpYRE6/Ek+vUlYHkdBT4yTmnUFaOMlfyCpTa9ZHrKpS0gFVPL\r\nQ1Fq58/VtI7fRIxSKiaakuxTkbN3aBbs+JBmNXe8dQBvXNkRvoHRR5RdiVufgJQGFbG5oG1Vg71D91F43LV4wMh6rsRGwGc3+REtIPhAPQLaclBxlyls9lzr8fw+z\r\ndHeahzvIeDdZrerAAMAf5OLNNSo58oAAAAASUVORK5CYII="
#define WEBDUINO_REGEN_DATA        "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nkBJREFUeNrUVjtuwkAQBcQBrBwgMmU6c4JAmQ6K1MQVJXAC4ARASQXUKXCX0uQEcZfWygGCb5C8SZ6jYYNhMUZRVhqtLc/um8+bGZdKf7TKeQ7N53MHm5e+d7vdzU\r\nWBAdjA1oO09nwOIDNbI8ongC6wPViojgE+OqZUyQEaQ3xIDQBieA0ygCT8PoT+JJfHOCj56zCPOp9LAQFgkpH3UOk2D4W9vOfwENLfoxvhoroF6V4gLmQD/aZtqEM\r\nFKl5NJWcU/1j4GIkxXxswxD3qMZRG9PZgSC1LbWuwXQgX/QI2lAMotc9pDrhvS27o5ePepRlqXZeDAhpTk+Irti9I2q9V5Z7mQggRn4uqwwqwiNxJidu2ruMCjBib\r\nkTWBnQvh6wi4GjgNr6fzUOByVARiDRwYJMj0/ObxbQL5gPRPAO4YDn4DG4UvHof7ih9gXkZXO1RaI5XbVVbLNCdQRCtXMC4AsLCzwejUXu+vkwywFsenqypmp+VWD\r\nAb6xqTxaG0vbYPp6JO8MQI6BWuIpGlN3Z8yZV0fnU4OAeXgrQz46dVdrDwIOAxEr849TJtH//3JobHP7A2bXD8C8CLt40uE1xfPaFhC4JAGxfhes7mzYgHqqlDLe0\r\nORZWbk0brdVi10hqoOxyrPMcemwxxG8DYoErilQPUQiFlawoEBQKNTyswGuE12a0L5BB4qthcLDE8kjBsSzFH1vVB/KstTe2jFktWu8XeyU9NZjeRsYOVpovIpoQ/\r\nwPL3oXJUuRc//9/oUYAAQYedl0fUGAwAAAABJRU5ErkJggg=="
#define WEBDUINO_SNEEUW_DATA       "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nkNJREFUeNrsVsttwkAQNVYKoARoIJgKMBUAFQROHAkVBCowHDnhVGCoAHcApAFIB3SQvLHeSqNliZePcklGejLY63kzuzNvHAR/zSrXLF4sFlVc+kAHiNWjI5AD0+\r\nFweHwoMUiFaAnUSpbOQD5+CDFI+yQ1tgPWzFICaQFdoMrnKcgHNxGDLKIzsRGdnoABnK4c6yWADIh4S7Z94k0MB0KWXNjSJpztSnZnq8jrl848tF5KGLUmzZnptIy\r\nU1lO/R6UZW+coRGMQpbe0CnxlPKYiYGBlZx6qNklUa9RvJaXteTV+D+CYuLa6rypSiud0pz7seES5uvemySvMeENB2IG0+UiFYncs7YILrXXrR0sjC/Ks4MLf0GUW\r\nlqmZyEXc8XG03X9kQHwl/6eruMwWR9TkMutaQ8LHWi7ilD0ntqT82VlGwBew5a0X/s88B0yskwx5BoVg8IGQbkVQ2N+BEpWVUrUa2+bdY8Bkykd6ptWOKWQWz0Xwk\r\nV0RlOr5WbPxPL7QQplD73tmwIRW9Uk0bapXoNSnZQnNgGv66igSwPyPLVJZ29ZT7clR+qI2dUYdk6h4AdlJ1imuR1xXKnOxVypVqlpHnuf0efunj6PgJKsDM6qpAj\r\n0huPpP74bXkAAb3b+SObOrqgyLmijzF3qSmjMr2oKtZcjGKtNiQCCg2d3EIJCz2xDFlGFlJ0oCa5y7ufpcujtj2cqZ496c2y1kTSk8QDrCa7pVPLe6y7488RynQnR\r\nPYfoWl6nYHtWqEfzblfYtwAAoa/jaJuNTAQAAAABJRU5ErkJggg=="
#define WEBDUINO_ZONNIG_DATA       "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\ndxJREFUeNrUVsttwzAMlY0M4F56TiZIskGzQFFvYE8SZIRM4GyQogvUG9iZID735A1SEngEWNWM/GnhVgARWaEeP3qk5NxMIxqz6fb2mNBPgc88ev5oh2LEIx3ekLxANmMA4rlSHVupJFlOBWcMHEvYMBSvLDTPDM\r\nyGpIU0htFM4SRBciHSq1pi8pwGRpop8vFYEUZzN2Io5GqpMMCfWAzbhed407uclNc1bdxijVm872ByTXIgvVfoVdAxsxUNSB87kQXUTmQo/7EG0mGUI7tgvkY9DzIe9TDKoGd8Mot3BFx7OpzWdxJhbyppn9JA9mr\r\n+zSgIyWs7Y48dMepME6YRJtJ/N0kvraWB7Jwl7aQbqfLUzYjJ2i4U/fU5cUofvHK59MjORXB4LxkoaVqpIxB+pLP1aomYWXj0WiKnqyTPnWJvaKzVuZeYbv1U92V1pc5/20UuxewKnyXp7aay+qDm7zDgjHKScfy/\r\nDeS3WmZ875LgGsYZOwCmQg5vlOhWufACe7NBEfv3qTQD/1r02Kv/u4Xu89keAouOffKkSSyjcK5SJdZ4vfuE+i8Ungu9QFhpBS+tSJdwLPGawxfjCqe1OleX8XZqW+x68sz+rl6M3FejiTijvP7u+BRgAKv37Q33o\r\nXCDAAAAAElFTkSuQmCC"
#define WEBDUINO_ZWAARBEWOLKT_DATA "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nWtJREFUeNrsVsFxwjAQFJn8QzpwCaSCmAogFWC//E0qCOlAPP3CVECogHQQSnA6cAnZY9aZi5CNGYsP45250WCdtHcr3QljBgy4NYwuXZDneYRhDnuGjfn5ANtlWfYVnBiEQvIOe21xE+I3BHAIQkzSPWzCTxXsE/\r\nYDe6ACkZqbniMfnSETopiyxpyysA9sXDn+Qr6m/DL3BJ+yM7GSNFFnWENktC3BTqiMrCvgm3Yi9khaX5yjhNho2uFYlgxc8OgqU+Pe+a1JJbNVm1wNsIp4i0A22KNozBgOCc9IkPqcL6iAb49qqb5wd2pyUZdEH1J\r\niJReQ5WUYxJZHeSJ1rBb1gg5cKRmxByzdjI2qw2BgENZR1Ut8Dew4Rj7i0o3qmtDEG44JG0FILJzk/hFbdb57kMchGHm5Eie5k86lW97fc9eDV/f4kv27aurVx5rTFyEAJIEX3QXbXid5bWY9A6j4B6EwAwbcPH4F\r\nGAAyCH649/5tRwAAAABJRU5ErkJggg=="
#define WEBDUINO_WOLKENNACHT_DATA  "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\ngBJREFUeNrsVb1OwlAUrobFrZFRBkzUUepidCsDcaQ8ge3EaPsEwhNQR6bWJ7BvQDeNi+ioBppQBkkwfQTPJeea4/UiLZTFcJKT2597z3fOd36uomxkI2uSrbwNPj2/lGG5BtVB2XMCap1UjgO6r5AzqAmLJ3xW0Y\r\nH1RAygLMIevkYELIBoG+L+TBF3u11mzMAoFAISkEj7oG3QO3y3ZLYKKQFVzJs9Z0tnHMfqXqnEnh1QDb+HEG0iO7CdErRHQJkhH6PiBaOO45Hy/vaaAFCILCiy3C7MMQDqWJl1EoHTbDZdiWMdUBM/uadn5zewDjn\r\nV4Iy/EBgMtWC5EvI4MwCg/h+OegR8H8A9dJwx1AZwVwpMKNXI/5AXEIBaKdIyRHpZ1G2JvRlrzAlaXB7ZxA65AJZk7KpbLELz8eFe3S0WrYPDow5GzqXyXdWYTyMNpQskRGDGnvk5nZrjnVjBauet5tCqvuQHVwDl\r\nhqtofFbZrNonk48EWazy9uJUa4SqpQVTE6K6wCQbIkY0GCgXtVpL1scamUJ5Ci9IFdP5C7gvOJCLIANS2yJwfQ1Xrypg/ADmudWBEjsvRLBlkrEZzRsgdPIEOB6jFUBt0lqsW6rzbicHveM9bcDhRKQopehCizXSz\r\nGobZ3U5B7Z9ZC7Jcjtpkosi0zBZYuRu5B/JlwADAHQbsA24cuD8AAAAAElFTkSuQmCC"
#define WEBDUINO_NBEWOLKT_DATA     "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nWtJREFUeNrsVsFxwjAQFJn8QzpwCaSCmAogFWC//E0qCOlAPP3CVECogHQQSnA6cAnZY9aZi5CNGYsP45250WCdtHcr3QljBgy4NYwuXZDneYRhDnuGjfn5ANtlWfYVnBiEQvIOe21xE+I3BHAIQkzSPWzCTxXsE/\r\nYDe6ACkZqbniMfnSETopiyxpyysA9sXDn+Qr6m/DL3BJ+yM7GSNFFnWENktC3BTqiMrCvgm3Yi9khaX5yjhNho2uFYlgxc8OgqU+Pe+a1JJbNVm1wNsIp4i0A22KNozBgOCc9IkPqcL6iAb49qqb5wd2pyUZdEH1J\r\niJReQ5WUYxJZHeSJ1rBb1gg5cKRmxByzdjI2qw2BgENZR1Ut8Dew4Rj7i0o3qmtDEG44JG0FILJzk/hFbdb57kMchGHm5Eie5k86lW97fc9eDV/f4kv27aurVx5rTFyEAJIEX3QXbXid5bWY9A6j4B6EwAwbcPH4F\r\nGAAyCH649/5tRwAAAABJRU5ErkJggg=="
#define WEBDUINO_NBLIKSEM_DATA     "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nspJREFUeNrUlr1u01AUx+2oEquDaNhQwocqFkgkhGDCfgEUD8xJpoxNnqDhCZKOmZLMDDW8gL2BWGoY2q2J2EiRuG9Qzon+F53c2rlO1CJxpavrOPb9nfM/H9euc0vj9Nt3X/xMG8+fKfn/3i0Am7QMaVbF7fc0B/\r\nK50g1De7ScGNCVx+az7i6A8Xhcp8XTm3a7XQVpY9yb0mzjuk8yj3YGE4xB7FErw6PkycGBVy7fZYMShrEA+K9sxrcwGB5mSbg27u3vOw8fPQ7pUmnvCZrJKBX0NAZUIVHKJK/Lk/emGfGzvy4vna9fPr8hWJKT3Zs\r\n9JhhLemh4yNCAYGnOOxMR1+Dlq9ct/F7QDMmYNBcsvKtn7B0SNLKow++yhxGBdZx1EiYw4piNMKWWUJa0BjlrNijGMVafJG8ulz87opR8KNBcayBk7UBAG1JSul4UTH4dW/ZyuLi4WGVzpXJfhisyk6ulPc2Lo21w\r\nPUOpEWT1CO5x0qG2azrWrvD4Cpe1LTy0VYROOIV91Vo50QP+DrIWGX1APcT4Wh2nwsrqTVHhod67fg2MB7Sn7axNnn74satBnq1zzbAeokVKKNfjfFs49tF7RXnn8UgcAE0tEcEGOQ1lNa4+VXqGV6n7dhkhZPq0S\r\nsxKKRnxCFEOEaAeWueqzM7fPVgYUB+H/pGY2sgqDFJIsuKnE6Q9xcshNp6RARHAc6Ofs2EN8liJakllGVnB8LaHbsQyTRCChMABJB4arwUETYrEfxP4BCCO/UcRr/DszgvefJ6RsRo6IwOmm8ClHKivmzmgup0mkH\r\nmYUyY+7lsPlLwPgSPhgRIfAB2ROEnGR1wEuZUNnPd5OxMtL4YXgc5q2jhAcsXinSnd7xStcVtWrzV5AiujlDS4T9DRNs1lz1JKfz9RJRRjgrVjS6StB8FjZLfZrdo0f9OsO/9yMJBm1fkfxx8BBgADTCctrebZ+gA\r\nAAABJRU5ErkJggg=="
#define WEBDUINO_NBUIEN_DATA       "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nvlJREFUeNrEVzuLE1EUvgkxgVXCIC5YiCY+WGw0WUG0MimtTEBrJ1VKk1+w5hfspEy1Y22R2UIsE0sLN8kiu4WFo+3KMkQIBCN6zvBduFzvJLNxxIHDZO7jfOf53R\r\nsh/tOTSlLZaHJo0csmeUJSwbBPUi3fveOrazMJgpbo1ScpaFMFGOEmDgzQAYllmB6SePpgOiGHdxVQh2SshLlOYQ70DbE97vV6HK7nJCUIKxvm8/mJkk+XQNoUgV/\r\n49kygsYAJkD3ZI6lpUzxem06ntcnoQNza2hIbG+c72povUXozMUAH8FDm6xVCKFC99nw+t46PjsTPxULP8bXY7URgJSWklgLaaDabrmF9AdXM63xaU6RQj5R0FE3h\r\nTmlKuEhaBgPbpNBZEhkGH8HQxv0HDwXSI1BoDQIfG4Fp8x6aX4Z0H5sqBPoyRi1Io3lPncB3FH3SAPZ8n4xwUthkKxYu9W5F1Q+UIbe8fU+cy2ZtfS0Bp2Qf78geX\r\nAdU9mwmkxnncjn5bY8OPtQWi4We37BOUsjPZwwWCdj/WzaB91xw1qXNTXH9xk055XCfS+aS3BokAcoP6eEaqfPvbycnYjabeajutokyrSRPKoCHjnw8nHT10ymt8K\r\noMUZJPEDWRJssCtI9aZH88t19/tUgGJLsx81xSyMePOp0kx1ZoQx9UaTqBKgbOjiIU2Z5DU+2oBKL2siSRd1z+zsXHFpgppM7jZ1fdCEAmkEcw0EKoqwQ8XkWZNXi\r\nm3iI6BCzTwMY0OIQE7iEF4fqn3993r/w4VQkkPItNoJF3LhgQ5ufNhbL3KXu5AGM85UBoQ3lfElLr9K30dEyA3tqXPS4ohJhBq3jLdBRBkTzmUgQaZyn3VReBFhQH\r\nmncOxuVc56x9turO9QLvLnnkI9Sc564sNM4j5hIF9kEwHvcwX2UIpIpCC2+V9D1ch1mWhpqUlpHrAVokoN8V5ZwN1qW0lddbAEkq7SrhH0b1cyLACqiHsAaQ9j/97\r\n4SWstEygUjo+S3AAMY/Hpq3sx6nAAAAAElFTkSuQmCC"
#define WEBDUINO_NHALFBEWOLKT_DATA "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\ngBJREFUeNrsVb1OwlAUrobFrZFRBkzUUepidCsDcaQ8ge3EaPsEwhNQR6bWJ7BvQDeNi+ioBppQBkkwfQTPJeea4/UiLZTFcJKT2597z3fOd36uomxkI2uSrbwNPj\r\n2/lGG5BtVB2XMCap1UjgO6r5AzqAmLJ3xW0YH1RAygLMIevkYELIBoG+L+TBF3u11mzMAoFAISkEj7oG3QO3y3ZLYKKQFVzJs9Z0tnHMfqXqnEnh1QDb+HEG0iO7C\r\ndErRHQJkhH6PiBaOO45Hy/vaaAFCILCiy3C7MMQDqWJl1EoHTbDZdiWMdUBM/uadn5zewDjnV4Iy/EBgMtWC5EvI4MwCg/h+OegR8H8A9dJwx1AZwVwpMKNXI/5AX\r\nEIBaKdIyRHpZ1G2JvRlrzAlaXB7ZxA65AJZk7KpbLELz8eFe3S0WrYPDow5GzqXyXdWYTyMNpQskRGDGnvk5nZrjnVjBauet5tCqvuQHVwDlhqtofFbZrNonk48EW\r\nazy9uJUa4SqpQVTE6K6wCQbIkY0GCgXtVpL1scamUJ5Ci9IFdP5C7gvOJCLIANS2yJwfQ1Xrypg/ADmudWBEjsvRLBlkrEZzRsgdPIEOB6jFUBt0lqsW6rzbicHve\r\nM9bcDhRKQopehCizXSzGobZ3U5B7Z9ZC7Jcjtpkosi0zBZYuRu5B/JlwADAHQbsA24cuD8AAAAAElFTkSuQmCC"
#define WEBDUINO_NMIST_DATA        "iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAA\r\nTpJREFUeNrsloENgkAMRcE4ABvIBIoTiBMoEwgTGCYRJlA30AlgA8QFZAMZwR4pSW1Oc5A7TAw/qXcK8tJPW7CsUf8uu8+fivLuwBJCrCAc/LmGiJaLea1yjWkPqA\r\nAeCJAqhci1gxF6/HC4wlDSpAPUZ9AL2Qt712CzfjDa2yqGuFKLu0CVwZCtB4uHX3OAJLC65JS8a62oZuyT/VlHO6mCHVZEFisk3xRYJmrvHntbO5hCNuIDi+lEHMm\r\nw8vVOLrjoEwFt69wwy4wUHu3nCs6JdFidsuw8HI9r5oiL9zz85oD9ZVjICuzICk0MkRL3O9ZizezG9W3CidtkS6AFs86EApnVpqENQ/aQCAaAJ799EcC2MJolFFQu\r\nAz9YRZpQjA+Xtz52BnB3JnsDEUNgaxBak/E6apRZvQQYADiuXrT7tYRBAAAAAElFTkSuQmCC"
// lichtbewolkt ontbreekt 11-8-2021
#define WEBDUINO_LICHTBEWOLKT_DATA "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAC2VBMVEX1xRX3yRbEysf20CHz0xj3zx6bnUOampnqyzD0yhj1wBL1xxfZ2tTs6+r2xhjk5OP1xRXv7\r\nu7m5uXh4eD5+fj1uQ72yRjj49zp6end3dz1uA31tw31sQv1tQzT0s71tQz1sAv20iDt7On0qwn0rQr2zxnp5+Dq6eP1sgz1rwrs6+jl5OH1swze3tvs6eDp6OTt6u\r\nHr6ujc3dzZ2Nf0qwng4Nzc3Nzb29rc29v20CH20B/3zhn2ziHs6+ff3tn+3IXb3Nvg39/2xxnh4d72zhn3yxf2xxT2yBT2yRb2zyT2xRP3zhf2yxf2yxn2wBL2wRL\r\nr38D2wBL2xRL3xhP2yhf39/bn0YTw2Zby8fD2yBXg17n5yUPs3rXi0Jbv7+/n3Lfr4sH1vRL2xhTy8vLi0Z3r3bDq4cHs3aTSzsPs37Xn3LX1sxro0I/0vTru7u3s\r\n4bzV0sr5zmPr3a32xEbXzbLo2KP1vTT1vBLzymHyz1zu7uzq4sb713nqz4DyzFTs1ozs0n/yy1b1xjj8+/r+/fz+lQL9hQP+igH29vX+kALV0sjZ1s3d2tHk4t778\r\nm/z8vD83B/+mgL771L782jp6OP+jQL89YLp5NPg3tj78F3j3tDy7bj89Hj99o39+KH895bz8Ofu5snn38nx7K7x69H9wGD38aT9u1Pt4sDe2MXw6qT6+PL+iw/++q\r\n7z7tz50Y782Cfz7Jv+ozL9+rr51Zb8y3X9x2r94iLv68D62EL41k/82zH33qz11lz68t/+mhL36mrx4bj+khr+mib679P+q0D62qH6zoP+qgrs5bD91Rvk3b7+/Mb\r\nz7sj+oh/9sDb+s039riPu0nXy3qP264z363z8t0L55b/59en/1Hz66Mj9zhbY0Lz71Dfx02j540r5uCr75D37yCv38a/9xRby14f64bT82mn85C7bz6n+oQb63Xjf\r\n1rno3bP9vBP65Fr8y1T44JT0x15/79ByAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQflCAsJIQt5sBu1AAABP\r\n0lEQVQ4y2NgGKZAUwWfbOHyTZs2XbiJS43K8kNtILDmaA12+WudUyGg85w9NgUf9rdCwPkLW9cXY8pXXLx4fMmxc0uPXX+2fv2WW5lo0qHN9qtP5B5cN6V/9Y0F+6\r\n4vWhQQj6rAmL+5ufndpYMLZ3V3d/d0r5y1yAhJt4iEhLCgSEvzo0f3wg+u7Lrz8s3K0ycRJthJCvFLSvD2d4g0h0fffr9r76VLQFQIl082kxQUlBCWnDGDt6O/4/K\r\npUxt27tp5anciXEGcubGZo7CxXFd374zeKf0d+w7snDBh+0wvJAfGGRk5mBdknH67Y+GOHWfPnp39/v77XSGonjA11Y3K2bhx8eJlG/sOHJg0aW4wZkDlN1xeNre\r\nvb/LkyTP75iVhj63ytfPmzp07f210MK7ojiz3i/b3C/ZgGAUkAgC3f5CZXD0ipQAAAABJRU5ErkJggg=="
// nachtbewolkt ontbreekt 5-2-2022
#define WEBDUINO_NACHTBEWOLKT_DATA "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAACalBMVEX7+vnd3d76+PiOgIHy7PLW0dUqNTNaYVny8uz7+Pn6+fj39vb5+Pj19PSOiZD5+Pj29fX9+/vFycn59/jQ0tPKzMr8+vr9+/v9+/v8+vr9+/uPkpH9/Pvq6uqfop/+/Pzl5ubp6enh4uP8+/v+/PzO0dH9+/zp6enn5+f9+/vf39/8+vvp6ens6+z9+/vq6un+/Pz9+/v8+vra29rk5ebl5eX9+vv8+/rq6em8v77+/Pzj4+Pe3t7o5+fo5+f7+vrv7u7g4ODT1NPr6+vn5ubl5eTs6+vs6+zn5+fb29vg4ODi4eHGx8b8+vru7e7o6Ofb2tri4eHT09PY2Njd3Nz9+/z8/Pzq6urj4+P8+/vi4uLh4OH8/PzZ2NjOzs79+/z7+/v7+/vj4+Pz8/Pa2tr9/Pz6+vr39/fx8fH5+fnw8O/v7u719fXY2dnc3Nzx8fHz8/Pd3d3e3d39/Pzr6+rV1tXY2dnz8vLr6urx8fH39/fY2NjPzszV1dXNzc3FxMGmpqHj4+P39/f////k5OXo6Oji4uL8/PzW1tbX19ft7e3V1dXp6en9/f3T09Tr7Ovx8fH09PTe3t7l5uX6+vrS0tL29vbm6Ob5+fnf39/v8O/v7+7c29vy8vLm5ubr6uvY2djg4ODh4eLg4N/s7Ozb3NvR0dHd3d7a2tvPz8/h4uHd3Ny/v7/BwcHExMO6urmnqKfa2tmkpKPMzMy1trW4uLe8vbzIycjGx8bOzs3Ky8qurq2rq6qxsbDo6umzs7KOjo6Tk5KXl5eenp2goaCbm5qJiYmDg4NtbWxiYmF7fHtcXFxYV1dTU1LvQFYBAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQfmAgUKCSXWhhu1AAABaklEQVQ4y+2RT0sCURTF/T4tWrQJsoVkRUpoCGEhBEVR4KK2togZZuY5/hma8Q+KM2/mjU9z0Jl6pqOWSiYiLfpOtUpN6wNEB+7q/ODee47D8ed0sf85v/jn4TQdESM/2WHgpdgEy3N2c3iyNGefZXkWgOSzbbe7L4OuGfq2PKfivJpOJkc1rWtJ/ZbZ884AHKRKLBbFdJJRQBbUjObb+9HEXnfKuACRLEuqSNO3aQBMQ+u1JsCqf6VCUbJhRDkOYZHOAVGnSObyC9gOevyBciZe1ko6VHGhoCiimLFvpo+4ut5xRglCKRYm+moMxxT4aM4+EvQ645V8XiISgn0W6pARNr5F4Wk3TMsS4kWdQZAIZfdcWO3xeDCqP5AoEqqpu/bBHLDbqWcYc2wpSkxCOuWbryNQe+VjOJujswouri0AHPZ9g4NSifCEr7rcwQVEaGiRlCaUO37X4dbi1k+fikTy7S0fbzr+NaUPYd+mrXPb4IsAAAAASUVORK5CYII="
// hagel en nhagel zijn hetzelfde
// regen en nregen zijn hetzelfde
// sneeuw en nsneeuw zijn hetzelfde
// zwaarbewolkt en nzwaarbewolkt zijn hetzelfde
#define P(name)   static const uint8_t name[] PROGMEM

// returns the number of elements in the array
#define SIZE(array) (sizeof(array) / sizeof(*array))

#endif
