#include <TimeLib.h>
time_t t_tijd;
time_t dynDNSResponseTime;
char IPtijd[16];
char myIP[32];
char priorIP[32];

bool GetWANIPAddress() {
  String strmyIP;
  char GWAlookfor[16];
  int rslt;
  char ocbuff[16];
  bool succes = false;
  t_tijd = now();
  sprintf(IPtijd, "%02d:%02d:%02d", hour(t_tijd), minute(t_tijd), second(t_tijd));  // IPtijd is max 16
  sprintf(temptxtbuff, "\r\n%s Start retrieving WAN IP via api.ipify.org ", IPtijd);
  textlog(temptxtbuff, false);
  // Get my ip address
  EthernetClient whatsmyipClient;
  rslt = whatsmyipClient.connect("api.ipify.org", 80);
  if (iplog && (rslt != 1)) {
    sprintf(temptxtbuff, " request result: %d ", rslt);
    textlog(temptxtbuff, false);
  }
  if (rslt == 1) {
    textStringLog("-> connected ", false);
    whatsmyipClient << F("GET / HTTP/1.1") << endl;
    whatsmyipClient << F("Host: api.ipify.org") << endl << endl;
    const static char whtsmpClnt[] PROGMEM = "vegur\r\n\r\n";
    strcpy_P(GWAlookfor, (char*) whtsmpClnt); // GWAlookfor is max 16
    /*received from api.ipify.org: HTTP/1.1 200 OK
      Server: Cowboy
      Connection: keep-alive
      Content-Type: text/plain
      Vary: Origin
      Date: Tue, 10 Nov 2020 16:00:11 GMT
      Content-Length: 14
      Via: 1.1 vegur

      77.173.126.238
      80.60.45.94
    */
    if (whatsmyipClient.find(GWAlookfor)) //response from websit
    {
      todayIPCnt++;
      strmyIP = whatsmyipClient.readStringUntil('<');
      if (iplog) {
        textStringLog("-> response: ", false);
        textStringLog(strmyIP, false);
      }
      if ((strmyIP.length() > 7) && (strmyIP.length() < 16)) {
        strcpy(myIP, strmyIP.c_str());  // myIP is max 32
        textStringLog(" -> My WAN IP: ", false);
        textlog(myIP, false);
        succes = true;
        if (strcmp(myIP, priorIP) != 0 ) {
          int retr = 0;
          bool resp = false;
          do {
            resp = updateDNS();
            retr++;
          } while ((resp == false) && (retr < 3));
          sendEmail(1);
          strncpy(priorIP, myIP, 17);   // priorIP (destination) and myIP is max 32
          if ( upTime != 0) {
            textStringLog(" -> WARNING WAN IP adres changed!", false);
          }
        }
      }
      whatsmyipClient.stop();
      textStringLog(" -> GetWANIPAddress -> ok", false);
    }
    else {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: in response from api.ipify.org, \"vegur\" not found in response.", hour(), minute(), second());    // temptxtbuff is max 1024 char
      textlog(temptxtbuff, false);
    }
  }
  else {
    String conerror;
    switch (rslt) {
      case -1:
        conerror = "TIMED_OUT";
        break;
      case -2:
        conerror = "INVALID_SERVER";
        break;
      case -3:
        conerror = "TRUNCATED";
        break;
      case -4:
        conerror = "INVALID_RESPONSE";
        break;
    }
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: %s connection failed to retrieve WAN IP address", hour(), minute(), second(), conerror.c_str());    // temptxtbuff is max 1024 char
    textlog(temptxtbuff, false);
  }
  if (whatsmyipClient.connected()) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: GetWANIPAddress stop unexpected connection", hour(), minute(), second());    // temptxtbuff is max 1024 char
    textlog(temptxtbuff, false);
    whatsmyipClient.stop();
  }
  return succes;
}

bool updateDNS() {
  //  updateDNSafraid(DYNDNS_API_KEY);
  //modify  to spi bus memory
  char uapikey[41];
  int i;
  for (i = 0; i < 40; i++) {
    uapikey[i] = DYNDNS_API_KEY[i];
  }
  uapikey[i] = '\0';
  textStringLog("\r\nStored DYNDNS_API_KEY from EEPROM reads: ", false);
  textlog(uapikey, true);
  return updateDNSafraid(uapikey);
}

bool updateDNSafraid(char* afraidstr)
{
  // meldingen zijn
  // Updated 1 host(s) baaisolar.remote.mx to 65.234.2.34 in 0.176 seconds
  // ERROR: Address 77.172.249.196 has not changed.

  bool succes = false;
  textStringLog("Update Dynamic DNS at freedns.afraid.org: @", false);
  t_tijd = now();
  sprintf(IPtijd, "%02d:%02d:%02d", hour(t_tijd), minute(t_tijd), second(t_tijd));  // IPtijd is max 16
  textlog(IPtijd, false);
  textStringLog(" use API key: ", false);
  textlog(afraidstr, false);
  // Get my ip address
  EthernetClient dynDNSClient;  //
  if (dynDNSClient.connect("freedns.afraid.org", 80)) {
    if (!min_serial || iplog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d DynDNS send cmd: GET /dynamic/update.php?", hour(), minute(), second());
      textlog(temptxtbuff, false);
      textlog(afraidstr, false);
    }
    dynDNSClient << F("GET /dynamic/update.php");
    dynDNSClient << F("?") << afraidstr << endl;
    dynDNSClient << F("Host: freedns.afraid.org") << endl << endl;
    //    if(dynDNSClient.find("\":\"")) //response from websit
    //    {
    dynDNSCnt++;
    String strdynDNSresponse = dynDNSClient.readString();
    char uDacobuff[512];
    int cpybuflen;
    if (strdynDNSresponse != "") {
      if (iplog) {
        textStringLog("\r\nDynDNS response: ", false);
        if (strdynDNSresponse.length() > 511) {
          cpybuflen = 511;
        }
        else {
          cpybuflen = strdynDNSresponse.length();
        }
        strdynDNSresponse.toCharArray(uDacobuff, cpybuflen); // uDacobuff is max 512
        uDacobuff[cpybuflen] = '\0';
        textlog(uDacobuff, false);
        textStringLog("\r\nprocessing response to: ", false);
      }
      int ncStr = strdynDNSresponse.indexOf("not changed");
      if (iplog) {
        sprintf(temptxtbuff, "not changed found at: %d", ncStr);
        textlog(temptxtbuff, false);
      }
      if ((ncStr > -1) && (strdynDNSresponse.length() >= ncStr + 11))
      {
        strdynDNSresponse = strdynDNSresponse.substring(ncStr, ncStr + 11); // response for webpage
        succes = true;
      }
      ncStr = strdynDNSresponse.indexOf("Updated");
      if ((ncStr > -1) && (strdynDNSresponse.length() >= ncStr + 7))
      {
        strdynDNSresponse = strdynDNSresponse.substring(ncStr, ncStr + 7); // response for webpage
        succes = true;
      }
      int i = strdynDNSresponse.length();
      if (iplog) {
        sprintf(temptxtbuff, "\r\nlength from strdynDNSresponse (i): %d", i);
        textlog(temptxtbuff, false);
      }
      if (i > 63) {
        i = 63;
        //        strdynDNSresponse[63] = '\0';
      }
      // Length (with one extra character for the null terminator)
      strdynDNSresponse.toCharArray(dynDNSresponse, i + 1);
      //      dynDNSresponse[i] = '\0'; //toCHarArray sets terminator
      textStringLog("\r\ndynDNSresponse: ", false);
      textlog(dynDNSresponse, false);
    }
    dynDNSClient.stop();
    dynDNSResponseTime = now();
    if (iplog)
      textStringLog("\r\nEnd updating address in DynDNS", false);
  }
  if (dynDNSClient.connected()) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: dynDNSClient stop unexpected connection", hour(), minute(), second());    // temptxtbuff is max 1024 char
    textlog(temptxtbuff, false);
    dynDNSClient.stop();
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Exit code from updateDNSafraid: ", hour(), minute(), second());    // temptxtbuff is max 1024 char
  textlog(temptxtbuff, false);
  succes ? textStringLog("success", false) : textStringLog("fail", false);
  return succes;
}

/*
   ============================================================
   TFTP  for up and downloading known IP scanners list, TFTP start by command setabuse
*/
/* Examples of network traffic.
     Note two cases when ACKs with block# of 0 are sent.

     Download without options:
     tftp -> "\0\1FILENAME\0octet\0"
             "\0\3\0\1FILEDATA..." <- tftpd
     tftp -> "\0\4\0\1"
     ...
     Download with option of blksize 16384:
     tftp -> "\0\1FILENAME\0octet\0blksize\00016384\0"
             "\0\6blksize\00016384\0" <- tftpd
     tftp -> "\0\4\0\0"
             "\0\3\0\1FILEDATA..." <- tftpd
     tftp -> "\0\4\0\1"
     ...
     Upload without options:
     tftp -> "\0\2FILENAME\0octet\0"
             "\0\4\0\0" <- tftpd
     tftp -> "\0\3\0\1FILEDATA..."
             "\0\4\0\1" <- tftpd
     ...
     Upload with option of blksize 16384:
     tftp -> "\0\2FILENAME\0octet\0blksize\00016384\0"
             "\0\6blksize\00016384\0" <- tftpd
     tftp -> "\0\3\0\1FILEDATA..."
             "\0\4\0\1" <- tftpd
     ...
*/

// TFTP Opcode values from RFC 1350
//
#define TFTP_OPCODE_RRQ   1
#define TFTP_OPCODE_WRQ   2
#define TFTP_OPCODE_DATA  3
#define TFTP_OPCODE_ACK   4
#define TFTP_OPCODE_ERROR 5

// TFTP Error codes from RFC 1350
//
#define TFTP_ERROR_UNDEF        0
#define TFTP_ERROR_NOT_FOUND    1
#define TFTP_ERROR_ACCESS       2
#define TFTP_ERROR_FULL         3
#define TFTP_ERROR_ILLEGAL_OP   4
#define TFTP_ERROR_UNKNOWN_XFER 5
#define TFTP_ERROR_EXISTS       6
#define TFTP_ERROR_NO_SUCH_USER 7


#define TFTP_PORT 69

// TFTP OP codes
#define ERROR_UNKNOWN     0
#define ERROR_INVALID     1
#define ACK               2
#define ERROR_FULL        3
#define FINAL_ACK         4   // Like an ACK, but for the final data packet.?????
#define INVALID_IMAGE     5
#define ERROR_LARGE       6   // packet size > 512 not support (excluding header)
#define INVALID_HOST      7
#define SEND_DATA         8
#define ACK_EOT           9
#define FLASH_ERROR      10

#define TFTP_DATA_SIZE    512


#define TFTP_OPCODE_SIZE  2
#define TFTP_BLOCKNO_SIZE 2
#define TFTP_MAX_PAYLOAD  512
#define TFTP_PACKET_MAX_SIZE ( TFTP_OPCODE_SIZE + TFTP_BLOCKNO_SIZE + TFTP_MAX_PAYLOAD )
#define TIMEOUT 60

bool convertHTTPlistReq = false;
bool convertIPtableReq = false;
bool outputHTTPlistReq = false;
bool outputIPtableReq = false;
bool EndOfTransmission = false;
uint16_t lastPacket = 0;
uint32_t bufferPointer = 0;
uint8_t DownloadStarted = 0;
uint8_t rdbuffer[EEPROM_PAGE_SIZE + 1]; // size of an EEPROM page
uint8_t* bufPtr = rdbuffer;
uint16_t pageOffset = 0; // EEPROM page offset
unsigned int localTFTPport;
unsigned int remoteTFTPport;

IPAddress UDPSendRxTFTPTo;

/* Opcode?: tftp operation is unsupported. The bootloader only supports 'put' */
#define TFTP_OPCODE_ERROR_LEN 12
char tftp_opcode_error_packet[]   PROGMEM = "\0\5"  "\0\4"  "Opcode?";

/* Full: Binary image file is larger than the available space. */
#define TFTP_FULL_ERROR_LEN 15
char tftp_full_error_packet[]     PROGMEM = "\0\5"  "\0\3"  "Flash Full";

/* General catch-all error for unknown errors */
#define TFTP_UNKNOWN_ERROR_LEN 10
char tftp_unknown_error_packet[]  PROGMEM = "\0\5"  "\0\0"  "Error";

/* General catch-all error for unknown errors */
#define TFTP_FLASH_ERROR_LEN 22
char tftp_flash_error_packet[] PROGMEM = "\0\5" "\0\0" "Flash write error";

/* packet too large only 4 + 512 datagram packets are accepted */
#define TFTP_LARGE_ERROR_LEN 25
char tftp_too_large_packet[] PROGMEM = "\0\5" "\0\4" "Packetsize too large";

/* Invalid image file: Doesn't look like a binary image file */
#define TFTP_INVALID_IMAGE_LEN 18
char tftp_invalid_image_packet[]  PROGMEM =  "\0\5"  "\0\1"  "Invalid image";

/* Invalid image file: Doesn't look like a binary image file */
#define TFTP_INVALID_HOST_LEN 17
char tftp_invalid_host_packet[]  PROGMEM =  "\0\5"  "\0\1"  "Invalid host";

uint8_t timedOut() {
  if (DownloadStarted) {
    if (tick > (TIMEOUT / 12))
      return 1;
  }
  else if (tick > TIMEOUT) {
    return 1; // close UDP port
  }
  return 0;
}

/*
   Tempory routine to print the readback of the flash
*/
void show_buffer_Row2Serial(uint8_t* bufferaddr, uint16_t nrofbytes) {
  char asciichar[17];
  int acpos = 0;
  char karakter;
  uint8_t* sfbufPtr = bufferaddr;
  Serial.println();  // start always on new line
  for (int axc = 0 ; axc < (nrofbytes / 16); axc++) {
    for (int i = 0; i < 16; i++) {
      // if this is the first byte of the row,
      // start row by printing the byte address
      if (i == 0) {
        sprintf(msgchararray, "%8d = 0x%08X ", sfbufPtr, sfbufPtr);  // msgchararray is max 64 char
        Serial.print(msgchararray);
      }
      // read current byte from flash
      // write byte in hex form
      karakter = *sfbufPtr++;
      sprintf(msgchararray, "%02X ", karakter); // msgchararray is max 64 char
      if (((byte)karakter > 0x1F) &&  ((byte)karakter < 0x7F)) {
        asciichar[acpos] = karakter;
      }
      else {
        asciichar[acpos] = 0x2E;
      }
      acpos++;
      asciichar[acpos] = 0x00;
      // if this is the last byte of the row,
      // reset row counter and use println()
      // to start a new line
      if (i == 15) {
        //      client << msgchararray << br << F("\r\n");
        Serial.print(msgchararray);
        Serial.print(F("        "));
        Serial.println(asciichar);
        acpos = 0;
      }
      // else just print the hex value with print()
      else {
        //      client << msgchararray;
        Serial.print(msgchararray);
      }
    }
  }
}

/*
   Tempory routine to print the readback of the flash
*/
void show_Flash_Row2Serial(long int flashaddr) {
#ifdef OPTION_FLASH
  char asciichar[17];
  int acpos = 0;
  char karakter;
  uint8_t* sfbufPtr = rdbuffer;
  Serial.println();  // start always on new line
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  for (int axc = 0 ; axc < (EEPROM_PAGE_SIZE / 16); axc++) {
    for (int i = 0; i < 16; i++) {
      // if this is the first byte of the row,
      // start row by printing the byte address
      if (i == 0) {
        sprintf(msgchararray, "%8d = 0x%08X ", sfbufPtr, sfbufPtr);  // msgchararray is max 64 char
        Serial.print(msgchararray);
      }
      // read current byte from flash
      // write byte in hex form
      karakter = *sfbufPtr++;
      sprintf(msgchararray, "%02X ", karakter); // msgchararray is max 64 char
      if (((byte)karakter > 0x1F) &&  ((byte)karakter < 0x7F)) {
        asciichar[acpos] = karakter;
      }
      else {
        asciichar[acpos] = 0x2E;
      }
      acpos++;
      asciichar[acpos] = 0x00;
      // if this is the last byte of the row,
      // reset row counter and use println()
      // to start a new line
      if (i == 15) {
        //      client << msgchararray << br << F("\r\n");
        Serial.print(msgchararray);
        Serial.print(F("        "));
        Serial.println(asciichar);
        acpos = 0;
      }
      // else just print the hex value with print()
      else {
        //      client << msgchararray;
        Serial.print(msgchararray);
      }
    }
  }
#endif
}


uint32_t countIPtableEntries() {
  uint32_t flashaddr = abusefilter1_addr;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start counting IPtable whitelist entries at startup", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08x", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  bufPtr = rdbuffer;
  //  while (*bufPtr != 0xFF) {
  while (bufPtr < (rdbuffer + EEPROM_PAGE_SIZE) && (*bufPtr != 0xFF)) {
    totalnets++;
    bufPtr += 4;
    if (bufPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFO Flash read pagebreak detected, reading last Flash page for converted blacklisted HTTP fragments.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      flashaddr += EEPROM_PAGE_SIZE;
      if (flashaddr == (abusefilter12_addr + EEPROM_PAGE_SIZE)) {
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d INFO end of flash pages for IPtable reached.", hour(), minute(), second(), __LINE__); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        return totalnets;
      }
      flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
      bufPtr = rdbuffer;
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08x", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d IPtable whitelist counts %d entries.", hour(), minute(), second(), totalnets); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  return totalnets;
}

uint32_t countHTTPtableEntries() {
  uint32_t flashaddr = HTTPblacklist1_addr;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start counting HTTP blacklist", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  bufPtr = rdbuffer;
  while (bufPtr < (rdbuffer + EEPROM_PAGE_SIZE) && (*bufPtr != 0xFF)) {
    totalHTTPcmd++;
    bufPtr += 16;
    if (bufPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFO Flash read pagebreak detected, reading last Flash page for converted blacklisted HTTP fragments.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      flashaddr += EEPROM_PAGE_SIZE;
      if (flashaddr == (HTTPblacklist2_addr + EEPROM_PAGE_SIZE)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Flash end of blacklist area reached.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        return totalHTTPcmd;
      }
      flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
      bufPtr = rdbuffer;
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d HTTP blacklist counts %d entries.", hour(), minute(), second(), totalHTTPcmd); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  return totalHTTPcmd;
}


void writeTFTPbuffer(uint8_t wrbuffer[EEPROM_PAGE_SIZE], uint8_t wrpageOffset, uint16_t nrofbytes) {
  uint32_t writeFileAddr = eepromEnd_addr + (wrpageOffset * EEPROM_PAGE_SIZE);
  flash.eraseSector(writeFileAddr);
  delay(25);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d Writing blacklisted HTTP fragment of %d bytes to TFTPbuffer in flash 0x%08X, reading from wrbuffer at address 0x%08X", hour(), minute(), second(), __LINE__, nrofbytes, writeFileAddr, wrbuffer);
    textlog(temptxtbuff, false);
  }
  if (!flash.writeByteArray(writeFileAddr, wrbuffer, nrofbytes)) {
    flash_error = F("writeByteArray writeFileAddr FAIL");
    StoreFlashError(flash_error, writeFileAddr);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Converted TFTPbuffer fragments writing... writeByteArray writeFileAddr FAIL address: 0x%08x", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  delay(3);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash write success address: 0x%08x, TFTPbuffer page: %d", hour(), minute(), second(), writeFileAddr, wrpageOffset); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
}

bool prepareIPtable2TFTPbuffer() {
  /*
     rdbuffer is global buffer
     bufPtr = global buffer pointer from rdbuffer
     rxBufPtr = recordpointer in global buffer
  */
  char cnvipbuff[4]; // the integer in array as ASCII
  char* cnvipbuffPtr;
  uint8_t ipnet;
  //  uint8_t* rxBufPtr;
  uint8_t wrbuffer[EEPROM_PAGE_SIZE]; // size of an EEPROM page
  uint8_t wrpageOffset = 0;
  uint8_t* wrBufPtr = wrbuffer;
  uint16_t recordcnt = 0;
  uint32_t flashaddr = abusefilter1_addr;

  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start converting IPtable whitelist to ASCII text, preparation for TFTP download", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  // delay(25);
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08x", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  bufPtr = rdbuffer;
  //  while (*bufPtr != 0xFF) {
  while (bufPtr < (rdbuffer + EEPROM_PAGE_SIZE) && (*bufPtr != 0xFF)) {
    //    rxBufPtr = bufPtr; //temp pointer for reading this entry
    //    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start processing record %d", hour(), minute(), second(), recordcnt); // temptxtbuff is max 1024
    //    textlog(temptxtbuff, false);
    //    delay(5);
    for (uint8_t idxip = 0; idxip < 4; idxip++) {
      cnvipbuffPtr = cnvipbuff;
      ipnet = *bufPtr++;
      sprintf(cnvipbuff, "%d", ipnet);
      /*
            if ((recordcnt > 677) && (recordcnt < 681)) {
              Serial.print("\r\nConverting: ");
              Serial.print(recordcnt);
              Serial.print(" value: ");
              Serial.print(ipnet);
            }
      */
      while (*cnvipbuffPtr != 0) {
        /*
                  if ((recordcnt > 677) && (recordcnt < 681)) {
                  Serial.print("->");
                  Serial.print(*cnvipbuffPtr);
                }
        */
        *wrBufPtr++ = *cnvipbuffPtr++;
        if (wrBufPtr == (wrbuffer + EEPROM_PAGE_SIZE)) {
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            // delay(25);
          }
          writeTFTPbuffer(wrbuffer, wrpageOffset, EEPROM_PAGE_SIZE);
          wrpageOffset++;
          wrBufPtr = wrbuffer;
        }
      }
      if (idxip < 3) {
        *wrBufPtr++ = 0x2E;
        //        Serial.print('.');
        if (wrBufPtr == (wrbuffer + EEPROM_PAGE_SIZE)) {
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            // delay(25);
          }
          writeTFTPbuffer(wrbuffer, wrpageOffset, EEPROM_PAGE_SIZE);
          wrpageOffset++;
          wrBufPtr = wrbuffer;
        }
      }
    }
    recordcnt++;
    *wrBufPtr++ = 0x0D;
    if (wrBufPtr == (wrbuffer + EEPROM_PAGE_SIZE)) {
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d%04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      writeTFTPbuffer(wrbuffer, wrpageOffset, EEPROM_PAGE_SIZE);
      wrpageOffset++;
      wrBufPtr = wrbuffer;
    }
    *wrBufPtr++ = 0x0A;
    if (wrBufPtr == (wrbuffer + EEPROM_PAGE_SIZE)) {
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      writeTFTPbuffer(wrbuffer, wrpageOffset, EEPROM_PAGE_SIZE);
      wrpageOffset++;
      wrBufPtr = wrbuffer;
    }
    bufferPointer += 4;
    //    bufPtr += 16;
    if (bufPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFO Flash read pagebreak detected, reading last Flash page for converted blacklisted HTTP fragments.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      flashaddr += EEPROM_PAGE_SIZE;
      if (flashaddr == (abusefilter12_addr + EEPROM_PAGE_SIZE)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d INFO end of flash pages for IPtable reached.", hour(), minute(), second(), __LINE__); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        return false;
      }
      flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
      bufPtr = rdbuffer;
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08x", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
  }
  //  }
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    // delay(25);
  }
  writeTFTPbuffer(wrbuffer, wrpageOffset, (wrBufPtr - 1 - wrbuffer)); // write the remaining
  return true;
}

bool prepareHTTP2TFTPbuffer() {
  /*
    rdbuffer is global buffer
    bufPtr = global buffer pointer from rdbuffer
    rxBufPtr = recordpointer in global buffer
    wr
  */
  uint8_t* rxBufPtr;
  uint8_t wrbuffer[EEPROM_PAGE_SIZE]; // size of an EEPROM page
  uint8_t wrpageOffset = 0;
  uint8_t* wrBufPtr = wrbuffer;
  uint16_t recordcnt = 0;
  uint32_t flashaddr = HTTPblacklist1_addr;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start converting HTTP blacklist to ASCII text, preparation for TFTP download", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  // delay(25);
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  bufPtr = rdbuffer;
  while (*bufPtr != 0xFF) {
    while (bufPtr < (rdbuffer + EEPROM_PAGE_SIZE) && (*bufPtr != 0xFF)) {
      rxBufPtr = bufPtr; //temp pointer for reading this entry
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start processing record %d", hour(), minute(), second(), recordcnt); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        //      delay(5);
      }
      while (*rxBufPtr != 0) {  //read record
        *wrBufPtr++ = *rxBufPtr++;
        if (wrBufPtr == (wrbuffer + EEPROM_PAGE_SIZE)) {
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            // delay(25);
          }
          writeTFTPbuffer(wrbuffer, wrpageOffset, EEPROM_PAGE_SIZE);
          wrpageOffset++;
        }
      }
      recordcnt++;
      *wrBufPtr++ = 0x0D;
      if (wrBufPtr == (wrbuffer + EEPROM_PAGE_SIZE)) {
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          // delay(25);
        }
        writeTFTPbuffer(wrbuffer, wrpageOffset, EEPROM_PAGE_SIZE);
        wrpageOffset++;
      }
      *wrBufPtr++ = 0x0A;
      if (wrBufPtr == (wrbuffer + EEPROM_PAGE_SIZE)) {
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          // delay(25);
        }
        writeTFTPbuffer(wrbuffer, wrpageOffset, EEPROM_PAGE_SIZE);
        wrpageOffset++;
      }
      bufferPointer += 16;
      bufPtr += 16;
      if (bufPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFO Flash read pagebreak detected, reading last Flash page for converted blacklisted HTTP fragments.", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        flashaddr += EEPROM_PAGE_SIZE;
        if (flashaddr == (HTTPblacklist2_addr + EEPROM_PAGE_SIZE)) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Flash end of blacklist area reached.", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          return false;
        }
        flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
        bufPtr = rdbuffer;
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
    }
  }
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d writing wrbuffer 0x%08X to flash", hour(), minute(), second(), __LINE__, wrbuffer); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    // delay(25);
  }
  writeTFTPbuffer(wrbuffer, wrpageOffset, (wrBufPtr - 1 - wrbuffer)); // write the remaining
  return true;
}


/*
  Read flash  to prepare transfer the ASCII IPtable or HTTP blacklist back to a host
*/
void prepareTFTPPacket() {
  //  Serial.print("\r\nsingle byte read from 0x0x000B3000: ");
  //  Serial.print(flash.readByte(0xB3000));
  uint32_t flashaddr = eepromEnd_addr + (pageOffset * EEPROM_PAGE_SIZE);
  uint8_t bogusbyte = flash.readByte(flashaddr);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s %s:%04d INFO for bug fixing, read a single byte, single byte read from 0x%08X: 0x%02X", hour(), minute(), second(), FILENAME, __func__, __LINE__, eepromEnd_addr, bogusbyte);
    textlog(temptxtbuff, false);
  }
  //  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Prepare and check readbuffer for sendTFTPPacket", hour(), minute(), second(), flashaddr, pageOffset); // temptxtbuff is max 1024
  //  textlog(temptxtbuff, false);
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  bufPtr = rdbuffer;
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s %s:%04d SUCCESS: Flash read success address: 0x%08X, pageOffset: %d, rdbuffer @bufPtr: 0x%08X: 0x%02X", hour(), minute(), second(), FILENAME, __func__, __LINE__, flashaddr, pageOffset, bufPtr, *bufPtr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  pageOffset++;
}

/*
  Load HTTP blacklist to memory
*/
void convertHTTPlist() {
  bool rdpagebreak = false;
  uint8_t bogusbyte;
  uint8_t HTTPposcnt = 0; // counter for first free position in HTTP blacklisttextfragment
  uint8_t outbuffer[EEPROM_PAGE_SIZE];
  uint8_t* writePtr = outbuffer; //position in text outputarray
  uint8_t* readPtr = rdbuffer; //position off scan point to detect a \r\n position in the textlist
  uint8_t pageoffset = 0; // current memory page write in flash
  uint8_t readoffset = 0;
  uint32_t flashaddr = eepromEnd_addr;
  uint32_t writeFileAddr;
  char HTTPtext[17];  // HTTP blacklisttextfragment+0x00+0x0D, string is max 15 positions

  totalHTTPcmd = 0;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start converting the HTTP text table to memory structure", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  bogusbyte = flash.readByte(flashaddr); // to overcom a library bug
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d outbuffer base is: 0x%08X, actual writePtr in outbuffer: 0x%08X, offset in outbuffer is: 0x%08X", hour(), minute(), second(), outbuffer, writePtr, (writePtr - outbuffer)); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s %s:%04d INFO: for bug fixing, read a single byte, single byte read from 0x%08X: 0x%02X", hour(), minute(), second(), FILENAME, __func__, __LINE__, eepromEnd_addr, bogusbyte);
    textlog(temptxtbuff, false);
  }
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X, sector: %d", hour(), minute(), second(), flashaddr, readoffset); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  while (*readPtr != 0xFF) {
    while ((readPtr < (rdbuffer + EEPROM_PAGE_SIZE)) && (*readPtr != 0x0D)) {
      if ((*readPtr != 0x0A) && (HTTPposcnt < 15)) {
        HTTPtext[HTTPposcnt++] = (char) * readPtr++;
      }
      else {
        readPtr++;
      }
    }

    if (*readPtr == 0x0D) {
      while (HTTPposcnt < 17) {
        HTTPtext[HTTPposcnt++] = 0x00;
      }
      //  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Content of HTTPtext array: %s", hour(), minute(), second(), HTTPtext); // temptxtbuff is max 1024
      //  textlog(temptxtbuff, false);
      readPtr++;  // pointer to next entry in case \r recordformat
      if (*readPtr == 0x0A) {
        readPtr++;  // pointer to next entry in case \r\n recordformat
      }
      // check if we have a valid string
      if ((HTTPtext[0] != '#') && (HTTPtext[0] != 0x00)) {
        totalHTTPcmd++;
        HTTPposcnt = 0;
        while ( HTTPposcnt < 16) {
          *writePtr++ = HTTPtext[HTTPposcnt++];
        }
        HTTPposcnt = 0; // reset for new string
        if (writePtr == (outbuffer + EEPROM_PAGE_SIZE)) {
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Converted blacklisted HTTP fragments: %d", hour(), minute(), second(), totalHTTPcmd); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Adres of HTTPblacklist1_addr: 0x%08X, pageoffset: %d, EEPROM_PAGE_SIZE: 0x%08X", hour(), minute(), second(), HTTPblacklist1_addr, pageoffset, EEPROM_PAGE_SIZE); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          writeFileAddr = HTTPblacklist1_addr + (pageoffset * EEPROM_PAGE_SIZE);
          flash.eraseSector(writeFileAddr);
          delay(25);
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d first entry of blacklisted HTTP fragments in this converted buffer %s at address 0x%08X", hour(), minute(), second(), (char*)outbuffer, outbuffer);
            textlog(temptxtbuff, false);
          }
          if (!flash.writeByteArray(writeFileAddr, outbuffer, EEPROM_PAGE_SIZE)) {
            flash_error = F("writeByteArray writeFileAddr FAIL");
            StoreFlashError(flash_error, writeFileAddr);
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Converted blacklisted HTTP fragments writing... writeByteArray writeFileAddr FAIL addres: 0x%08X", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            return;
          }
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: blacklisted HTTP fragments writeByteArray writeFileAddr success address: 0x%08X, sector: %d", hour(), minute(), second(), writeFileAddr, pageoffset); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          pageoffset++;
          writePtr = outbuffer;
          //        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d outbuffer base is: 0x%08X, writePtr: 0x%08X, offset address is: 0x%08X", hour(), minute(), second(), outbuffer, writePtr, (writePtr - outbuffer)); // temptxtbuff is max 1024
          //        textlog(temptxtbuff, false);
          delay(3);
        }
      }
    }
    if (readPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFO Flash read pagebreak detected, already converted blacklisted HTTP fragments: %d (partial) readed content: %s", hour(), minute(), second(), totalHTTPcmd, HTTPtext); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      flashaddr += EEPROM_PAGE_SIZE;
      readoffset++;
      flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
      readPtr = rdbuffer;
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X, sector: %d", hour(), minute(), second(), flashaddr, readoffset); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      rdpagebreak = true;
    }

  }
  if (writePtr != outbuffer) {
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Total Converted blacklisted HTTP fragments: %d, remaining converted blacklisted HTTP fragments in outputbuffer: %d.", hour(), minute(), second(), totalHTTPcmd, ((writePtr - outbuffer) / 16)); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    writeFileAddr = HTTPblacklist1_addr + (pageoffset * EEPROM_PAGE_SIZE);
    flash.eraseSector(writeFileAddr);
    delay(25);
    if (!flash.writeByteArray(writeFileAddr, outbuffer, (writePtr - outbuffer))) {
      flash_error = F("writeByteArray writeFileAddr FAIL");
      StoreFlashError(flash_error, writeFileAddr);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Converted blacklisted HTTP fragments writing... writeByteArray writeFileAddr FAIL addres: 0x%08X", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      return;
    }
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Converted blacklisted HTTP fragments writeByteArray writeFileAddr success address: 0x%08X, sector: %d", hour(), minute(), second(), writeFileAddr, pageoffset); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    delay(3);
  }
  writePtr -= 16;
  HTTPposcnt = 0;
  while (HTTPposcnt < 16) {
    HTTPtext[HTTPposcnt++] = (char) * writePtr++;
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d HTTP fragments converted: %d, last content of buffer to convert: %s.", hour(), minute(), second(), totalHTTPcmd, HTTPtext); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
}
/*
  read ipaddress and transform to binary table
*/
bool convertIPtable() {
  uint8_t bogusbyte;
  uint8_t outbuffer[EEPROM_PAGE_SIZE];
  uint8_t* writePtr = outbuffer; //position in binary outputarray
  uint8_t* readPtr = rdbuffer; //position off scan point to detect a \r\n position char of ipaddres in text
  uint8_t pageoffset = 0; // current memory page write in flash
  uint8_t readoffset = 0;
  char ipaddrbuffer[17];  // 255.255.255.225+0x0D+0x00
  uint8_t targetIP[4];
  uint8_t ipposcnt = 0; // counter for first free position in ipaddrbuffer
  uint32_t flashaddr = eepromEnd_addr;
  uint32_t writeFileAddr;
  bool rdpagebreak = false;
  totalnets = 0;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start converting the text IPtable to binary table", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  // for unknown reasons, probably timing a read of a byte from flash is necessary to work a reliable flash page read
  bogusbyte = flash.readByte(flashaddr);
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d outbuffer base is: 0x%08X, writePtr: 0x%08X", hour(), minute(), second(), outbuffer, writePtr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s %s:%04d INFO: for bug fixing, read a single byte, single byte read from 0x%08X: 0x%02X, every value is allowed and ok", hour(), minute(), second(), FILENAME, __func__, __LINE__, flashaddr, bogusbyte);
    textlog(temptxtbuff, false);
  }
  if (!flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE) ) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Flash read error address: 0x%08X, sector: %d to readbuffer 0x%08X", hour(), minute(), second(), flashaddr, readoffset, rdbuffer); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  else {
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X, sector: %d to readbuffer 0x%08X", hour(), minute(), second(), flashaddr, readoffset, rdbuffer); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d readbuffer base is: 0x%08X, readPtr: 0x%08X", hour(), minute(), second(), rdbuffer, readPtr); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  while (*readPtr != 0xFF) {
    while ((readPtr < (rdbuffer + EEPROM_PAGE_SIZE)) && (*readPtr != 0x0A)) {
      if (ipposcnt > 16) { // 123.567.890.234d0
        ipaddrbuffer[ipposcnt] = '\0';
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR Trying a write beyond array bound after net: %d failed content: %s, ipposcnt: %d", hour(), minute(), second(), totalnets, ipaddrbuffer, ipposcnt); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        return false;
      }
      if (((*readPtr >= 0x30) && (*readPtr <= 0x39)) || (*readPtr == '.') || (*readPtr == 0x0D)) {
        ipaddrbuffer[ipposcnt] = *readPtr++;
        ipposcnt++;
      }
      else {
        ipaddrbuffer[ipposcnt] = '\0';
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s %04d %s: WARNING: Input file contains illegal at about line %d readPtr: 0x%08X, character value: 0x%02X failed content: %s, ipposcnt: %d", hour(), minute(), second(),
                FILENAME, __LINE__, __func__, totalnets + 1, readPtr, *readPtr,  ipaddrbuffer, ipposcnt + 1); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        int verschil = (uint32_t)readPtr % 16;
        show_buffer_Row2Serial(readPtr - verschil - 16, 48);
        readPtr++;
        //  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d readPtr after dump: 0x%08X", hour(), minute(), second(), __LINE__, readPtr); // temptxtbuff is max 1024
        //  textlog(temptxtbuff, false);
      }
    }
    ipaddrbuffer[ipposcnt] = '\0';
    //    if ((totalnets > 677) && (totalnets < 679)) {
    //      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d net: %d ipaddrbuff: %s", hour(), minute(), second(), __LINE__, totalnets, ipaddrbuffer); // temptxtbuff is max 1024
    //      textlog(temptxtbuff, false);
    //    }
    if (rdpagebreak) {
      bool nltrue = false;
      if (ipaddrbuffer[ipposcnt - 1] == 0x0D) {
        // this is normal condition
        //mark we have modified for output
        nltrue = true;
        ipaddrbuffer[ipposcnt - 1] = '\0';
      }
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d Supplemented readed content after pagebreak: %s", hour(), minute(), second(), __LINE__, ipaddrbuffer); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      if (nltrue) {
        ipaddrbuffer[ipposcnt - 1] = 0x0D;
      }
      rdpagebreak = false;
    }
    if (readPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
      bool nltrue = false;
      if (ipaddrbuffer[ipposcnt - 1] == 0x0D) {
        // this is a coincedence condition
        //mark we have modified for output
        nltrue = true;
        ipaddrbuffer[ipposcnt - 1] = '\0';
      }
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d INFO: Flash read pagebreak detected, already converted nets: %d (partial) readed content: %s", hour(), minute(), second(), __LINE__, totalnets, ipaddrbuffer); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      if (nltrue) {
        //restore the convertbuffer
        ipaddrbuffer[ipposcnt - 1] = 0x0D;
      }
      flashaddr += EEPROM_PAGE_SIZE;
      readoffset++;
      flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
      readPtr = rdbuffer;
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08X, sector: %d", hour(), minute(), second(), flashaddr, readoffset); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      rdpagebreak = true;
    }
    if (ipaddrbuffer[ipposcnt - 1] == 0x0D) {
      // check if we have a valid string
      int prdcnt = 0; // number of '.'
      int cpos = 0;
      char btip[0];
      for (int sgm = 0; sgm < ipposcnt; sgm++) {
        //        if ((totalnets > 677) && (totalnets < 679)) {
        //          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d Value readed from rdbuffer: %c => 0x%02x", hour(), minute(), second(), __LINE__, ipaddrbuffer[sgm], ipaddrbuffer[sgm]); // temptxtbuff is max 1024
        //          textlog(temptxtbuff, false);
        //        }
        if ((ipaddrbuffer[sgm] != '.') && (ipaddrbuffer[sgm] != 0x0D)) { // eventueel hier ook checken op cijfer
          if (cpos < 3) {
            btip[cpos] = ipaddrbuffer[sgm];
            btip[cpos + 1] = '\0';
          }
          else {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s:%04d ERROR: Trying a write beyond array bound after net: %d failed content: %s, netpart: %s, cpos: %d", hour(), minute(), second(), __func__, __LINE__, totalnets, ipaddrbuffer, btip, cpos); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          cpos++;
        }
        else {
          targetIP[prdcnt] = atoi(btip);
          //          if ((totalnets > 677) && (totalnets < 679)) {
          //            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d net: %d Value to convert btip: %s converted to: %d", hour(), minute(), second(), __LINE__, totalnets, btip, targetIP[prdcnt]); // temptxtbuff is max 1024
          //            textlog(temptxtbuff, false);
          //          }
          cpos = 0;
          if (ipaddrbuffer[sgm] != 0x0D) {
            prdcnt++;
          }
          // modify th convertbuffer to a normal c_string
          if  (ipaddrbuffer[sgm] == 0x0D) {
            ipaddrbuffer[sgm] = '\0';
          }
        }
      }
      if (prdcnt != 3) {
        sprintf(temptxtbuff, "\r\nERROR: converting text IPtable to binary, period count is %d, last buffercontent: %s", prdcnt, ipaddrbuffer); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        return false;
      }
      if (*readPtr == 0x0A) {
        readPtr++;  // pointer to next entry in case \r\n recordformat
      }
      else {
        ipaddrbuffer[ipposcnt] = '\0';
        sprintf(temptxtbuff, "\r\nERROR: converting text IPtable to binary, 0x0A not found after %d nets, last converted net: %s", totalnets, ipaddrbuffer); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      ipposcnt = 0;
      totalnets++;
      for (int wc = 0; wc < 4; wc++) {
        *writePtr++ = targetIP[wc];
      }
      if (writePtr == (outbuffer + EEPROM_PAGE_SIZE)) {
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d Converted nets to 4 bytes: %d", hour(), minute(), second(), __LINE__, totalnets); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        writeFileAddr = abusefilter1_addr + (pageoffset * EEPROM_PAGE_SIZE);
        flash.eraseSector(writeFileAddr);
        delay(25);
        //        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d first address of net in this converted buffer %d %d %d %d", hour(), minute(), second(), outbuffer[0], outbuffer[1], outbuffer[2], outbuffer[3]);
        // textlog(temptxtbuff, false);
        if (!flash.writeByteArray(writeFileAddr, outbuffer, EEPROM_PAGE_SIZE)) {
          flash_error = F("writeByteArray writeFileAddr FAIL");
          StoreFlashError(flash_error, writeFileAddr);
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: ConvertIPtable writing... writeByteArray writeFileAddr FAIL addres: 0x%08X", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          return false;
        }
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: ConvertIPtable writeByteArray writeFileAddr success addres: 0x%08X, sector: %d", hour(), minute(), second(), writeFileAddr, pageoffset); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        pageoffset++;
        writePtr = outbuffer;
        //        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d outbuffer base is: 0x%08X, writePtr: 0x%08X, offset address is: 0x%08X", hour(), minute(), second(), outbuffer, writePtr, (writePtr - outbuffer)); // temptxtbuff is max 1024
        //        textlog(temptxtbuff, false);
        delay(3);
      }
    }
  }
  if (writePtr != outbuffer) {
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Total nets to 4 bytes: %d, remaining converted  nets in outputbuffer: %d.", hour(), minute(), second(), totalnets, ((writePtr - outbuffer) / 4)); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    writeFileAddr = abusefilter1_addr + (pageoffset * EEPROM_PAGE_SIZE);
    flash.eraseSector(writeFileAddr);
    delay(25);
    if (!flash.writeByteArray(writeFileAddr, outbuffer, (writePtr - outbuffer))) {
      flash_error = F("writeByteArray writeFileAddr FAIL");
      StoreFlashError(flash_error, writeFileAddr);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: ConvertIPtable writing... writeByteArray writeFileAddr FAIL addres: 0x%08X", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      return false;
    }
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: ConvertIPtable writeByteArray writeFileAddr success addres: 0x%08X, sector: %d", hour(), minute(), second(), writeFileAddr, pageoffset); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    delay(3);
  }
  /*
    // remove the <CR> from the addres, should be done by conversion above
    for (ipposcnt = 7; ipposcnt < 17, ipposcnt++) {
      if (ipaddrbuffer[ipposcnt] == 0x0D) {
        ipaddrbuffer[ipposcnt] = '\0';
        break;
      }
    }
  */
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Last content of buffer to convert: %s, nets converted: %d", hour(), minute(), second(), ipaddrbuffer , totalnets); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  return true;
}

uint8_t processPacket(uint16_t packetSize) {
  char remfilename[32];
  char* remfilenamePtr = remfilename;
  uint8_t returnCode = ERROR_UNKNOWN;
  uint16_t count;
  uint16_t tftpBlock;
  uint16_t UDPPacketSize;
  uint32_t writeFileAddr;
  String remfilestring;
  if (InExlog) {
    textlog("\r\n-------------------------Starting ProcessPacket---------------------------------", false);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Bytes in buffer: %d", hour(), minute(), second(), packetSize); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    // delay(25);
  }
  if (packetSize > 0x0204) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Packetbuffer OVERFLOW! packetSize = 0x%04X", hour(), minute(), second(), packetSize); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    //drain packet
    for (count = packetSize; count--;) {
      TFTPclient.read(); //Read value from W5100
    }
    returnCode = ERROR_LARGE;
    return returnCode;
  }
  //  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d write Pointer: %d", hour(), minute(), second(), bufferPointer); // temptxtbuff is max 1024
  //  textlog(temptxtbuff, false);
  uint16_t tftpOpcode = (TFTPclient.read() << 8) + TFTPclient.read();
  // Parse packet
  if ((tftpOpcode == 1) || (tftpOpcode == 2)) {
    tftpBlock = 0;
    UDPPacketSize = packetSize - 2;
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start TFTP for %s.", hour(), minute(), second(), (tftpOpcode == 1) ? "download" : "upload"); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    SerInpRcdv = true;
  }
  else {
    tftpBlock = (TFTPclient.read() << 8) + TFTPclient.read();
    UDPPacketSize = packetSize - 4;
  }
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Packet datagram size received: %d", hour(), minute(), second(), UDPPacketSize); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  if (UDPPacketSize > 0 ) {
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d This block: Opcode = %d block n°: %d, datasize: %d bytes, bufferPointer: %d in rdbuffer 0x%08X with bufPTR 0x%08X", hour(), minute(), second(), __LINE__, tftpOpcode, tftpBlock, UDPPacketSize, bufferPointer, rdbuffer, bufPtr); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      // delay(25);
    }
    //Read whole packet
    for (count = UDPPacketSize; count--;) {
      *bufPtr++ = TFTPclient.read(); //Read value from W5100
      bufferPointer++;
    }
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d datasize: %d bytes, Write bufferPointer: %d in rdbuffer 0x%08X with bufPTR 0x%08X", hour(), minute(), second(), __LINE__, UDPPacketSize, bufferPointer, rdbuffer, bufPtr); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      // delay(25);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d readbufferPointer 0x%08X, Write bufferPointer after processing: %d", hour(), minute(), second(), bufPtr, bufferPointer); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      // delay(25);
    }
    // Dump packet
    //bufPtr = rdbuffer;
    //Serial.print("\r\nContent of packet is: ");
    //for (count = 0; count < UDPPacketSize; count++) {
    //  uint8_t bval = *bufPtr++;
    //      if (count % 16 == 0) {
    //sprintf(temptxtbuff, "\r\n%02d:%02d:%02d content of bufPtr position 0x%08X = 0x%02x => %c", hour(), minute(), second(), bufPtr, *bufPtr, *bufPtr); // temptxtbuff is max 1024
    //textlog(temptxtbuff, false);
    //delay(25);
    //      }
  }

  switch (tftpOpcode) {
    case TFTP_OPCODE_RRQ: // Read request
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d 0x%04X is RRQ opcode, packet %04d, reading header for filename parameter", hour(), minute(), second(), tftpOpcode, tftpBlock); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      bufferPointer = 0;
      bufPtr -= UDPPacketSize; // restore to the start of data or info at WRQ or RRQ
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d datasize: %d bytes, bufferPointer: %d in rdbuffer 0x%08X with bufPTR 0x%08X", hour(), minute(), second(), __LINE__, UDPPacketSize, bufferPointer, rdbuffer, bufPtr); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      while ((bufferPointer < 32) && (*bufPtr != 0x00)) {
        //remfilename[bufferPointer] = *bufPtr++;
        *remfilenamePtr++ = *bufPtr++;
        bufferPointer++;
        *remfilenamePtr = 0x00;
      }
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d filename in request readed: %s", hour(), minute(), second(), __LINE__, remfilename); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      remfilestring = String(remfilename);

      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d filename converted to String: %s", hour(), minute(), second(), remfilestring.c_str()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      if (remfilestring.indexOf("HTTP")  != -1) {
        outputHTTPlistReq = true;
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d download for HTTP blacklist file: %s request detected", hour(), minute(), second(), remfilename); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
        if (!prepareHTTP2TFTPbuffer()) {
          outputHTTPlistReq = false;
        }
      }
      else if (remfilestring.indexOf("IPtable")  != -1) {
        outputIPtableReq = true;
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d download for IPtable request detected", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
        if (!prepareIPtable2TFTPbuffer()) {
          outputIPtableReq = false;
        }
      }
      remoteTFTPport = TFTPclient.remotePort();
      TFTPclient.stop();
      //      delay(25);
      localTFTPport = random(49152, 65535);
      TFTPclient.begin(localTFTPport);
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Readrequest(RRQ)  Remote port: %d, Local port changed to %d", hour(), minute(), second(), remoteTFTPport, localTFTPport); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      bufferPointer = 0; //initialize pointer
      lastPacket = 0;
      pageOffset = 0; // EEPROM page offset
      //      totalnets = 0;
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Preparing for returncode", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      if (!outputHTTPlistReq && !outputIPtableReq) {
        returnCode = INVALID_IMAGE;
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d returncode = INVALID_IMAGE", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          // delay(25);
        }
      }
      else {
        returnCode = SEND_DATA; // create the first data packet
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d returncode = SEND_DATA", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          // delay(25);
        }
      }
      break;

    case TFTP_OPCODE_WRQ: // Write request
      remoteTFTPport = TFTPclient.remotePort();
      TFTPclient.stop();
      localTFTPport = random(49152, 65535);
      TFTPclient.begin(localTFTPport);
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Writerequest(WRQ) Remote port: %d, Local port changed to %d", hour(), minute(), second(), remoteTFTPport, localTFTPport); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      bufferPointer = 0; //initialize pointer
      bufPtr = rdbuffer;
      //    bufPtr -= UDPPacketSize; // restore to the start of data or info at WRQ or RRQ
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d datasize: %d bytes, bufferPointer: %d in rdbuffer 0x%08X with bufPTR 0x%08X", hour(), minute(), second(), __LINE__, UDPPacketSize, bufferPointer, rdbuffer, bufPtr); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      while ((*bufPtr != 0) && (bufferPointer < 32)) {
        //        remfilename[bufferPointer] = (char) * bufPtr++;
        //        bufferPointer++;
        //        remfilename[bufferPointer] = 0x00;
        *remfilenamePtr++ = *bufPtr++;
        bufferPointer++;
        *remfilenamePtr = 0x00;
      }

      //      remfilestring = String((char*)remfilename);
      remfilestring = String(remfilename);

      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d filename converted to String: %s", hour(), minute(), second(), remfilestring.c_str()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      if (remfilestring.indexOf("HTTP") != -1) {
        convertHTTPlistReq = true;
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d HTTP blacklist request detected", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else if (remfilestring.indexOf("IPtable")  != -1) {
        convertIPtableReq = true;
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d IPtable request detected", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      bufferPointer = 0; //initialize pointer
      bufPtr = rdbuffer;
      lastPacket = 0;
      pageOffset = 0; // EEPROM page offset
      totalnets = 0;
      TFTPclient.begin(localTFTPport);
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Readrequest(RRQ)  Remote port: %d, Local port changed to %d", hour(), minute(), second(), remoteTFTPport, localTFTPport); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
      }
      if (!convertHTTPlistReq && !convertIPtableReq) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: TFTP unsupported filename request.", hour(), minute(), second(), remoteTFTPport, localTFTPport); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        // delay(25);
        returnCode = INVALID_IMAGE;
      }
      returnCode = ACK; // Send back acknowledge for packet 0
      break;

    case TFTP_OPCODE_DATA:
      lastPacket = tftpBlock;
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Receiving Data for block n°: %d", hour(), minute(), second(), lastPacket); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      if ((writeFileAddr + UDPPacketSize) > EEPROM_MAX_ADDR) {
        // Flash is full - abort with an error before a bootloader overwrite occurs
        //
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: TFTP (Flash)memory is full!", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        returnCode = ERROR_FULL;
      }
      else {
        IPAddress segm = Ethernet.localIP();
        // First check sender - validate
        if ((UDPSendRxTFTPTo[0] != segm[0]) && (UDPSendRxTFTPTo[1] != segm[1])) {
          returnCode = INVALID_HOST;
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: TFTP Invalid Host! Remote addres is not in local subnet", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          break;
        }
        if ((bufferPointer == EEPROM_PAGE_SIZE) || (UDPPacketSize < TFTP_DATA_SIZE)) {
          //          Serial.println();
          //          for (int ixc = 0; ixc < EEPROM_PAGE_SIZE; ixc++) {
          //            Serial.print((char)rdbuffer[ixc]);
          //          }
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP components for calculating writeFileAddr: eepromEnd_addr: 0x%08X, pageOffset: %d, EEPROM_PAGE_SIZE: 0x%08X", hour(), minute(), second(), eepromEnd_addr, pageOffset, EEPROM_PAGE_SIZE); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          writeFileAddr = eepromEnd_addr + (pageOffset * EEPROM_PAGE_SIZE); // Flash write address for first block
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP writeFileAddress pointer is %d = 0x%08X should be > %d", hour(), minute(), second(), writeFileAddr, writeFileAddr, eepromEnd_addr); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          // Flash packets
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP writing... 0x%04X bytes to Flash", hour(), minute(), second(), bufferPointer); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          if (!flash.eraseSector(writeFileAddr)) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Flash erase error... for address 0x08X", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            flash_error = F("Erase writeFileAddr FAIL");
            StoreFlashError(flash_error, writeFileAddr);
            returnCode = FLASH_ERROR;
            break;
          }
          else {
            if (InExlog) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash erase address 0x%08X completed", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
            }
          }
          delay(25);
          if (!flash.writeByteArray(writeFileAddr, rdbuffer, bufferPointer)) {
            flash_error = F("writeByteArray writeFileAddr FAIL");
            StoreFlashError(flash_error, writeFileAddr);
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: TFTP writing... writeFileAddr addres: 0x%08X, length: ", hour(), minute(), second(), writeFileAddr, bufferPointer); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            returnCode = FLASH_ERROR;
            break;
          }
          else {
            if (InExlog) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: TFTP writeByteArray writeFileAddr success addres: 0x%08X", hour(), minute(), second(), writeFileAddr); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
            }
          }
          delay(3);
          uint8_t rbb = flash.readByte((writeFileAddr + EEPROM_PAGE_SIZE));
          if (InExlog) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Flash readback byte of 1st address of next flash page: 0x%08X is: 0x%02X => %d", hour(), minute(), second(), writeFileAddr + EEPROM_PAGE_SIZE, rbb, rbb); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          //          if ((bufferPointer == EEPROM_PAGE_SIZE) && ((writeFileAddr + EEPROM_PAGE_SIZE) < EEPROM_MAX_ADDR)) { // be sure that the next page contains 0xFF
          if ((UDPPacketSize < TFTP_DATA_SIZE) && ((writeFileAddr + EEPROM_PAGE_SIZE) < EEPROM_MAX_ADDR)) {
            if (!flash.eraseSector((writeFileAddr + EEPROM_PAGE_SIZE))) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Flash erase error... for address 0x08X", hour(), minute(), second(), writeFileAddr + EEPROM_PAGE_SIZE); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
              flash_error = F("Erase writeFileAddr FAIL");
              StoreFlashError(flash_error, writeFileAddr);
              returnCode = FLASH_ERROR;
              break;
            }
            delay(25);
            if (InExlog) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash erase address 0x%08X completed", hour(), minute(), second(), writeFileAddr + EEPROM_PAGE_SIZE); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
            }
          }
          bufferPointer = 0; //If end of pointer reached, initialize pointer
          pageOffset++;
          bufPtr = rdbuffer;
        }

        /*
          Read back the flash flash momory
        */
        //        show_Flash_Row2Serial(eepromEnd_addr);

        if ((UDPPacketSize) < TFTP_DATA_SIZE) {
          // Flash is complete
          // Hand over to EEPROM
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP transfer is complete.", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          returnCode = FINAL_ACK;
        } else {
          returnCode = ACK;
        }
      }
      break;

    case TFTP_OPCODE_ACK: // Acknowledgement
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Ack", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      //send a new packet
      if (!EndOfTransmission)
        returnCode = SEND_DATA;
      else
        returnCode = ACK_EOT;
      break;

    case TFTP_OPCODE_ERROR: // Error signal
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d TFTP opcode Error", hour(), minute(), second(), __LINE__); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      break;

    default:
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s %s:%04d ERROR: Invalid opcode 0x%04X, not supported for packet %04d", hour(), minute(), second(), FILENAME, __func__, __LINE__, tftpOpcode, tftpBlock); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      // Invalid - return error
      returnCode = ERROR_INVALID;
      break;

  }
  return returnCode;
}

void sendResponse(uint16_t response) {
  uint8_t txBuffer[TFTP_PACKET_MAX_SIZE + 1];
  uint8_t *txPtr = txBuffer;
  uint16_t packetLength = 0;

  switch (response) {
    default:
    case ERROR_UNKNOWN:
      // Send unknown error packet
      packetLength = TFTP_UNKNOWN_ERROR_LEN;
      memcpy_P(txBuffer, tftp_unknown_error_packet, packetLength);
      break;

    case INVALID_IMAGE:
      // Send bad image packet
      packetLength = TFTP_INVALID_IMAGE_LEN;
      memcpy_P(txBuffer, tftp_invalid_image_packet, packetLength);
      break;

    case INVALID_HOST:
      // Send forbidden host packet
      packetLength = TFTP_INVALID_HOST_LEN;
      memcpy_P(txBuffer, tftp_invalid_host_packet, packetLength);
      break;

    case ERROR_INVALID:
      // Send invalid opcode packet
      packetLength = TFTP_OPCODE_ERROR_LEN;
      memcpy_P(txBuffer, tftp_opcode_error_packet, packetLength);
      break;

    case ERROR_FULL:
      // Send unknown error packet
      packetLength = TFTP_FULL_ERROR_LEN;
      memcpy_P(txBuffer, tftp_full_error_packet, packetLength);
      break;

    case ERROR_LARGE:
      // Received too large packet
      packetLength = TFTP_LARGE_ERROR_LEN;
      memcpy_P(txBuffer, tftp_too_large_packet, packetLength);
      break;
    case FLASH_ERROR:
      // Error write packet to flash TFTPbuffer
      packetLength = TFTP_FLASH_ERROR_LEN;
      memcpy_P(txBuffer, tftp_flash_error_packet, packetLength);
      break;

    case ACK:
      if (InExlog) {
        textlog("\r\nSend ACK (Not Final)", false);
      }
      packetLength = 4;
      *txPtr++ = TFTP_OPCODE_ACK >> 8;
      *txPtr++ = TFTP_OPCODE_ACK & 0xff;
      *txPtr++ = lastPacket >> 8; //lastPacket is block code
      *txPtr = lastPacket & 0xff;
      break;
    case FINAL_ACK:
      if (InExlog) {
        textlog("\r\nSend FINAL_ACK ", false);
      }
      DownloadStarted = 0;
      packetLength = 4;
      *txPtr++ = TFTP_OPCODE_ACK >> 8;
      *txPtr++ = TFTP_OPCODE_ACK & 0xff;
      *txPtr++ = lastPacket >> 8; //lastPacket is block code
      *txPtr = lastPacket & 0xff;
      break;
    case SEND_DATA:
      lastPacket++;
      *txPtr++ = TFTP_OPCODE_DATA >> 8;
      *txPtr++ = TFTP_OPCODE_DATA & 0xff;
      *txPtr++ = lastPacket >> 8; //lastPacket is block code
      *txPtr++ = lastPacket & 0xff;
      if ((lastPacket % 8) == 1) {
        prepareTFTPPacket();
      }
      while ( (*bufPtr != 0xFF) && (packetLength < TFTP_DATA_SIZE) ) {
        *txPtr++ = *bufPtr++;
        packetLength++;
      }
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d TFTP SEND_DATA Packetno: %d Packetlength: %d", hour(), minute(), second(), lastPacket, packetLength); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        //      delay(25);
      }
      if (packetLength < 512) {
        EndOfTransmission = true;
        if (InExlog) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d TFTP end of transmission flag set.", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          //        delay(25);
        }
      }
      if (InExlog) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %04d TFTP packet %d containing %d bytes sent.", hour(), minute(), second(), __LINE__, lastPacket, packetLength); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        //      delay(25);
      }
      packetLength += 4;
      break;
  }
  txPtr = txBuffer;
  TFTPclient.beginPacket(UDPSendRxTFTPTo, remoteTFTPport); //NTP requests are to port 123
  TFTPclient.write(txPtr, packetLength);
  TFTPclient.endPacket();
  if (InExlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Response has sent.", hour(), minute(), second()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
}

void tftpInit(EthernetClient client) {
  //  if (client != NULL) {
  UDPSendRxTFTPTo = client.remoteIP();
  IPAddress segm = Ethernet.localIP();
  if ((UDPSendRxTFTPTo[0] == segm[0]) && (UDPSendRxTFTPTo[1] == segm[1]) && (UDPSendRxTFTPTo[2] <= 16)) {
    tftpReqEna = true;
    TFTPclient.begin(TFTP_PORT);
    if (InExlog) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP socket started. Listening for UDP on port %d.", hour(), minute(), second(), TFTP_PORT); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    client << (F("<p>UDP TFTP client started listening on port: ")) << TFTP_PORT <<  (F("<br>\r\n"));
    sprintf(temptxtbuff, "\r\nTFTP has started from remote ip: %d.%d.%d.%d", UDPSendRxTFTPTo[0], UDPSendRxTFTPTo[1], UDPSendRxTFTPTo[2], UDPSendRxTFTPTo[3]);  // temptxtbuff is max 1024
    if (InExlog) {
      textlog(temptxtbuff, false);
    }
    client << temptxtbuff <<  (F("<br>\r\n"));
    bufferPointer = 0; //initialize pointer
    lastPacket = 0;
    pageOffset = 0; // EEPROM page offset
    bufPtr = rdbuffer;
    convertHTTPlistReq = false;
    convertIPtableReq = false;
    outputHTTPlistReq = false;
    outputIPtableReq = false;
    EndOfTransmission = false;
    DownloadStarted = 0;
    tick = 0; //Reset tick counter
  }
  else {
    client << ("<p>You are not entitled to start TFTP listener from a remote network<br>\r\n<br>\r\n");
    client << ("Request denied!");
    sprintf(temptxtbuff, "\r\nERROR TFTP request denied, due to remote network request from remote ip: %d.%d.%d.%d", UDPSendRxTFTPTo[0], UDPSendRxTFTPTo[1], UDPSendRxTFTPTo[2], UDPSendRxTFTPTo[3]);  // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  client << F("</p><br>");
  client << (F("<br>\r\n</body>\r\n</html>"));
}

uint8_t tftpPoll() {
  uint8_t response = ACK;
  uint16_t packetSize = TFTPclient.parsePacket();
  if (packetSize) {
    UDPSendRxTFTPTo = TFTPclient.remoteIP();
    response = processPacket(packetSize); //Process Packet and get TFTP response code
    if (!EndOfTransmission) {
      sendResponse(response); //send the response
      DownloadStarted = 1;
      tick = 0; //Reset tick counter
      if ((response == FINAL_ACK)) {
        TFTPclient.stop();
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d TFTP Complete, remoteport %d, localport %d, TFTP server ended -> UDP socket for TFTP is closed!", hour(), minute(), second(), remoteTFTPport, localTFTPport); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        tftpReqEna = false;
        if (convertIPtableReq) {
          delay(30);
          convertIPtable();
          convertIPtableReq = false;
        }
        else {
          convertHTTPlist();
          convertHTTPlistReq = false;
        }
        SerInpRcdv = true;
        return 0; //Complete
      }
    }
    else {
      EndOfTransmission = false;
      TFTPclient.stop();
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Download completed. TFTP server ended.", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      DownloadStarted = 0;
      //      tftpInit(NULL);// Restart TFTP
      tftpReqEna = false;
      SerInpRcdv = true;
      return 0; // tftp UDP stop
    }
  }

  //Will timeout if download has failed
  if (timedOut() && DownloadStarted) {
    TFTPclient.stop();
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Download interrupted, TFTP server stopped....", hour(), minute(), second()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    DownloadStarted = 0;
    //    tftpInit(NULL);// Restart TFTP
    SerInpRcdv = true;
    return 0; // tftp stops
  }

  //Will timeout if no download is in progress
  if (timedOut() && !DownloadStarted ) {
    TFTPclient.stop();
    textlog("\r\nTimeout! TFTP server stopped.", false);
    SerInpRcdv = true;
    return 0; // Complete and start app

  }
  return 1; // tftp continues
}
