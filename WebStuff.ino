//char suspinBuff[512];
//int suspBufferIndex = 0;

// max command length = 31 chars

const char* const cmd_string_table[] = {"saveyield", "save", "RAMyield", "GET / HTTP/", "erase", "ntp", "help", "showiplog", "showversion", "fandefault",
                                        "fan", "showsockets", "showPVIdebug", "showPVOstring", "Seriallogging", "showbufferremaining", "showPluvioURL", "GoUpdateDNS", "UpdateDNSKey", "Ethernetlogging",
                                        "showmem", "yield", "forceEmail", "showlogging", "showWeatherlog", "showPVOutputlog", "showip", "showmaillog", "UDPon", "UDPoff",
                                        "showHTTPlogging", "PluvioEna", "showinverterlog", "showbrlog", "showextended", "showsuspectURL", "WolfSSLmail", "showabuslog", "showunknowncmd", "dump",
                                        "reportunkwncmd", "SuspEmail", "UnknownCmdEmail", "setBrCorrFact", "startTFTP", "testIPwhitelist", "testHTTPblacklist"
                                       };
#define numcmd_string_table (int)sizeof(cmd_string_table) / sizeof(cmd_string_table[0])  //array size

const char* const icon_string_table[] = {"zonnig", "bliksem", "regen", "buien", "hagel", "mist", "sneeuw", "bewolkt", "halfbewolkt", "zwaarbewolkt", "nachtmist", "helderenacht", "wolkennacht", "lichtbewolkt", "nachtbewolkt"};
#define numicon_string_table (int)sizeof(icon_string_table) / sizeof(icon_string_table[0])

const char* br = "<br>";

long httpShown = 1;
int httphacks = 0;

uint8_t ipreadvalueshown[4];
int readvalueshown;
time_t timereadvalueshown = 0;
uint8_t iptesthttpblacklistshown[4];
int testhttpblacklistshown = 0;
time_t timetesthttpblacklistshown = 0;
uint8_t iptestipwhitelistshown[4];
int testipwhitelistshown = 0;
time_t timetestipwhitelistshown = 0;
uint8_t ipstarttftpshown[4];
int  starttftpshown = 0;
time_t timestarttftpshown = 0;
uint8_t ipsetbrcorrfactshown[4];
int setbrcorrfactshown = 0;
time_t timesetbrcorrfactshown = 0;
uint8_t iperasecountersshown[4];
int erasecountersshown = 0;
time_t timeerasecountersshown = 0;
uint8_t ipfandefaultshown[4];
int fandefaultshown = 0;
time_t timefandefaultshown = 0;
uint8_t ipwolfsslmailswitchshown[4];
int wolfsslmailswitchshown = 0;
time_t timewolfsslmailswitchshown = 0;
uint8_t ipshowfanshown[4];
int showfanshown = 0;
time_t timeshowfanshown = 0;
uint8_t ipswitchloggingshown[4];
int switchloggingshown = 0;
time_t timeswitchloggingshown = 0;
uint8_t ipshowpvoutputlogshown[4];
int showpvoutputlogshown = 0;
time_t timeshowpvoutputlogshown = 0;
uint8_t ipshowmailloggingshown[4];
int showmailloggingshown = 0;
time_t timeshowmailloggingshown = 0;
uint8_t ipshowiplogshown[4];
int showiplogshown = 0;
time_t timeshowiplogshown = 0;
uint8_t ipshowweatherlogshown[4];
int showweatherlogshown = 0;
time_t timeshowweatherlogshown = 0;
uint8_t ipsaveyieldshown[4];
int saveyieldshown = 0;
time_t timesaveyieldshown = 0;
uint8_t ipsavevalueshown[4];
int savevalueshown = 0;
time_t timesavevalueshown = 0;
uint8_t ipshowipshown[4];
int showipshown = 0;
time_t  timeshowipshown = 0;
uint8_t ipshowversionshown[4];
int showversionshown = 0;
time_t timeshowversionshown = 0;
uint8_t ipsendRobotstxtshown[4];
int sendRobotstxtshown = 0;
time_t timesendRobotstxtshown = 0;
uint8_t ipsend404shown[4];
int send404shown = 0;
time_t timesend404shown = 0;
uint8_t ipdumpyieldshown[4];
int dumpyieldshown = 0;
time_t timedumpyieldshown = 0;
uint8_t ipflash_serial_dump_tableshown[4];
int flash_serial_dump_tableshown = 0;
time_t timeflash_serial_dump_tableshown = 0;
uint8_t ipupdatetimewebshown[4];
int updatetimewebshown = 0;
time_t timeupdatetimewebshown = 0;
uint8_t iphelpshown[4];
int helpshown = 0;
time_t timehelpshown = 0;
uint8_t ipshowpvostringshown[4];
int showpvostringshown = 0;
time_t timeshowpvostringshown = 0;
uint8_t ipshowunknowncmdshown[4];
int showunknowncmdshown = 0;
time_t timeshowunknowncmdshown = 0;
uint8_t ipshowsuspecturlshown[4];
int showsuspecturlshown = 0;
time_t timeshowsuspecturlshown = 0;
uint8_t ipshowstatusshown[4];
int showstatusshown = 0;
time_t timeshowstatusshown = 0;
uint8_t ipshowstatusextshown[4];
int showstatusextshown = 0;
time_t timeshowstatusextshown = 0;
uint8_t ipshowpvidebugshown[4];
int showpvidebugshown = 0;
time_t timeshowpvidebugshown = 0;
uint8_t ipshowpluviourlshown[4];
int showpluviourlshown = 0;
time_t timeshowpluviourlshown = 0;
uint8_t ipgoupdatednsshown[4];
int goupdatednsshown = 0;
time_t timegoupdatednsshown = 0;
uint8_t ipupdatednskeyshown[4];
int updatednskeyshown = 0;
time_t timeupdatednskeyshown = 0;
uint8_t ipethernetloggingshown[4];
int ethernetloggingshown = 0;
time_t timeethernetloggingshown = 0;
uint8_t ipshowbufferremainingshown[4];
int showbufferremainingshown = 0;
time_t timeshowbufferremainingshown = 0;
uint8_t ipshow_loggingshown[4];
int show_loggingshown = 0;
time_t timeshow_loggingshown = 0;
uint8_t ipdisplay_memoryinfoshown[4];
int display_memoryinfoshown = 0;
time_t timedisplay_memoryinfoshown = 0;
uint8_t ipforceemailshown[4];
int forceemailshown = 0;
time_t timeforceemailshown = 0;
uint8_t ipshowsockstatusshown[4];
int showsockstatusshown = 0;
time_t timeshowsockstatusshown = 0;


time_t httphacksTime = 0;
int httpbuffoverflow = 0;
time_t httpbuffoverflowTime = 0;


void printprintable(char c) {
  char cbuff[4];
  cbuff[0] = c;
  cbuff[1] = '\0';
  if (((byte)c > 0x1F) &&  ((byte)c < 0x7F)) {
    textlog(cbuff, false);
  }
  else {
    sprintf(temptxtbuff, " 0x%02X ", (byte)c);
    textlog(temptxtbuff, false);
  }
}

bool isIPself(IPAddress reqIP) {
  IPAddress segm = Ethernet.localIP();
  for (int iip = 0; iip < 3; iip++) {
    if ((iip == 0) || (iip == 1)) {
      if (reqIP[iip] != segm[iip]) {
        return false;
      }
    }
    /*
       Comment the next conditional out if all public class C networks are accepted as local.Now 192.168.17 and larger are noted.
    */
    if (iip == 2) {
      if (reqIP[iip] > 16) {
        return false;
      }
    }
  }
  return true;
}

String HTTPlineEncode(const char *src, int CALen) {
  const char *hexarray = "0123456789ABCDEF";
  String target = "";
  int charcnt = 0;
  int charpos = 0;
  //  textlog("\r\ncharacter in bewerking: ", false);
  while ((charpos < CALen) && (charcnt < 272))
  {
    //    Serial.print(*src);
    //    if (('a' <= *src && *src <= 'z') || ('A' <= *src && *src <= 'Z') || ('0' <= *src && *src <= '9') || *src == '-' || *src == '_' || *src == '.' || *src == '~')
    if (' ' <= *src && *src <= '~')
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
    charpos++;
    //    textlog("\r\nnext char: ", false);
  }
  if (!min_serial && !HTTPlog) {
    Serial.print("\r\nconverted string: ");
    Serial.print(target);
  }
  return target;
}

void Send_header(EthernetClient client) {
  client << (F("HTTP/1.1 200 OK\r\n"));
  client << (F("Content-Type: text/html; charset=utf-8\r\n"));
  client << (F("Connection: close\r\n\r\n"));

  client << F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\r\n");

  client << (F("<html>\r\n")) << (F("<head>\r\n"));

  client << (F("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\r\n"));
  client << (F("<meta name=\"robots\" content=\"noindex,nofollow\">\r\n"));
  client << (F("<link rel=\"icon\" type =\"image/x-icon\" href=\"/favicon.ico\">\r\n"));

  client << (F("<style type=\"text/css\">\r\n"));
  //  client << (F("body {padding:8;text-align:center;word-wrap:break-word;}\r\n"));
  client << (F("body {padding:8;text-align:center;word-wrap:break-word;}\r\n"));
  client << (F("pre {font-family:courier, \"courier new\", monospace; font-size:1em; color:#000; background-color:#fff;}\r\n"));
  client << (F("</style>\r\n"));

  client << (F("<title>Zonocta PVI - Aurora Power One 3.6 </title>\r\n"));
  //  client << (F("</head>\r\n")) << (F("<body>\r\n"));
  client << (F("</head>\r\n")) << (F("<body>\r\n"));
}

void sendHTMLheaderwarning(EthernetClient client) {
  client << (F("Unauthorized access prohibited. You have no business here.\r\nUsername: \r\n"));
  client << (F("Connection: close\r\n\r\n"));
}

char StrSuspcsCmd2Long(EthernetClient webclient, int bufferSize, IPAddress segm, char *inBuff, char c) {
  String cmdencoded = "";
  sprintf(temptxtbuff, "\r\nERROR: max linebuffer is %d, current buffer position is: %d", bufferMax, bufferSize); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  for (int bx = 0; bx < 4; bx++) {
    suspicious_ips[suspicious_index][bx] = segm[bx];
  }
  suspicious_string[suspicious_index][bufferSize] = c;
  bufferSize++;
  suspicious_string[suspicious_index][bufferSize] = '\0';
  textlog("\r\nHTTP received line: ", false);
  textlog(inBuff, false);
  textStringLog("\r\nIgnored part of cmdline: ", false);
  printprintable(c);
  while (webclient.available() && (c != '\n') )
  {
    c = webclient.read();
    printprintable(c);
    if (bufferSize < 255) {
      //      suspicious_length[suspicious_index] = bufferSize;
      suspicious_string[suspicious_index][bufferSize] = c; // store all character and translate afterwards to HTML encoded
      bufferSize++;
    }
    suspicious_string[suspicious_index][bufferSize] = '\0'; // always terminate
    if (bufferSize == 255) {
      textStringLog("\r\nERROR, suspicious_string too long to log completely", false);
      bufferSize++; //disable
    }
  }
  if (!min_serial) {
    textStringLog("\r\n", false);
  }
  cmdencoded = "port=888 Long line received on IOTdev: " + HTTPlineEncode(suspicious_string[suspicious_index], bufferSize);
  int cmdenclen = cmdencoded.length();
  if (cmdenclen > 255) {
    sprintf(temptxtbuff, "\r\nERROR: Encoded command is too long (%d)to store completely, shortened to 255.", cmdenclen); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    cmdenclen = 255;
  }
  cmdencoded.toCharArray(suspicious_string[suspicious_index], cmdenclen + 1) ;
  suspicious_length[suspicious_index] = cmdenclen;
  suspicious_perc[suspicious_index] = 0;
  suspicious_time[suspicious_index] = now();
  if (suspicious_mail) { // if enabled send for every suspicious command an email
    sprintf(mailsubj, "baaisolar: Too long suspicious HTML commands received");
    if (!sendEmail(11)) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail for too long suspicious command.", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  suspicious_index++;
  if (suspicious_index == SUSPARRAYDEPTH) {
    suspicious_index = 0;
  }
  suspicious_total++;
  return c;
}

void RqstCntnDtgrm(int bx, IPAddress segm) {
  String cmdencoded = "";
  if ( bx > 255 ) {
    sprintf(temptxtbuff, "\r\nERROR, unwanted unsolicited datagram too long, %d bytes, to completely log", bx);
    textlog(temptxtbuff, false);
    bx = 255;
  }
  for (bx = 0; bx < 4; bx++) {
    suspicious_ips[suspicious_index][bx] = segm[bx];
  }
  cmdencoded = HTTPlineEncode(suspicious_string[suspicious_index], bx);
  cmdencoded = "port=888 unsolicited datagram request=" + cmdencoded;
  bx = cmdencoded.length();
  if (bx > 255) {
    sprintf(temptxtbuff, "\r\nERROR: Encoded RqstCntnDtgrm is too long (%d)to store completely, shortened to 255.", bx); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    bx = 255;
  }
  cmdencoded.toCharArray(suspicious_string[suspicious_index], bx + 1) ;
  suspicious_length[suspicious_index] = bx;
  suspicious_perc[suspicious_index] = 0;
  suspicious_time[suspicious_index] = now();
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Suspicious unsolicited datagram request stored in index %d", hour(), minute(), second(), suspicious_index); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  if (suspicious_mail) { // if enabled send for every suspicious command an email
    sprintf(mailsubj, "baaisolar: suspicious HTML command received contains unsolicited datagram");
    if (!sendEmail(11)) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail for suspicious received HTML command which contains unsolicited datagram.", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  suspicious_index++;
  if (suspicious_index == SUSPARRAYDEPTH) {
    suspicious_index = 0;
  }
  suspicious_total++;
}

bool checkknownIP(IPAddress segm) {
  /*
     Ranges to be excluded from reporting to abuseIPDB
  */
  bool result = false;
  long int flashaddr;
  uint8_t* readPtr = rdbuffer; //position off scan point to detect a \r\n position char of ipaddres in text
  uint32_t bx = 0;
  uint8_t uisegm[4];
  for (int iip = 0; iip < 4; iip++) {
    uisegm[iip] = segm[iip];
  }
  if (HTTPlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Scanning for IP address: %d.%d.%d.%d", hour(), minute(), second(), uisegm[0], uisegm[1], uisegm[2], uisegm[3]); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  abuse_well_known_scans++;
  flashaddr = abusefilter1_addr;
  abuse_array_scan_start_time = millis();
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  while (*readPtr != 0xFF) {
    if (*readPtr == uisegm[0]) {
      //      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Subnet IP %d comparing to %d", hour(), minute(), second(), *readPtr, uisegm[0]); // temptxtbuff is max 1024
      //      textlog(temptxtbuff, false);
      if (*(readPtr + 1) == uisegm[1]) {
        if (*(readPtr + 2) == uisegm[2]) {
          if ((*(readPtr + 3) == uisegm[3]) || (*(readPtr + 3) == 0)) {
            if (HTTPlog) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Subnet IP %d.%d.%d.%d found in well known subnet array", hour(), minute(), second(), uisegm[0], uisegm[1], uisegm[2], uisegm[3]); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
            }
            result = true;
            abuse_well_known_hits++;
            break;
          }
        }
      }
    }
    bx++;
    readPtr += 4;
    //      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d DEBUG: Buffer read pointer address: 0x%08x offset in buffer is: 0x%08x", hour(), minute(), second(), readPtr, (readPtr-rdbuffer)); // temptxtbuff is max 1024
    //      textlog(temptxtbuff, false);
    if (readPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
      flashaddr += EEPROM_PAGE_SIZE;
      flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
      readPtr = rdbuffer;
      //      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08x", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
      //      textlog(temptxtbuff, false);
    }
  }
  abuse_array_scan_run_time = millis() - abuse_array_scan_start_time;
  if (HTTPlog && !result) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Subnet IP not found in well known subnet array", hour(), minute(), second()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  if (HTTPlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Subnet IP scanned %d subnets in %dms.", hour(), minute(), second(), bx, abuse_array_scan_run_time); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  return result;
}

bool checkForUnknownHTTPcommand(String unknwncmdStr) {
  uint8_t* readPtr = rdbuffer; //position off scan point to detect a \r\n position in the textlist
  uint32_t flashaddr = HTTPblacklist1_addr;
  char HTTPcmd[17];
  uint8_t HTTPcmdptr = 0;
  String HTTPtext;  // HTML blacklisttextfragment+0x00, string is max 15 positions
  // convert the command to uppercase
  unknwncmdStr.toUpperCase();
  flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08x, first byte 0x%02X", hour(), minute(), second(), flashaddr, *readPtr); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  //  while (*readPtr != 0xFF) {
  while ((readPtr < (rdbuffer + EEPROM_PAGE_SIZE)) && (*readPtr != 0xFF)) {
    while ( *readPtr != 0 ) {
      HTTPcmd[HTTPcmdptr++] = *readPtr++;
    }
    HTTPcmd[HTTPcmdptr] = '\0';
    if ( *readPtr == 0 ) {
      HTTPtext = (String)HTTPcmd;
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s:%s:%04d DEBUG: checking unknown HTML command: %s via string: %s.", hour(), minute(), second(), FILENAME, __func__, __LINE__, HTTPcmd, HTTPtext.c_str()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);

      HTTPtext.toUpperCase();
      if ( unknwncmdStr.indexOf(HTTPtext) != -1) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %s:%s:%04d INFO: Unknown HTML command found in blacklist.", hour(), minute(), second(), FILENAME, __func__, __LINE__); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        return true;
      }
      readPtr += 16 - HTTPcmdptr;
      HTTPcmdptr = 0;
    }
  }
  if (readPtr == (rdbuffer + EEPROM_PAGE_SIZE)) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFO Flash read pagebreak detected, reading last Flash page for converted blacklisted HTML fragments.", hour(), minute(), second()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    flashaddr += EEPROM_PAGE_SIZE;
    if (flashaddr == (HTTPblacklist2_addr + EEPROM_PAGE_SIZE)) {
      return false;
    }
    flash.readByteArray(flashaddr, rdbuffer, EEPROM_PAGE_SIZE);
    readPtr = rdbuffer;
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Flash read success address: 0x%08x", hour(), minute(), second(), flashaddr); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  //  }
  return false;
}

void charsReceivedNoCmd(int bufferSize, char *inBuff, IPAddress segm) {
  // deze zou afgevangen moeten worden door StrSuspcsCmd0Start
  String ncavlbl;
  bool TLStry = false;
  if (inBuff[0] == 0) {
    ncavlbl = "port=888 Binarycommand starting with 0: " + HTTPlineEncode(inBuff, bufferSize);
  }
  else if (bufferSize > 0) {
    if (((byte)inBuff[0] == 0x16) && ((byte)inBuff[1] == 0x03)) {
      if (checkknownIP(segm)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: TLS connection from known IP source", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        return;
      }
      TLStry = true;
      ncavlbl = "port=888 client is trying to connect by TLS, not allowed for this port.";
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: %s", hour(), minute(), second(), ncavlbl.c_str()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    else {
      ncavlbl = "port=888 Binary protocol request-> " + HTTPlineEncode(inBuff, bufferSize);
    }
  }
  else {
    ncavlbl = "port=888 no valid HTTP header available";
  }
  errorHTMLnocharavlbl++;
  if (HTTPlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Storing suspicious command or incorrect protocol by charsReceivedNoCmd: %s", hour(), minute(), second(), ncavlbl.c_str()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  int bx = ncavlbl.length();
  if (bx > 255) {
    sprintf(temptxtbuff, "\r\nERROR: Encoded charsReceivedNoCmd is too long (%d)to store completely, shortened to 255.", bx); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    bx = 255;
  }
  suspicious_length[suspicious_index] = bx;
  ncavlbl.toCharArray(suspicious_string[suspicious_index], suspicious_length[suspicious_index] + 1);
  for (bx = 0; bx < 4; bx++) {
    suspicious_ips[suspicious_index][bx] = segm[bx];
  }
  suspicious_perc[suspicious_index] = 0;
  suspicious_time[suspicious_index] = now();
  if (suspicious_mail) { // send email for every suspicious command
    sprintf(mailsubj, "baaisolar: suspicious connection request received, characters received but no HTTP command");
    if (!sendEmail(11)) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail for modulo 5 number of suspicious commands.", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  suspicious_index++;
  if (suspicious_index == SUSPARRAYDEPTH) {
    suspicious_index = 0;
  }
  suspicious_total++;
  sprintf(temptxtbuff, "\r\nhttphacks increased in line: %d ", __LINE__ + 2); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  httphacks++;
  httphacksTime = now();
}

void StoreUnknwnCmd(String unknwncmdStr, IPAddress segm) {
  int ucsl;
  if (checkknownIP(segm)) {
    sprintf(temptxtbuff, "\r\nINFORMATION: String Unknown Command is originated from adddres in whitelist, no action taken.", ucsl); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    unkcmdfrmwhl++;
    unknownCmd_wkwnips[unknownCmd_index] = true;
  }
  else {
    unknownCmd_wkwnips[unknownCmd_index] = false;
  }
  ucsl = unknwncmdStr.length();
  if ( ucsl > 255 ) {
    ucsl = 255;
    sprintf(temptxtbuff, "\r\nERROR: String Unknown Command is too long (%d)to store completely, shortened to 255.", ucsl); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }

  sprintf(temptxtbuff, "\r\nTo store in unknown command index: %d stringlength=%d, unknwncmdStr=%s", unknownCmd_index, unknwncmdStr.length(), unknwncmdStr.c_str()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  unknwncmdStr.toCharArray(unknownCmd_string[unknownCmd_index], ucsl + 1); //unknown_string is max 256
  //  sprintf(temptxtbuff, "\r\nAfter conversion to char array, unknwncmdStr= %s", unknownCmd_string[unknownCmd_index]); // temptxtbuff is max 1024
  //  textlog(temptxtbuff, false);
  unknownCmd_length[unknownCmd_index] = unknwncmdStr.length();
  for (int bx = 0; bx < 4; bx++) {
    unknownCmd_ips[unknownCmd_index][bx] = segm[bx];
  }
  // convert the command to uppercase
  unknwncmdStr.toUpperCase();

  if (checkForUnknownHTTPcommand(unknwncmdStr)) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Match in well known dictionary, queued without manual intervention on array index %d.", hour(), minute(), second(), unknownCmd_index);
    textlog(temptxtbuff, false);
    unknownCmd_queued[unknownCmd_index] = 1;
    unknownCmd_sema = true;
  }
  else {
    unknownCmd_queued[unknownCmd_index] = 0; // verification requested
  }
  unknownCmd_perc[unknownCmd_index] = 0;
  unknownCmd_time[unknownCmd_index] = now();
  if (unknownCmd_mail) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Sending email for unknown HTML command index array position %d", hour(), minute(), second(), unknownCmd_index);
    textlog(temptxtbuff, false);
    sprintf(mailsubj, "baaisolar: Unknown HTML command received");
    sendEmail(16);
  }
  unknownCmd_index++;
  if (unknownCmd_index == UNKWNCMDARRAYDEPTH) {
    unknownCmd_index = 0;
  }
  unknownCmd_total++;
}

bool showRxLine(int bufferSize, const char *inBuff) {
  bool inBuffisValid = true;
  String encodedLine = "";
  if (HTTPlog) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d The length of the received HTTP request line is: %d, if necessary the line is automatic HTML encoded for display", hour(), minute(), second(), bufferSize); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  if (bufferSize > 0 ) {
    for (int inbx = 0; inbx < bufferSize; inbx++) {
      if (inBuff[inbx] == '\0') {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: the received HTTP request line contains a NULL character at position: %d (index is 0)", hour(), minute(), second(), inbx); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        inBuffisValid = false;
        break;
      }
    }
    if (HTTPlog) {
      if (inBuffisValid) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d The received HTTP request line URL unencoded is: %s", hour(), minute(), second(), inBuff); // temptxtbuff is max 1024
      }
      else {
        encodedLine = HTTPlineEncode(inBuff, bufferSize);
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d The received HTTP request line HTTP encoded is: %s", hour(), minute(), second(), encodedLine.c_str()); // temptxtbuff is max 1024
      }
      textlog(temptxtbuff, false);
    }
  }
  return inBuffisValid;
}

void ServeWebClients() {
  EthernetClient webclient = server.available();
  if (webclient)
  {
    int bufferSize = 0;
    int bx = 0;
    int charcnt = 0;
    int inbx = 0;
    char inBuff[bufferMax];  // max is 256
    String cmdString = "";
    //    cmdString[0] = '\0';
    char optcmd[32];
    bool cmdRcvd = false;
    bool currentLineIsBlank = true;
    bool cmdprocessed = false;
    char c = '\0';
    //    suspBufferIndex = 0;
    //    suspinBuff[0] = '\0';
    inBuff[0] = '\0';
    unsigned long starttime = millis();
    //    char valbuff[20];
    webclient.setConnectionTimeout(5000);
    IPAddress segm = webclient.remoteIP();
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d New HTTP client, remote IP address: %d.%d.%d.%d", hour(), minute(), second(), segm[0], segm[1], segm[2], segm[3]);
    textlog(temptxtbuff, false);
    while (webclient.connected() && ((millis() - starttime) < 5000))
    {
      if ( webclient.available())
      {
        c = webclient.read();
        if ( c == '\0' && currentLineIsBlank) {
          // this must be a suspicious command, pull down the datagram and return the last received char/byte
          sprintf(temptxtbuff, "\r\nERROR: A new line has started with a NULL character, datagram pulled last character received: %c, in hex 0x%02X", c, (byte)c); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          while (webclient.available()) {   //empty readbuffer
            c = webclient.read();
            if (bufferSize < bufferMax - 1) {
              inBuff[bufferSize] = c;
              bufferSize++;
              inBuff[bufferSize] = '\0'; //allways terminate the buffer
            }
          }
          charsReceivedNoCmd(bufferSize, inBuff, segm);
          bufferSize = 0;
          inBuff[0] = '\0';
          sprintf(temptxtbuff, "\r\nhttphacks increased in line: %d ", __LINE__ + 2); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          break;
        }
        if ( c == '\n' && currentLineIsBlank) {
          // this is most probably the regular header line that marks the end of header, a valid command should be received
          //    cmdString = webclient.readStringUntil('\n');  // for debug reasons, surge the whole header
          //         textStringLog("\r\nInterpreting command: ", false);
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Interpreting command: ", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);

          char cobuff[512];
          int cpybuflen;
          if (cmdString.length() > 511) {
            cpybuflen = 511;
          }
          else {
            cpybuflen = cmdString.length();
          }
          cmdString.toCharArray(cobuff, cpybuflen + 1);  // the +1 toCharArray sets terminator
          textlog(cobuff, false);
          // command index selection
          if (HTTPlog) {
            sprintf(temptxtbuff, "\r\nlength numcmd_string_table: %d", numcmd_string_table); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          int cmdi = 0;
          for ( cmdi = 0 ; cmdi < numcmd_string_table ; cmdi++ ) {
            if (cmdi > numcmd_string_table) {
              sprintf(temptxtbuff, "\r\nERROR comparisition command check tablelength out of bound, CHECK array for quotes!"); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
              break;
            }
            strcpy(optcmd, cmd_string_table[cmdi]); // Necessary casts and dereferencing, just copy.
            if (HTTPlog) {
              sprintf(temptxtbuff, "\r\nOptional command: %d from equal test: %s -> test: ", cmdi, optcmd); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
              delay(3);
            }
            if (Command(cmdString, optcmd)) {
              if (HTTPlog) textlog("true", false);
              break;
            }
            if (HTTPlog) {
              textlog("false", false);
            }
          }
          if (cmdi == numcmd_string_table) {
            //            textStringLog("\r\nNo matching command in lookuptable, check for fan command remains: ", false);
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d No matching command in lookuptable, check for fan command remains: ", hour(), minute(), second()); // temptxtbuff is max 1024
          }
          else {
            //            textStringLog("\r\nHTTP Response sending for command: ", false);
            //            textlog(optcmd, false);
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d HTTP Response sending for command: %s", hour(), minute(), second(), optcmd); // temptxtbuff is max 1024
            Send_header(webclient);
          }
          textlog(temptxtbuff, false);
          textStringLog(", index nr: ", false);
          itoa(cmdi, cobuff, 10);
          textlog(cobuff, false);
          switch (cmdi) {
            case 0:
              SaveYield(webclient, segm); // "saveyield" the variables of this day
              break;
            case 1:
              SaveValue(webclient, segm); // "save" the variables of this day
              break;
            case 2:
              DumpYield(webclient, 0, segm); // "RAMyield" dump the daily yield for last three months from RAM array
              break;
            case 3:
              ShowStatus(webclient, false, segm); //shows received commands that not could interpreted
              break;
            case 4:
              eraseCounters(webclient, segm); // "erase" the saved variables of this day, restore @startup
              break;
            case 5:
              UpdateTimeWeb(webclient, segm); // "ntp" update time from web
              break;
            case 6:
              help(webclient, segm); // "help", show the commands
              break;
            case 7:
              showIPlog(webclient, segm); // "showiplog" shows and toggles the WAN IP logging
              break;
            case 8:
              showversion(webclient, 1, segm); // "showversion", show the version build
              break;
            case 9:
              fanDefault(webclient, segm); // "fandefault", sets the default thresholds for fan temperature
              break;
            case 10:
              showFan(webclient, segm); // "fan", show the actual temperature thresholds
              break;
            case 11:
              ShowSockStatus(webclient, segm); // "showsockets", shows the forced disconnects
              break;
            case 12:
              showPVIdebug(webclient, segm); // "showPVIdebug", shows the PVI measurement values
              break;
            case 13:
              showPVOstring(webclient, segm); // "showPVOstring", shows the PVOutput add outputstring
              break;
            case 14:
              switchLogging(webclient, segm); // "Seriallogging", Toggles serial traces
              break;
            case 15:
              showBufferremaining(webclient, segm); // "showbufferremaining",
              break;
            case 16:
              showPluvioURL(webclient, segm); // "showPluvioURL", shows the url to get the pluvio
              break;
            case 17:
              GoUpdateDNS(webclient, segm); // "GoUpdateDNS", updates the DynDNS
              break;
            case 18:
              UpdateDNSKey(webclient, cmdString, segm); // "UpdateDNSKey", updates the DynDNS updatekey
              break;
            case 19:
              ethernetlogging(webclient, segm); // "Ethernetlogging", toggles ethernetlogging
              break;
            case 20:
              display_memoryinfo(webclient, segm); // "showmem", show memory usage
              break;
            case 21:
              DumpYield(webclient, 1, segm); // "yield", dump the daily yield for last three monthes from flash
              break;
            case 22:
              forceEmail(webclient, segm); // "forceEmail", force to send email
              break;
            case 23:
              show_logging(webclient, segm); // "showlogging", shows the actual status of the logging flags
              break;
            case 24:
              showWeatherlog(webclient, segm); // "showWeatherlog", Toggles weather buffer in logging
              break;
            case 25:
              showPVOutputlog(webclient, segm); // "showPVOutputlog", toggles the logging to pvoutput
              break;
            case 26:
              showIP(webclient, segm); // "showip", shows the actual networkinterface configuration
              break;
            case 27:
              showMailLogging(webclient, segm); // "showmaillog" Toggles maillogging
              break;
            case 28:
              setUDPon(webclient, segm); // "UDPon" switch ethernet UDP client logging on
              break;
            case 29:
              setUDPoff(webclient, segm); // "UDPoff"switch ethernet UDP client logging on
              break;
            case 30:
              setHTTPlogging(webclient, segm); // "setHTTPlog"switch HTTP client logging on
              break;
            case 31:
              setPluvioEna(webclient, segm); // enbles requests to buienradar
              break;
            case 32:
              setInverterlog(webclient, segm); // enables inverter logging
              break;
            case 33:
              setbrlogging(webclient, segm); //enables buienradar.nl logging
              break;
            case 34:
              ShowStatus(webclient, true, segm); //shows the extended status including debug counters
              break;
            case 35:
              ShowSuspectURL(webclient, segm); //shows the suspected commands
              break;
            case 36:
              WolfSSLMailSwitch(webclient, segm); //shows the suspected commands
              break;
            case 37:
              setabuslogging(webclient, segm); //enables AbuseIPDB logging
              break;
            case 38:
              showUnknownCmd(webclient, segm); //shows received commands that not could interpreted
              break;
            case 39:
              flash_serial_dump_table(webclient, segm); // "dump" the content of the flash
              break;
            case 40:
              reportunkwncmd(webclient, cmdString, segm); // reports an unknown command for abuse
              break;
            case 41:
              SuspEmail(webclient, segm); // reports an unknown command for abuse
              break;
            case 42:
              UnknownCmdEmail(webclient, segm); // reports an unknown command for abuse
              break;
            case 43:
              setBrCorrFact(webclient, cmdString, segm); // sets the correction factor for Buienradar pluvio, must contain a real in commandstring
              break;
            case 44:
              startTFTP(webclient, segm); // reports an unknown command for abuse
              break;
            case 45:
              testIPwhitelist(webclient, segm); // test for reports an unknown command for abuse
              break;
            case 46:
              testHTTPblacklist(webclient, cmdString, segm); // test reports an unknown command for abuse
              break;
            default:
              //check if it is a fan command;
              int i = cmdString.indexOf("?");
              if ((i != -1) && ((cmdString[i + 1] == 'A') || (cmdString[i + 1] == 'B') || (cmdString[i + 1] == 'C'))) {
                Send_header(webclient);
                ReadValue(cmdString, i, webclient, segm);
              }
              else {
                if (!min_serial) {
                  textStringLog("\r\nNo command matches, 404 send. ", false);
                }
                /*
                   Only GET is supported
                   HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH are not supported and reported, somebody is too curious....
                */
                if (cmdString.length() > 0) {
                  // We received an unknown command, store this to a different catogory. Per definition it is not suspicious could be a typo, but most of the time it is an attempted intrusion
                  StoreUnknwnCmd (cmdString, segm);
                  sprintf(temptxtbuff, "\r\nhttphacks increased: %d ", __LINE__); // temptxtbuff is max 1024
                  textlog(temptxtbuff, false);
                  httphacks++;
                  httphacksTime = now();
                }
                send404(webclient, segm);
                //                ShowStatus(webclient, false);
              }
          }
          cmdprocessed = true;
          httpShown++;
          if (HTTPlog && showbufferremaining && (webclient.available() > 0)) {
            textlog("\r\nEmptying buffer, content of data is: ", true);
          }
          bx = 0;
          while (webclient.available()) {   //empty readbuffer
            c = webclient.read();
            if (bx < 255) {
              suspicious_string[suspicious_index][bx] = c;
              suspicious_string[suspicious_index][bx + 1] = '\0';
            }
            bx++;
            if (HTTPlog && showbufferremaining) {
              printprintable(c); // temptxtbuff is max 1024
            }
          }
          if (HTTPlog && showbufferremaining && (bx != 0)) {
            textlog("\r\n", false);
            sprintf(temptxtbuff, "Buffer contained %d characters of data.", bx);
            textlog(temptxtbuff, false);
          }
          if (bx > 3) {
            // the remaining data in the request containt additional datagram which is suspicious
            RqstCntnDtgrm(bx, segm);
          }
          bufferSize = 0;
          inBuff[bufferSize] = '\0';
          break;
        }
        else if (c == '\n') {
          // we're starting a new line
          currentLineIsBlank = true;
          // if line contain non ASCII, the header is invalid
          if (!showRxLine(bufferSize, inBuff)) {
            while (webclient.available()) {   //empty readbuffer
              c = webclient.read();
              if (bufferSize < bufferMax - 1) {
                inBuff[bufferSize] = c;
                bufferSize++;
                inBuff[bufferSize] = '\0'; //allways terminate the buffer
              }
            }
            charsReceivedNoCmd(bufferSize, inBuff, segm);
            bufferSize = 0;
            inBuff[0] = '\0';
            break;
          }
          if ( !cmdRcvd ) {
            String tmpString = String(inBuff);
            // reset command buffer
            bufferSize = 0;
            inBuff[0] = '\0';
            if ( tmpString.indexOf("/favicon") != -1 )
            {
              //              textStringLog("\r\nHTTP Response sending favicon", false);
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d HTTP Response sending favicon", hour(), minute(), second()); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
              // Cache-Control: max-age=31536000\r\nAccept-Ranges: bytes\r\n
              webclient << F("HTTP/1.0 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccept-Ranges: bytes\r\nContent-Type: image/x-icon\r\nConnection: close\r\n") << endl;
              favicon(webclient);
              bufferSize = 0;
              bool susprqst = false;
              while (webclient.available()) {   //empty readbuffer
                c = webclient.read();
                if ((byte)c == 0) {
                  susprqst = true;
                }

                if (bufferSize < bufferMax - 1) {
                  inBuff[bufferSize] = c;
                  bufferSize++;
                  inBuff[bufferSize] = '\0'; //allways terminate the buffer
                  if (HTTPlog && showbufferremaining) {
                    if (bufferSize == 1) {
                      textlog("\r\nReadbuffer after favicon request contains:", false);
                    }
                    printprintable(c);
                  }
                }
              }
              if (susprqst && (bufferSize > 5)) { // prevent that it triggers on the end of a valid header
                charsReceivedNoCmd(bufferSize, inBuff, segm);
              }
              if (bufferSize > 0) {
                sprintf(temptxtbuff, " -> flushed %d characters from buffer after favicon request", bufferSize); // temptxtbuff is max 1024
                textlog(temptxtbuff, false);
              }
              bufferSize = 0;
              inBuff[0] = '\0';
              break;
            }
            else if ( tmpString.indexOf("PRI * HTTP/2.0") != -1 ) {
              textStringLog("\r\nHTTP Response sending PRI not supported", false);
              // Cache-Control: max-age=31536000\r\nAccept-Ranges: bytes\r\n
              //Illegal request, responding with status '501 Not Implemented': Unsupported HTTP method: PRI
              webclient << F("HTTP/1.0 501 Not Implemented\r\nAccess-Control-Allow-Origin: *\r\nAccept-Ranges: none\r\nConnection: close\r\n") << endl;
              bufferSize = 0;
              while (webclient.available()) {   //empty readbuffer
                c = webclient.read();
                bufferSize++;
                if (HTTPlog && showbufferremaining) {
                  if (bufferSize == 1) {
                    textlog("\r\n", false);
                  }
                  printprintable(c);
                }
              }
              if (bufferSize > 0) {
                sprintf(temptxtbuff, " -> flushed %d characters from buffer by PRI command", bufferSize); // temptxtbuff is max 1024
                textlog(temptxtbuff, false);
              }
              bufferSize = 0;
              inBuff[0] = '\0';
              break;
            }
            else if ( tmpString.indexOf("GET /robots.txt HTTP/") != -1 ) {
              sendRobotstxt(webclient, segm); // answer that robots are not appreciated
              bufferSize = 0;
              inBuff[0] = '\0';
              break;
            }
            else if ( tmpString.indexOf("GET /sitemap.xml HTTP/") != -1 ) {
              send404(webclient, segm);
              bufferSize = 0;
              inBuff[0] = '\0';
              break;
            }
            //            else if ( tmpString.indexOf("GET /") != -1 ) {
            else { // the first line is always the HTTP request method
              cmdString = tmpString;
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d HTTP cmd rcvd: ", hour(), minute(), second());
              textlog(temptxtbuff, false);
              char cobuff[512];
              int cpybuflen;
              if (cmdString.length() > 511) {
                cpybuflen = 511;
              }
              else {
                cpybuflen = cmdString.length();
              }
              cmdString.toCharArray(cobuff, cpybuflen + 1);
              //              cobuff[cpybuflen] = '\0'; // toCharArray sets terminator
              textlog(cobuff, false);
              cmdRcvd = true;
            }
          }
          bufferSize = 0;
          inBuff[bufferSize] = '\0';
        }
        else if (c != '\r') {
          // we've gotten a character on the current line
          currentLineIsBlank = false;
          if ( bufferSize < (bufferMax - 2) ) // last position is for string terminator buffermax = 256
          {
            inBuff[bufferSize] = c;
            bufferSize++;
            inBuff[bufferSize] = '\0'; //allways terminate the buffer
          }
          else {
            // the command is too long, maybe a bufferoverflow attack, drain the input to EOL
            sprintf(temptxtbuff, "\r\nERROR: HTTP buffer input OVERFLOW, input after position %d ignored.", bufferMax); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            c = StrSuspcsCmd2Long(webclient, bufferSize, segm, inBuff, c);
            if (c == '\n') {
              // we're starting a new line
              currentLineIsBlank = true;
              bufferSize = 0;
              inBuff[bufferSize] = '\0';
            }
            httpbuffoverflow++;
            httpbuffoverflowTime = now();
            webclient << F("HTTP/1.0 413 Entity Too Large\r\nAccess-Control-Allow-Origin: *\r\nAccept-Ranges: none\r\nConnection: close\r\n") << endl;
          }
        }
      }
      else {
        if (cmdprocessed) {
          break;
        }
        textStringLog("\r\nJust connected, no characters available", false);
        sendHTMLheaderwarning(webclient);
        delay(100);
        if ( !webclient.available()) {
          charsReceivedNoCmd(bufferSize, inBuff, segm);
          break;
        }
        //     printprintable(c); // enable this one if the buffer http request is looping
      }
    }
    if (!min_serial) {
      //      textStringLog("\r\nelapsed requesttime in ms: ", false);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d elapsed requesttime in ms: %dms", hour(), minute(), second(), millis() - starttime); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);

      //      char cobuff[16];
      //      ltoa((millis() - starttime), cobuff, 10);
      //      textlog(cobuff, false);
    }
    webclient.flush();

    if (webclient.connected()) {
      if (HTTPlog) Serial.print(F("\r\nwebclient is connected after request"));
      //emptying the read buffer
      while (webclient.available()) {
        webclient.read();
      }
      if (HTTPlog) textStringLog("\r\nwebclient connection stop", false);
      webclient.stop();
      if (HTTPlog) textStringLog("ped", false);
    }
    else {
      if (HTTPlog) Serial.print(F("\r\nwebclient is NOT connected after request"));
    }
    if (!min_serial) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Polling PVI: ", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
}

bool Command(String cmdString, char* command) {
  int i = cmdString.indexOf(command);
  if (i == -1) return false;
  return true;
}

void ReadValue(String input, int i, EthernetClient client, IPAddress segm) {
  float value = 0;
  bool neg = false;
  int j = i + 3;
  int address = input[i + 1];
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipreadvalueshown[iip] = segm[iip];
    }
    readvalueshown++;
    timereadvalueshown = now();
  }
  Serial.print(F("String input is: "));
  Serial.println(input);
  if ((address >= 'A' && address < 'D') && (input[i + 2] = '='))
  {
    char c = input[j];
    if (c == '-')
    {
      neg = true;
      j ++;
      c = input[j];
    }
    while (c >= '0' && c <= '9')
    {
      value = 10 * value + (c - '0');
      j ++;
      c = input[j];
    }
    Serial.print(F("value readed: "));
    while (value >= 100)
    {
      value = value / 10;
    }
    if (neg)
    {
      // negeer indien kleiner dan 30 of groter dan 50
      if ((value < 30.0) || (value > 50.0))
      {
        value = 38.0;
        textStringLog("Value out of bounds 30-50", true);
        client << (F("Value out of bounds 30-50, defaulted to: ")) << value << endl;
      }
      float strdval;
      switch (address) {
        case 'A':
          // check if off temp < on temp
          strdval = tempFanAOn;
          if (strdval <= value)
          {
            value = strdval - (float)1.5;
            client << (F("ERROR, temp fan on < temp fan off. Corrected fan off to fan on - 1.5 \r\n"));
            textStringLog("ERROR, temp fan on < temp fan off. Corrected fan off to fan on - 1.5 ", true);
          }
          tempFanAOff = value; // fan A off temp
          textStringLog("new fan A off temperature: ", false);
          client << (F("new fan A off temperature: "));
          break;
        case 'B':
          // check if off temp < on temp
          strdval = tempFanBOn;
          if (strdval <= value)
          {
            value = strdval - (float)2.0;
            client << (F("Error, temp fan on < temp fan off. Corrected fan off to fan on - 2.0 \r\n"));
            textStringLog("Error, temp fan on < temp fan off. Corrected fan off to fan on - 2.0 ", true);
          }
          tempFanBOff = value; // fan B off temp
          textStringLog("new fan B off temperature: ", false);
          client << (F("new fan B off temperature: "));
          break;
        case 'C':
          // check if off temp < on temp
          strdval = tempFanCOn;
          if (strdval <= value)
          {
            value = strdval - (float)3.0;
            client << (F("Error, temp fan on < temp fan off. Corrected fan off to fan on - 3.0 \r\n"));
            textStringLog("Error, temp fan on < temp fan off. Corrected fan off to fan on - 3.0 ", true);
          }
          tempFanCOff = value; // fan C off temp
          textStringLog("new fan C off temperature: ", false);
          client << (F("new fan C off temperature: "));
          break;
      }
    }
    else
    {
      if ((value < 30.0) || (value > 50.0))
      {
        value = 40.0;
        textStringLog("Value out of bounds 30-50", true);
        client << (F("Value out of bounds 30-50, defaulted to: ")) << value << endl;
      }
      float strdval;
      switch (address) {
        case 'A':
          // check if on temp>off temp
          strdval = tempFanAOff;
          if (value <= strdval)
          {
            value = strdval + (float)1.5;
            client << (F("Error, temp fan on < temp fan off. Corrected fan on to + 1.5 of fan off temp.\r\n"));
            textStringLog("Error, temp fan on < temp fan off. Corrected fan on to + 1.5 of fan off temp.", true);
          }
          tempFanAOn = value; // fan A on temp
          textStringLog("new fan A on temperature: ", false);
          client << (F("new fan A on temperature: "));
          break;
        case 'B':
          // check if on temp>off temp
          strdval = tempFanBOff;
          if (value <=  strdval)
          {
            value = strdval + (float)2.0;
            client << (F("Error, temp fan on < temp fan off. Corrected fan on to + 2.0 of fan off temp.\r\n"));
            textStringLog("Error, temp fan on < temp fan off. Corrected fan on to + 2.0 of fan off temp.", true);
          }
          tempFanBOn = value; // fan B on temp
          textStringLog("new fan B on temperature: ", false);
          client << (F("new fan B on temperature: "));
          break;
        case 'C':
          // check if on temp>off temp
          strdval = tempFanCOff;
          if (value <=  strdval)
          {
            value = strdval + (float)3.0;
            client << (F("Error, temp fan on < temp fan off. Corrected fan on to + 3.0 of fan off temp.\r\n"));
            textStringLog("Error, temp fan on < temp fan off. Corrected fan on to + 3.0 of fan off temp.", true);
          }
          tempFanCOn = value; // fan C on temp
          textStringLog("new fan C on temperature: ", false);
          client << (F("new fan C on temperature: "));
          break;
      }
    }
    char cobuff[16];
    String fpstring = String(value, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(cobuff, 6);
    textlog(cobuff, true);
    //    client << value << F("\r\n</body>\r\n</html>");
    client << value << F("\r\n</body>\r\n</html>");
  }
}

void testHTTPblacklist(EthernetClient webclient, String cmd, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      iptesthttpblacklistshown[iip] = segm[iip];
    }
    testhttpblacklistshown++;
    timetesthttpblacklistshown = now();
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Your commandline contain %s suspicious part", hour(), minute(), second(), checkForUnknownHTTPcommand(cmd) ? "a" : "no");
  textlog(temptxtbuff, false);
  webclient << temptxtbuff << F("\r\n</body>\r\n</html>");
}

void testIPwhitelist(EthernetClient webclient, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      iptestipwhitelistshown[iip] = segm[iip];
    }
    testipwhitelistshown++;
    timetestipwhitelistshown = now();
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Your address: %d.%d.%d.%d %s found in the whitelist", hour(), minute(), second(), segm[0], segm[1], segm[2], segm[3], checkknownIP(segm) ? "is" : "is not");
  textlog(temptxtbuff, false);
  webclient << temptxtbuff << F("\r\n</body>\r\n</html>");
}

void startTFTP(EthernetClient webclient, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipstarttftpshown[iip] = segm[iip];
    }
    starttftpshown++;
    timestarttftpshown = now();
  }
  tftpInit(webclient);
}

void setBrCorrFact(EthernetClient webclient, String cmdstring, IPAddress segm) {
  int is = cmdstring.indexOf("=");
  int dt = cmdstring.indexOf(".");
  float value, fraction;
  char c;
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipsetbrcorrfactshown[iip] = segm[iip];
    }
    setbrcorrfactshown++;
    timesetbrcorrfactshown = now();
  }
  if (is > 0 && dt > 0 && dt < 22) {
    if ( !isnan(flash.readFloat(BrCorrFact_addr))) {
      BrCorrFact = flash.readFloat(BrCorrFact_addr);
    }
    else {
      BrCorrFact = 1.0;
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: reading setBrCorrFact from EEPROM, set to default: 1.00", hour(), minute(), second());
      textlog(temptxtbuff, false);
    }
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Old value for Buienradar pluvio correction factor = ", hour(), minute(), second());
    textlog(temptxtbuff, false);
    char cobuff[8];
    String fpstring = String(BrCorrFact, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(cobuff, 6);
    textlog(cobuff, false);
    webclient << temptxtbuff << cobuff << F("<br>\r\n");
    is++;
    value = 0.0;
    c = cmdstring[is];
    while (c >= '0' && c <= '9')
    {
      value = (10 * value) + (c - '0');
      is++;
      c = cmdstring[is];
    }
    dt++;
    is = 1;
    fraction = 0.0;
    c = cmdstring[dt];
    while (c >= '0' && c <= '9')
    {
      fraction = (float)(c - '0') / (float)pow( 10, is);
      value += fraction;
      dt++;
      is++;
      c = cmdstring[dt];
    }
    if (value < 10.00 && value > 00.00) {
      BrCorrFact = value;
    }
    else {
      BrCorrFact = 1.00;
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Conversion error for new value or out of bounds; 0.00 < BrCorrFact < 9.99. Default to 1.00.<br>", hour(), minute(), second());
      textlog(temptxtbuff, false);
      webclient << temptxtbuff;
    }
    flash.eraseSector(BrCorrFact_addr);
    if (!flash.writeFloat(BrCorrFact_addr, BrCorrFact)) {
      flash_error = F("writeFloat BrCorrFact_addr FAIL");
      StoreFlashError(flash_error, BrCorrFact_addr);
    }
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d New value for Buienradar pluvio correction factor = ", hour(), minute(), second());
    textlog(temptxtbuff, false);
    fpstring = String(value, 2); //necessary to convert the float to char array, %f does not function
    fpstring.toCharArray(cobuff, 6);
    textlog(cobuff, false);
    webclient << temptxtbuff << cobuff;
  }
  else {
    if (is == -1) {
      String fpstring = String(BrCorrFact, 2); //necessary to convert the float to char array, %f does not function
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Current value for Buienradar pluvio correction factor = %s", hour(), minute(), second(), fpstring.c_str());
    }
    else {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: value out of bound or no real entered", hour(), minute(), second());
    }
    textlog(temptxtbuff, false);
    webclient << temptxtbuff ;
  }
  webclient << F("\r\n</body>\r\n</html>");
}

void reportunkwncmd(EthernetClient client, String cmdstring, IPAddress segm) {
  int i = cmdstring.indexOf("unkwncmd");
  i += 8;
  char n = cmdstring[i];
  int ix = n - '0';
  //  sprintf(temptxtbuff, "\r\nKarakter uit string om regel nummer te bepalen is: % c, geconverteerd getal: % d", n, ix); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  if ((ix == 0) || (ix < 0) || (ix > UNKWNCMDARRAYDEPTH)) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: index % d out of bound", hour(), minute(), second(), ix);
    textlog(temptxtbuff, false);
    client << temptxtbuff << F("\r\n</body>\r\n</html>");
    return;
  }
  if (unknownCmd_ips[ix - 1][0] == 0) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: index % d has no entry", hour(), minute(), second(), ix);
    textlog(temptxtbuff, false);
    client << temptxtbuff << F("\r\n</body>\r\n</html>");
    return;
  }
  if (unknownCmd_queued[ix - 1] == 1) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Already queued to AbuseIPDB unqueued for index % d", hour(), minute(), second(), ix - 1);
    unknownCmd_queued[ix - 1] = 0;
  }
  else if (unknownCmd_queued[ix - 1] == 2) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Index % d already reported to AbuseIPDB, status not changeable", hour(), minute(), second(), ix - 1);
    client << temptxtbuff << F("\r\n</body>\r\n</html>");
  }
  else {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SUCCESS: Report queued to AbuseIPDB successful for line % d index % d", hour(), minute(), second(), ix, ix - 1);
    unknownCmd_sema = true;
    unknownCmd_queued[ix - 1] = 1;
  }
  //  client << temptxtbuff << F("\r\n</body>\r\n</html>");
  //  textlog(temptxtbuff, false);
  showUnknownCmd(client, segm);
}

void eraseCounters(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      iperasecountersshown[iip] = segm[iip];
    }
    erasecountersshown++;
    timeerasecountersshown = now();
  }
  eraseDayYield();
  client.println(F("<p>Day counters erased (in flash).</p>\r\n</body>\r\n</html>"));
}

void fanDefault(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipfandefaultshown[iip] = segm[iip];
    }
    fandefaultshown++;
    timefandefaultshown = now();
  }
  tempFanAOff = 36.5; // fan A off temp was 37
  tempFanBOff = 38.0; // fan B off temp was 38
  tempFanCOff = 37.0; // fan C off temp was 37,5
  tempFanAOn = 38.0; // fan A on temp was 38
  tempFanBOn = 39.0; // fan B on temp was 39
  tempFanCOn = 40.0; // fan C on temp was 40
  client << (F("<p>New defaults written; </p>"));

  showFan(client, segm);
}

void WolfSSLMailSwitch(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipwolfsslmailswitchshown[iip] = segm[iip];
    }
    wolfsslmailswitchshown++;
    timewolfsslmailswitchshown = now();
  }
  setwolfSSLMail(client);
}

void showFan(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowfanshown[iip] = segm[iip];
    }
    showfanshown++;
    timeshowfanshown = now();
  }
  client << (F("<p>Fan switch temperature: <br>"));
  float fanoff;
  float fanon;
  fanon = tempFanAOn;
  fanoff = tempFanAOff;
  client << (F("Fan A on & gt; = ")) << fanon << (F(" off & lt; ")) << fanoff << br << endl;
  fanon = tempFanBOn;
  fanoff = tempFanBOff;
  client << (F("Fan B on & gt; = ")) << fanon << (F(" off & lt; ")) << fanoff << br << endl;
  fanon = tempFanCOn;
  fanoff = tempFanCOff;
  client << (F("Fan C on & gt; = ")) << fanon << (F(" off & lt; ")) << fanoff << br << endl;
  client << (F("Readonly parameters: ")) << (F("<br>\r\n"));
  client << (F("Power fan A on & gt; = ")) << fanApowerOn << (F("W off & lt; ")) << fanApowerOff << (F("W<br>\r\n"));
  client << (F("Power fan B on & gt; = ")) << fanBpowerOn << (F("W off & lt; ")) << fanBpowerOff << (F("W<br>\r\n"));
  client << (F("Power fan C on & gt; = ")) << fanCpowerOn << (F("W off & lt; ")) << fanCpowerOff << (F("W<br>\r\n"));
  client << (F("</p>\r\n</body>\r\n</html>"));
}

void switchLogging(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipswitchloggingshown[iip] = segm[iip];
    }
    switchloggingshown++;
    timeswitchloggingshown = now();
  }
  if (min_serial == false) {
    min_serial = true;
  }
  else {
    min_serial = false;
  }
#ifdef OPTION_FLASH
  flash.eraseSector(min_serial_addr);
  if (!flash.writeByte(min_serial_addr, min_serial)) {
    flash_error = F("writeByte min_serial_addr FAIL");
    StoreFlashError(flash_error, min_serial_addr);
  }
  delay(3);
#endif
  client.print(F("<p>The new status for Serial logging is: "));
  textStringLog("The new status for Serial logging is: ", false);
  if (min_serial == false) {
    client.print(F("on"));
    textStringLog("on", true);
  }
  else {
    client.print(F("off"));
    textStringLog("off", true);
  }
  client << (F(".</p>\r\n</body>\r\n</html>"));
}

void showPVOutputlog(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowpvoutputlogshown[iip] = segm[iip];
    }
    showpvoutputlogshown++;
    timeshowpvoutputlogshown = now();
  }
  textStringLog("current pvoutputlogging: ", false);
  pvoutputlog ? textStringLog("true", false) : textStringLog("false", false);
  textStringLog(" pvoutputlogging switched to: ", false);
  if (pvoutputlog == false) {
    pvoutputlog = true;
  }
  else {
    pvoutputlog = false;
  }
  pvoutputlog ? textStringLog("->true", true) : textStringLog("->false", true);
  client.print(F("<p>The new status for pvoutputlogging is: "));
  if (pvoutputlog == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(pvoutputlog_addr);
  if (!flash.writeByte(pvoutputlog_addr, pvoutputlog)) {
    flash_error = F("writeByte pvoutputlog_addr FAIL");
    StoreFlashError(flash_error, pvoutputlog_addr);
  }
  delay(3);
#endif
}

void showMailLogging(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowmailloggingshown[iip] = segm[iip];
    }
    showmailloggingshown++;
    timeshowmailloggingshown = now();
  }
  textStringLog("current maillogging: ", false);
  maillogging ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" maillogging switched to: ", false);
  if (maillogging == false) {
    maillogging = true;
  }
  else {
    maillogging = false;
  }
  maillogging ? textStringLog("->on", true) : textStringLog("->off", true);
  client.print(F("<p>The new status for maillogging is: "));
  if (maillogging == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(maillogging_addr);
  if (!flash.writeByte(maillogging_addr, maillogging)) {
    flash_error = F("writeByte maillogging_addr FAIL");
    StoreFlashError(flash_error, maillogging_addr);
  }
  delay(3);
#endif
}

void showIPlog(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowiplogshown[iip] = segm[iip];
    }
    showiplogshown++;
    timeshowiplogshown = now();
  }
  textStringLog("current iplogging: ", false);
  iplog ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" iplogging switched to: ", false);
  if (iplog == false) {
    iplog = true;
  }
  else {
    iplog = false;
  }
  iplog ? textStringLog("on", true) : textStringLog("off", true);
  client.print(F("<p>The new status for iplogging is: "));
  if (iplog == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(". </p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(iplog_addr);
  if (!flash.writeByte(iplog_addr, iplog)) {
    flash_error = F("writeByte iplog_addr FAIL");
    StoreFlashError(flash_error, iplog_addr);
  }
  delay(3);
#endif
}

void showWeatherlog(EthernetClient client, IPAddress segm) {
  textStringLog("showbuffer switched to: ", false);
  if (showbuffer == false) {
    showbuffer = true;
  }
  else {
    showbuffer = false;
  }
  showbuffer ? textStringLog("on", true) : textStringLog("off", true);
#ifdef OPTION_FLASH
  flash.eraseSector(showbuffer_addr);
  if (!flash.writeByte(showbuffer_addr, showbuffer)) {
    flash_error = "writeByte showbuffer_addr FAIL";

    StoreFlashError(flash_error, showbuffer_addr);
  }
  delay(3);
#endif
  client.print(F("<p>The new status for \"Show buffer weather on Serial\" is: "));
  if (showbuffer == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowweatherlogshown[iip] = segm[iip];
    }
    showweatherlogshown++;
    timeshowweatherlogshown = now();
  }
}


void SaveYield(EthernetClient client, IPAddress segm) {
  StoreDayYield(day(), month());
  client.println(F("<p>Yield for this day stored.</p>\r\n</body>\r\n</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipsaveyieldshown[iip] = segm[iip];
    }
    saveyieldshown++;
    timesaveyieldshown = now();
  }
}

void SaveValue(EthernetClient client, IPAddress segm) {
  client.println(F("<p>Start storing variables for this day to prepare for restore.</p>\r\n"));
  StoreDayTotal();
  client.println(F("<p>Variables for this day stored for restore.</p>\r\n</body>\r\n</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipsavevalueshown[iip] = segm[iip];
    }
    savevalueshown++;
    timesavevalueshown = now();
  }
}

void showIP(EthernetClient client, IPAddress segm) {
  GetWANIPAddress();
  client << (F("<p>WAN IP address: ")) << myIP << (F("<br>\r\n"));
  sprintf(webData, "%02X-%02X-%02X-%02X-%02X-%02X<br>\r\n", macBuffer[0], macBuffer[1], macBuffer[2], macBuffer[3], macBuffer[4], macBuffer[5]);  //webdata is max 96
  client << (F("Physical address: ")) << webData;
  client << (F("LAN IP address: ")) << Ethernet.localIP() << (F("</p>\r\n</body>\r\n</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowipshown[iip] = segm[iip];
    }
    showipshown++;
    timeshowipshown = now();
  }
}

void showversion(EthernetClient client, bool endtable, IPAddress segm) {
  client << (F("<table border=\"1\" cellspacing=\"0\">\r\n")) ;
  client << F("<tr><td colspan=\"11\">&copy; ") << F(NAME) << ' ' << F(WELCOME) << (F(" PVI version ")) << F(PVIVersion) << F(Release);
  client << F("<br>\r\nRemember 31 mai 2018 A.L. H - d H, 3 december 2019 Cleo");
  client << F("<br>\r\n --- Build: ") << F(COMPILEDATE) << (F("  ")) << F(COMPILETIME) << ' ';
#if defined(ARDUINO)
  client << F("with ARDUINO version: ") << (ARDUINO);
#endif
  client << (F(" --- </tr>"));
  if (endtable) {
    client << (F("</table>\r\n</body>\r\n</html>"));
  }
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowversionshown[iip] = segm[iip];
    }
    showversionshown++;
    timeshowversionshown = now();
  }
}

void sendRobotstxt(EthernetClient client, IPAddress segm) {
  client << (F("HTTP/1.1 200 OK\r\n"));
  client << (F("Content-Type: text/plain; charset=utf-8\r\n"));
  client << (F("Connection: close\r\n\r\n"));
  client << (F("User-agent: *\nDisallow: /\n"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipsendRobotstxtshown[iip] = segm[iip];
    }
    sendRobotstxtshown++;
    timesendRobotstxtshown = now();
  }
}

void send404(EthernetClient client, IPAddress segm) {
  client << (F("HTTP/1.1 404 Not Found\r\n"));
  client << (F("Content-Type: text/html; charset=UTF-8\r\n"));
  client << (F("Connection: close\r\n\r\n"));
  client << (F("<!DOCTYPE HTML>\r\n"));
  client << (F("<html lang=en>\r\n"));
  client << (F("<meta charset=utf-8\">\r\n"));
  client << (F("<title>Error 404 (Not Found)!!!</title>\r\n"));
  client << (F("<p>404 Not Found\r\nThe requested URL was not found on this server.</p>\r\n"));
  client << (F("</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipsend404shown[iip] = segm[iip];
    }
    send404shown++;
    timesend404shown = now();
  }
}

void DumpYield(EthernetClient client, int rm, IPAddress segm) {
  byte monthIndex;
  byte dmpmonth = month();
  int dmpyear = year();
  if (dmpmonth > 2)
  {
    dmpmonth = dmpmonth - 2; //pointer to 2 month back
  }
  else
  {
    dmpmonth += 10;
    dmpyear -= 1;
  }
#ifdef OPTION_FLASH
  if ( rm == 0 ) {
    client << (F("<p>Function readed from RAM array instead of flashmemory on STM32 platform<br>\r\n<br>\r\n"));
  }
  else {
    client << (F("<p>Function readed from flashmemory on STM32 platform<br>\r\n<br>\r\n"));
  }
#else
  client << (F("<p>Function readed from RAM array instead of EEPROM on STM32 platform<br>\r\n<br>\r\n"));
#endif
  client << (F("<p>Power generated last three months, starting from "));
  client << dmpmonth << '-' << dmpyear << (F("</p><br>\r\n"));
  client << F("<p style=\"font-family: DejaVu Sans Mono, monospace; font-size:1em;\">");
  unsigned long loWord;
  int datint;
  for (byte x = 0; x < 3; x++)
  {
    monthIndex = (dmpmonth - 1) % 3;
    for (byte i = 1; i <= 31; i++)
    {
#ifdef OPTION_FLASH
      datint = monthBase_addr + (((monthIndex * 31) + (i - 1)) * EEPROM_PAGE_SIZE);
      loWord = flash.readULong(datint);
      if ( rm == 0 ) {
#endif
        loWord = dayYield[(monthIndex * 31) + (i - 1)];
#ifdef OPTION_FLASH
      }
#endif
      if ((loWord != 0) && (loWord != 4294967295))
      {
        sprintf(msgchararray, "%02d-%02d-%04d;", i, dmpmonth, dmpyear); //is max 64 char
        client << msgchararray << loWord << (F(";<br>\r\n"));
      }
    }
    dmpmonth++;
    if (dmpmonth > 11)
    {
      dmpmonth = 1;
      dmpyear++;
    }
  }
  client << br << (F("</p>\r\n</body>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipdumpyieldshown[iip] = segm[iip];
    }
    dumpyieldshown++;
    timedumpyieldshown = now();
  }
}

void dump_Flash_Row(EthernetClient client, unsigned long startaddres) {
#ifdef OPTION_FLASH
  char asciichar[17];
  int acpos = 0;
  char karakter;
  for (unsigned long i = startaddres; i <= startaddres + 15; i++) {
    // if this is the first byte of the row,
    // start row by printing the byte address
    if (i == startaddres) {
      sprintf(msgchararray, "%10d:%08X ", i, i);  // msgchararray is max 64 char
      client << msgchararray;
      // Serial.print(msgchararray);
    }
    // read current byte from flash
    // write byte in hex form
    karakter = flash.readByte(i);
    sprintf(msgchararray, "%02X ", karakter); // msgchararray is max 64 char
    if (((byte)karakter > 0x1F) &&  ((byte)karakter < 0x7F)) {
      asciichar[acpos] = karakter;
    }
    else {
      asciichar[acpos] = 0x2E;
    }
    acpos++;
    // if this is the last byte of the row,
    // reset row counter and use println()
    // to start a new line
    if (i == startaddres + 15) {
      asciichar[acpos] = 0x00;
      client << msgchararray << F("        ") << asciichar << F("\r\n");
      //      Serial.println(msgchararray);
      acpos = 0;
    }
    // else just print the hex value with print()
    else {
      client << msgchararray;
      //      Serial.print(msgchararray);
    }
  }
#endif
}

void dump_Flash_Row2Serial(unsigned long startaddres) {
#ifdef OPTION_FLASH
  char asciichar[17];
  int acpos = 0;
  char karakter;
  for (unsigned long i = startaddres; i <= startaddres + 15; i++) {
    // if this is the first byte of the row,
    // start row by printing the byte address
    if (i == startaddres) {
      sprintf(msgchararray, "%8d:%08X ", i, i);  // msgchararray is max 64 char
      Serial.print(msgchararray);
    }
    // read current byte from flash
    // write byte in hex form
    karakter = flash.readByte(i);
    sprintf(msgchararray, "%02X ", karakter); // msgchararray is max 64 char
    if (((byte)karakter > 0x1F) &&  ((byte)karakter < 0x7F)) {
      asciichar[acpos] = karakter;
    }
    else {
      asciichar[acpos] = 0x2E;
    }
    acpos++;
    // if this is the last byte of the row,
    // reset row counter and use println()
    // to start a new line
    if (i == startaddres + 15) {
      //      client << msgchararray << br << F("\r\n");
      asciichar[acpos] = 0x00;
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
#endif
}

//
// Dump eeprom memory contents over ethernet client in tabular form.
// Each printed row shows the value of bytesPerRow bytes
// (by default 16).
//
void flash_serial_dump_table(EthernetClient client, IPAddress rsegm) {
  IPAddress lsegm = Ethernet.localIP();
  int bytesPerRow = 16;
  // address counter
  int i;
  // row bytes counter
  int j;
  // byte read from eeprom
  byte b;
  // temporary buffer for sprintf
  //  char buf[11]; replaced by msgchararray
  // initialize column counter
  j = 0;
  client << (F("<pre style=\"font-family: DejaVu Sans Mono, monospace; font-size:1em; text-align:left; color:#000; background-color:#fff;\">"));
#if defined(ARCH_STM32)|| defined(ARDUINO_ARCH_ESP32) || defined(STM32F4xx)
#ifdef OPTION_FLASH
  client << F("prevActual:              ");
  dump_Flash_Row(client, prevActual_addr);
  client << F("ethrlog:                 ");
  dump_Flash_Row(client, ethrlog_addr);
  client << F("TimestartInvTemp:        ");
  dump_Flash_Row(client, TimestartInvTemp_addr);
  client << F("startInvTemp:            ");
  dump_Flash_Row(client, startInvTemp_addr);
  client << F("last_neerslagintensiteit:");
  dump_Flash_Row(client, last_neerslagintensiteit_addr);
  //  delay(5);
  client << F("lastpluvioTime_br:       ");
  dump_Flash_Row(client, lastpluvioTime_br_addr);
  client << F("max_neerslagintensiteit: ");
  dump_Flash_Row(client, max_neerslagintensiteit_addr);
  client << F("maxpluvioTime_br:        ");
  dump_Flash_Row(client, maxpluvioTime_br_addr);
  client << F("showbuffer:              ");
  dump_Flash_Row(client, showbuffer_addr);
  client << F("min_serial:              ");
  dump_Flash_Row(client, min_serial_addr);
  delay(5);
  client << F("DYNDNS_API_KEY:                     sorry...., only shown on serial port\r\n");
  Serial.print("\r\nDYNDNS_API_KEY:          ");
  dump_Flash_Row2Serial(DYNDNS_API_KEY_addr);
  Serial.print(F("                         "));
  dump_Flash_Row2Serial(DYNDNS_API_KEY_addr + 16);
  Serial.print(F("                         "));
  dump_Flash_Row2Serial(DYNDNS_API_KEY_addr + 32);
  client << F("eepromOffset:            ");
  dump_Flash_Row(client, eepromOffset_addr);
  client << F("totPluvio_htn:           ");
  dump_Flash_Row(client, totPluvio_htn_addr);
  //  delay(5);
  client << F("runTimefanA:             ");
  dump_Flash_Row(client, runTimefanA_addr);
  client << F("runTimefanB:             ");
  dump_Flash_Row(client, runTimefanB_addr);
  client << F("runTimefanC:             ");
  dump_Flash_Row(client, runTimefanC_addr);
  client << F("shadow:                  ");
  dump_Flash_Row(client, shadow_addr);
  client << F("fanAOnCount:             ");
  dump_Flash_Row(client, fanAOnCount_addr);
  //  delay(5);
  client << F("fanBOnCount:             ");
  dump_Flash_Row(client, fanBOnCount_addr);
  client << F("fanCOnCount:             ");
  dump_Flash_Row(client, fanCOnCount_addr);
  client << F("runTimeThisDayfanA:      ");
  dump_Flash_Row(client, runTimeThisDayfanA_addr);
  client << F("runTimeThisDayfanB:      ");
  dump_Flash_Row(client, runTimeThisDayfanB_addr);
  client << F("runTimeThisDayfanC:      ");
  //  delay(5);
  dump_Flash_Row(client, runTimeThisDayfanC_addr);
  client << F("TimeMinTemp:             ");
  dump_Flash_Row(client, TimeMinTemp_addr);
  client << F("TimeMaxTemp:             ");
  dump_Flash_Row(client, TimeMaxTemp_addr);
  client << F("TimePeakTemp:            ");
  dump_Flash_Row(client, TimePeakTemp_addr);
  client << F("tempFanAOff:             ");
  dump_Flash_Row(client, tempFanAOff_addr);
  client << F("tempFanBOff:             ");
  dump_Flash_Row(client, tempFanBOff_addr);
  //  delay(5);
  client << F("tempFanCOff:             ");
  dump_Flash_Row(client, tempFanCOff_addr);
  client << F("tempFanAOn:              ");
  dump_Flash_Row(client, tempFanAOn_addr);
  client << F("tempFanBOn:              ");
  dump_Flash_Row(client, tempFanBOn_addr);
  client << F("tempFanCOn:              ");
  dump_Flash_Row(client, tempFanCOn_addr);
  client << F("saveFlag:                ");
  dump_Flash_Row(client, saveFlag_addr);
  //  delay(5);
  client << F("maxInvTemp:              ");
  dump_Flash_Row(client, maxInvTemp_addr);
  client << F("TimePeakPowerIn:         ");
  dump_Flash_Row(client, TimePeakPowerIn_addr);
  client << F("PeakPowerInObserved:     ");
  dump_Flash_Row(client, PeakPowerInObserved_addr);
  client << F("fanAOnTotalCount:        ");
  dump_Flash_Row(client, fanAOnTotalCount_addr);
  client << F("fanBOnTotalCount:        ");
  //  delay(5);
  dump_Flash_Row(client, fanBOnTotalCount_addr);
  client << F("fanCOnTotalCount:        ");
  dump_Flash_Row(client, fanCOnTotalCount_addr);
  client << F("monthBase for 3 months\r\n");
  //  delay(5);
  unsigned long membase = monthBase_addr;
  for (int rr = 0; rr < 93 ; rr++) {
    if (rr % 10 == 0) {
      //      delay(25);
    }
    sprintf(msgchararray, "day position: %02d         ", rr);  // msgchararray is max 64 char
    client << msgchararray;
    dump_Flash_Row(client, membase);//  (deze 93 keer uitlezen met EEPROM_PAGE_SIZE)
    membase += EEPROM_PAGE_SIZE;
  }
  client << F("iMonth:                  ");
  dump_Flash_Row(client, iMonth_addr);
  client << F("monthIndexBase not used: ");
  dump_Flash_Row(client, monthIndexBase_addr);
  client << F("maxtotalToday:           ");
  dump_Flash_Row(client, maxtotalToday_addr);
  client << F("TimePeakPower:           ");
  dump_Flash_Row(client, TimePeakPower_addr);
  client << F("MaxcmltvPwr:             ");
  //  delay(5);
  dump_Flash_Row(client, MaxcmltvPwr_addr);
  client << F("invRiso:                 ");
  dump_Flash_Row(client, invRiso_addr);
  client << F("MaxTemp:                 ");
  dump_Flash_Row(client, MaxTemp_addr);
  client << F("MinTemp:                 ");
  dump_Flash_Row(client, MinTemp_addr);
  client << F("luchtvochtigheid:        ");
  dump_Flash_Row(client, luchtvochtigheid_addr);
  //  delay(5);
  client << F("AlreadyAdded:            ");
  dump_Flash_Row(client, AlreadyAdded_addr);
  client << F("totPluvio_br:            ");
  dump_Flash_Row(client, totPluvio_br_addr);
  client << F("wCLength:                ");
  dump_Flash_Row(client, wCLength_addr);
  client << F("weatherCondition1300:    ");
  dump_Flash_Row(client, weatherCondition1300_addr);
  if (wCLength > 12 ) {
    client << F("weatherCondition1300:    ");
    dump_Flash_Row(client, weatherCondition1300_addr + 16);
  }
  if (wCLength > 28 ) {
    client << F("weatherCondition1300:    ");
    dump_Flash_Row(client, weatherCondition1300_addr + 32);
  }
  if (wCLength > 44 ) {
    client << F("weatherCondition1300:    ");
    dump_Flash_Row(client, weatherCondition1300_addr + 48);
  }
  client << F("AlreadyDailySend:        ");
  dump_Flash_Row(client, AlreadyDailySend_addr);
  client << F("AlreadyMonthSend:        ");
  dump_Flash_Row(client, AlreadyMonthSend_addr);
  client << F("iplog                    ");
  dump_Flash_Row(client, iplog_addr);
  client << F("maillog                  ");
  dump_Flash_Row(client, maillogging_addr);
  client << F("pvoutputlog              ");
  dump_Flash_Row(client, pvoutputlog_addr);
  client << F("showbufferremaining      ");
  dump_Flash_Row(client, showbufferremaining_addr);
  client << F("udpdump                  ");
  dump_Flash_Row(client, udpdump_addr);
  client << F("HTTPlog                  ");
  dump_Flash_Row(client, HTTPlog_addr);
  client << F("firstShadowTime          ");
  dump_Flash_Row(client, firstShadowTime_addr);
  client << F("lastShadowTime           ");
  dump_Flash_Row(client, lastShadowTime_addr);
  client << F("pluvio_ena               ");
  dump_Flash_Row(client, pluvio_ena_addr);
  client << F("invlogging               ");
  dump_Flash_Row(client, invlogging_addr);
  client << F("brlogging                ");
  dump_Flash_Row(client, brlogging_addr);
  client << F("wolfSSLdbg               ");
  dump_Flash_Row(client, wolfSSLdbg_addr);
  client << F("WolfSSLrwmail            ");
  dump_Flash_Row(client, wolfSSLMail_addr);
  client << F("abuslogging              ");
  dump_Flash_Row(client, abuslogging_addr);
  client << F("suspicious_mail          ");
  dump_Flash_Row(client, suspicious_mail_addr);
  client << F("unknownCmd_mail          ");
  dump_Flash_Row(client, unknownCmd_mail_addr);
  client << F("BrCorrFact               ");
  dump_Flash_Row(client, BrCorrFact_addr);
  sprintf(temptxtbuff, "lsegm[%d] == rsegm[%d], lsegm[%d] == rsegm[%d], rsegm[%d] <= 16", lsegm[0], rsegm[0], lsegm[1], rsegm[1], rsegm[2]);
  textlog(temptxtbuff, false);
  if ((lsegm[0] == rsegm[0]) && (lsegm[1] == rsegm[1]) && (rsegm[2] <= 16)) {
    sprintf(temptxtbuff, "\r\nOK, we are on the local subnet.... value on membase: 0x%02X, value on membase+16: 0x%02X", flash.readByte(membase), flash.readByte(membase + 16));
    textlog(temptxtbuff, false);
    membase = abusefilter1_addr;
    int rr = 0;
    uint8_t filterpage = 0;
    while ((flash.readByte(membase) != 0xFF) && (flash.readByte(membase + 16) != 0xFF)) {
      //      if (rr % 20 == 0) {
      //        delay(25);
      //      }
      //      rr++;
      if (((membase - abusefilter1_addr) % EEPROM_PAGE_SIZE) == 0) {
        filterpage++;
      }
      sprintf(temptxtbuff, "abusefilter%02d            ", filterpage);
      client << temptxtbuff;
      dump_Flash_Row(client, membase);//  (deze  uitlezen met EEPROM_PAGE_SIZE)
      if (membase >= abusefilter1_addr + (12 * EEPROM_PAGE_SIZE)) {
        sprintf(temptxtbuff, "\r\nINFO end of abuseIP filter flash space reached @ address: 0x%08X, value of membase: 0x%08X", abusefilter12_addr + EEPROM_PAGE_SIZE, membase);
        textlog(temptxtbuff, false);
        break;
      }
      membase += 16;
    }
    membase = HTTPblacklist1_addr;
    rr = 0;
    filterpage = 0;
    while ((flash.readByte(membase) != 0xFF) && (flash.readByte(membase + 16) != 0xFF)) {
      //      if (rr % 20 == 0) {
      //                delay(25);
      //      }
      //      rr++;
      if (((membase - HTTPblacklist1_addr) % EEPROM_PAGE_SIZE) == 0) {
        filterpage++;
      }
      sprintf(temptxtbuff, "HTTPblacklist%02d          ", filterpage);
      client << temptxtbuff;
      dump_Flash_Row(client, membase);//  (deze  uitlezen met EEPROM_PAGE_SIZE)
      membase += 16;
      if (membase >= HTTPblacklist1_addr + (2 * EEPROM_PAGE_SIZE)) {
        sprintf(temptxtbuff, "\r\nINFO end of HTTP blacklist flash space reached @ address: 0x%08X, value of membase: 0x%08X", HTTPblacklist2_addr + EEPROM_PAGE_SIZE, membase);
        textlog(temptxtbuff, false);
        break;
      }
    }
    client << F("Import/Export logging    ");
    dump_Flash_Row(client, InExlog_addr);
    client << F("eepromEnd\r\n");
    membase = eepromEnd_addr;
    rr = 0;
    sprintf(temptxtbuff, "\r\n TFTPbuffer value on membase: 0x%02X, value on membase+16: 0x%02X", flash.readByte(membase));
    textlog(temptxtbuff, false);
    while (flash.readByte(membase) != 0xFF) {
      if (rr % 20 == 0) {
        delay(25);
      }
      rr++;
      client << F("TFTPbuffer               ");
      dump_Flash_Row(client, membase);//  (deze  uitlezen met EEPROM_PAGE_SIZE)
      if (rr == 12000 ) break;
      membase += 16;
    }
  }
#else
  client << (F("No flash available"));
#endif
#endif
  client << (F("\r\n</pre></body>\r\n</html>"));
  if (!isIPself(rsegm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipflash_serial_dump_tableshown[iip] = rsegm[iip];
    }
    flash_serial_dump_tableshown++;
    timeflash_serial_dump_tableshown = now();
  }
}

void UpdateTimeWeb(EthernetClient client, IPAddress segm) {
  client << (F("<p>Current status of time: ")) << timeStatus();
  client << (F("<br>\r\nRequesting new time..."));
  UpdateTime(); // reload the ntp time
  client << (F("<br>\r\nTime resync@; ")) << DateTime(now());
  client << (F("<br>\r\ntimeStatus: ")) << timeStatus() << (F(" value if succesfull: ")) << timeSet;
  client << (F("<br>\r\n</p>\r\n</body>\r\n</html>"));
  if (!min_serial && (lastSecond > 5)) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Polling PVI: ", hour(), minute(), second());  // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipupdatetimewebshown[iip] = segm[iip];
    }
    updatetimewebshown++;
    timeupdatetimewebshown = now();
  }
}

void help(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  if (HTTPlog) {
    textStringLog("\r\nethernet TX buffer available at start help: ", false);
    itoa(client.availableForWrite(), cobuff, 10);
    textlog(cobuff, true);
  }
  client << (F("<p>Local time: ")) << DateTime(now());
  client << (F("<br>\r\nPossible commands: <br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?dump\">dump</a> - hexdump the content of the flash<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?erase\">erase</a> - erases the saved day total (no restore @startup)<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?Ethernetlogging\">Ethernetlogging</a> - toggles Ethernet socket logging on/off<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?fan\">fan</a> - shows the fan temperature switches<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?fandefault\">fandefault</a> - sets the default thresholds for fan temperature<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?forceEmail\">forceEmail</a> - Sends an end of day and monthly yield message<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?GoUpdateDNS\">GoUpdateDNS</a> - updates the DynDNS<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?help\">help</a> - shows this message<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showinverterlog\">showinverterlog</a> - toggles the inverter logging<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?ntp\">ntp</a> - syncs the time<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?PluvioEna\">PluvioEna</a> - Enable/Disable reading pluvio from buienradar.nl<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?pvoutputlog\">pvoutputlog</a> - shows the logging to PVOutput.org site<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?RAMyield\">RAMyield</a> - Reads the yield from RAM array instead Flashmemory<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?save\">save</a> - stores the day all vars<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?saveyield\">saveyield</a> - stores the current yield of the day<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?Seriallogging\">Seriallogging</a> - toggles the serial logging <br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?setBrCorrFact\">setBrCorrFact=1.00</a> - Sets the Buienradar pluvio correction factor<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showWeatherlog\">showWeatherlog</a> - toggles the serial logging for weather data<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showabuslog\">showabuslog</a> - toggles the logging to AbuseIPDB<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showbrlog\">showbrlog</a> - toggles the buienradar.nl logging for pluvio<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showbufferremaining\">showbufferremaining</a> - toggles the serial remaining buffer logging for mail transactions<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showextended\">showextended</a> - shows the status with debug counters included<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showHTTPlogging\">showHTTPlogging</a> - toggles logging from HTTP requests<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showip\">showip</a> - shows current IP address<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showiplog\">showiplog</a> - toggles logging abount IP address requests and dynDNS activity<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showlogging\">showlogging</a> - shows the logging settings<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showmaillog\">showmaillog</a> - shows the logging of the STMP dialog<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showmem\">showmem</a> - shows STM32 memory usage<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showPluvioURL\">showPluvioURL</a> - shows the url to get the pluvio data<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showPVIdebug\">showPVIdebug</a> - shows the inverter values<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showPVOstring\">showPVOstring</a> - shows the update string for PVoutput.org<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showPVOutputlog\">showPVOutputlog</a> - shows the logging for PVoutput.org<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showsockets\">showsockets</a> - shows the socketstatus of the W5500<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showsuspectURL\">showsuspectURL</a> - shows the last suspected URL commands<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?showunknowncmd\">showunknowncmd</a> - shows URL commands that not could correct interpreted<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?startTFTP\">startTFTP</a> - starts TFTP server for loading HTML or IPtable list<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?SuspEmail\">SuspEmail</a> - Enables email sending for suspicious commands<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?testHTTPblacklist\">testHTTPblacklist</a> - Returns if the HTML commandline contain a suspicious verb or noun or part<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?testIPwhitelist\">testIPwhitelist</a> - Returns if an IP net is whitelisted.<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?UnknownCmdEmail\">UnknownCmdEmail</a> - Enables email sending for unknown commands<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?UDPon\">UDPon</a> - enables a UDP socket, remote must listen on port: 5555<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?UDPoff\">UDPoff</a> - disables UDP logging and frees the socket<br>\r\n"));
  client << (F("UpdateDNSKey - updates the DynDNS update key: /?UpdateDNSKey:oldkey=newkey<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?version\">version</a> - show current version<br>\r\n"));
  client << (F("<a href=\"http://baaisolar.boxathome.net:888/?yield\">yield</a> - shows the yield for the last three monthes per day<br>\r\n"));
  client << (F("fandefault - sets the default fan temperature switches of the fans<br>\r\n"));
  client << (F("Value for xxxx between 3500 - 5000, equals 35.00 - 50.00 degrees Celsius. The minus sign is for switch off<br>\r\n"));
  client << (F("A=xxxx - set the the temp for fan A to switch on<br>\r\n"));
  client << (F("B=xxxx - set the the temp for fan B to switch on<br>\r\n"));
  client << (F("C=xxxx - set the the temp for fan C to switch on<br>\r\n"));
  client << (F("A=-xxxx - set the the temp for fan A to switch off<br>\r\n"));
  client << (F("B=-xxxx - set the the temp for fan B to switch off<br>\r\n"));
  client << (F("C=-xxxx - set the the temp for fan C to switch off<br></p>\r\n"));
  client << (F("</body>\r\n</html>"));
  if (HTTPlog) {
    textStringLog("\r\nethernet TX buffer available at end of help: ", false);
    itoa(client.availableForWrite(), cobuff, 10);
    textlog(cobuff, true);
  }
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      iphelpshown[iip] = segm[iip];
    }
    helpshown++;
    timehelpshown = now();
  }
}

void showPVOstring(EthernetClient client, IPAddress segm) {
  if (maillog[0] != 0 )
  { // whipe key
    for (int i = 34 ; i < 74 ; i++ ) {
      maillog[i] = 'x';
    }
    client << (F("<p>Anonymous last PVOutput update string:<br>\r\n"));
    client << maillog << F("</p>");
  }
  else
    client << (F("<p>Sorry, no last PVOutput update string available.</p>"));
  client << (F("<br>\r\n</body>\r\n</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowpvostringshown[iip] = segm[iip];
    }
    showpvostringshown++;
    timeshowpvostringshown = now();
  }
}

void showUnknownCmd(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowunknowncmdshown[iip] = segm[iip];
    }
    showunknowncmdshown++;
    timeshowunknowncmdshown = now();
  }
  if (unknownCmd_total > 0) {
    client << (F("<table border=\"1\" cellspacing=\"0\">\r\n<caption>Following commands are received, which cold not be correct interpreted.</caption>\r\n"));
    client << F("<tr><th>Time:</th><th>From IP:</th><th>Well known IP:</th><th>command length:</th><th>Received command request:</th><th>abuseConfidenceScore:</th><th>Report to AbuseIPDB</th></tr><br>\r\n");
    for (int i = 0; i < UNKWNCMDARRAYDEPTH; i++) {
      if ((byte)unknownCmd_string[i][0] > 0) {
        client << F("<tr><td>") << DateTime(unknownCmd_time[i]) << F("</td><td>") << (IPAddress)unknownCmd_ips[i] << F("</td><td>");
        unknownCmd_wkwnips[i] ? client << "Yes" : client << "No";
        client <<  F("</td><td>") << unknownCmd_length[i] << F("</td><td>") << unknownCmd_string[i] << F("</td><td>") << unknownCmd_perc[i];
        if (unknownCmd_queued[i] == 0) {
          sprintf(temptxtbuff, "<a href=\"http://baaisolar.boxathome.net:888/?reportunkwncmd%d\">report</a>", i + 1); // temptxtbuff is max 1024
        }
        else if (unknownCmd_queued[i] == 1) {
          sprintf(temptxtbuff, "<a href=\"http://baaisolar.boxathome.net:888/?reportunkwncmd%d\">submitted</a>", i + 1);
        }
        else if (unknownCmd_queued[i] == 2) {
          sprintf(temptxtbuff, "done");
        }
        client << F("</td><td>") << temptxtbuff << F("</td></tr><br>\r\n");
      }
    }
    client << (F("</table><br>\r\n"));
  }
  else {
    client << (F("<p>No unknown commands received.</p><br>\r\n"));
  }
  client << (F("</body><br>\r\n</html>"));
}

void ShowSuspectURL(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowsuspecturlshown[iip] = segm[iip];
    }
    showsuspecturlshown++;
    timeshowsuspecturlshown = now();
  }
  if (suspicious_total > 0) {
    client << (F("<table border=\"1\" cellspacing=\"0\">\r\n<caption>Following commands are received, which contains suspicious headers or commands.</caption>\r\n"));
    client << F("<tr><th>No:</th><th>Time:</th><th>From IP:</th><th>command length:</th><th>Received command request:</th><th>abuseConfidenceScore:</th></tr>\r\n");
    for (int i = 0; i < SUSPARRAYDEPTH; i++) {
      if ((byte)suspicious_string[i][0] > 0) {
        client << F("<tr><td>") << i + 1 << F("</td><td>") << DateTime(suspicious_time[i]) << F("</td><td>") << (IPAddress)suspicious_ips[i] << F("</td><td>") << suspicious_length[i] << F("</td><td>") << suspicious_string[i] << F("</td><td>") << suspicious_perc[i] << F("</td></tr><br>\r\n");
      }
    }
    client << (F("</table>\r\n"));
  }
  else {
    client << (F("<p>No suspicious commands received.</p><br>\r\n"));
  }
  client << (F("</body><br>\r\n</html>"));
}

void ShowStatus(EthernetClient client, bool extended, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowstatusshown[iip] = segm[iip];
    }
    showstatusshown++;
    timeshowstatusshown = now();
  }
  if (extended) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowstatusextshown[iip] = segm[iip];
    }
    showstatusextshown++;
    timeshowstatusextshown = now();
  }
  client << (F("<table border=3 cellpadding=10 cellspacing=10><tr><td>\r\n"));
  showversion(client, 0, segm);
  client << F("<tr><td colspan=\"11\"><br>Sketch start time: ") << DateTime(sketchStartTime);
  client << (F(" &rArr; Uptime: ")) << upTime / (24 * 60) << "d:" << (upTime % (24 * 60)) / 60 << "h:" << upTime % 60 << (F("m<br>"));
  client << (F("Current time in Houten (NL): ")) << DateTime(now()) << (F("<br>"));
  client << (F("Weather (Dutch):&nbsp;")) << samenvatting << (F("<br>\r\n</tr>\r\n"));
  client << (F("<tr><th>Grid (V)</th><th>Strings (W)</th><th>Actual In (W)</th><th>Actual Out (W)</th><th>Peak 5 min (W)</th><th>Day peak (W)</th><th>"));
  client << (F("Today (Wh)</th><th>Total (kWh)</th><th>&eta; (%)</th><th>Temperature (&deg;C)</th><th>Isolation</th></tr>\r\n"));
  int shPwr1 = Power1 / 8;
  shPwr1 = (shPwr1 * 6) + 60; // correct the integer division
  if ( (Power2 > shPwr1) && (GridPower > 100)) {
    sprintf(msgchararray, "<span style=\"color:red\">%d</span>", Power1);; // if lower string is in shade ( and not at startup), msgchararray is max 64, 35 used
  }
  else {
    sprintf(msgchararray, "%d", Power1);
  }
  client << (F("<tr><td>")) << actTension << (F("</td><td>upper(2)=")) << Power2 << (F("<br>lower(1)=")) << msgchararray << (F("</td><td>")) << Power1 + Power2 << (F("</td><td>")) << GridPower << (F("</td><td>")) << peakpower5min << (F("</td><td>"));
  client << (F("In: ")) << PeakPowerInObserved << (F("@")) << TimePeakPowerIn << (F("<br>Out: ")) << PeakPowerObserved << (F("@")) << TimePeakPower << (F("</td><td>")) << maxtotalToday << (F("</td><td>")) << MaxcmltvPwr;
  //  delay(5);
  client << (F("</td><td>")) << rendement << (F("</td><td>booster=")) << actBstTemp;
  client << (F("<br>inverter=")) << actInvTemp << (F("</td><td>")) << invRiso << (F("M&Omega;")) << (F("</td></tr>\r\n"));
  client << (F("<tr><th>Rain (mm/h)</th><th>Rain (mm)</th><th>Wind (m/s)</th><th>direction</th><th>visibility (m)</th><th>Temperature (&deg;C)"));
  client << (F("</th><th>hPa</th><th>&Phi; @07:00</th><th>&Phi; act</th><th>Condition</th><th>Sky</th></tr>\r\n"));
  // for the weather
  client << (F("<tr><td>"));
  if (pluvio_ena == true) {
    client << pluvio_br << (F("</td><td>")) << totPluvio_br;
  }
  else {
    client << F("disabled") << (F("</th><th>")) << F("--");
  }
  client << (F("</td><td>")) << windsnelheidMS << F(" (") << windkrachtBft << (F(" Bft)</td><td>")) << windrichting << (F("</td><td>")) << visibility << (F("</td><td>")) << meetStation << '=' << actual << "<br>min=" << MinTemp;
  client << " max=" << MaxTemp << (F("</td><td>")) << luchtdruk << " "  << hPaTrend << (F("</td><td>")) << luchtvochtigheid << (F("%</td><td>")) << lv_phi << (F("%</td><td>actual&rArr;")) << weatherType;
  if ((iHour >= 13) && (weatherCondition1300 != "")) {
    client << (F("<br>@13:00&rArr;")) << weatherCondition1300;
  }
  //  client << (F("</th><th><IMG alt=\"Sky\" src=\"https://www.buienradar.nl/resources/images/icons/weather/30x30/")) << icon_temperature << (F("\"></td></tr>\r\n"));
  client << (F("</td><td>\r\n"));
  //  delay(5);
  if (selectIcon(client)) {
    client << (F("<img src=\"data:image/png;base64, "));
    showIcon(client);
    client << (F("\" alt=\"")) << icon_temperature << (F(".png\" >"));
  }
  else {
    client << (F("icon: ")) << icon_temperature << (F("<br>not found"));
  }
  client << (F("</td></tr>\r\n"));
  //  delay(5);
  client << (F("<tr><td valign = \"Top\" align = \"center\" colspan=\"5\"><table border=\"1\" cellspacing=\"0\"> <caption>Status &amp; Counters</caption>\r\n"));
  client << (F("<tr><td>PVI polled:</td><td>")) << PVIPolled << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>CRCErr:</td><td>")) << ChkSumErrCnt << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Rx timeouts:</td><td>")) << cntrespTimeout << (F("</tr>\r\n"));
    //  client << (F("<tr><td>Rx charcount by timeout:<td>")) << rxReadCnt << (F("</tr>\r\n"));
    client << (F("<tr><td>minRxTime: (ms)</td><td>"));
    if (minRxTime == receiveMaxRS485Wait) {
      client << "0</td></tr>";
    }
    else {
      client << minRxTime << (F("</td></tr>"));
    }
    client << (F("\r\n<tr><td>maxRxTime: (ms)</td><td>")) << maxRxTime << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>RxTime last reset:</td><td>"));
  if ( resetRxTime != 0 )
    client << DateTime(resetRxTime);
  else
    client << (F("Never"));
  client <<  (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Actual RS485 timeout:</td><td>")) << receiveMaxRS485Wait << (F("ms</td></tr>\r\n"));
  }
  client << (F("<tr><td>Inverter temp @wakeup:</td><td>")) << startInvTemp << F("&deg;C @ ");
  if (TimestartInvTemp[0] != 0)
    client << TimestartInvTemp;
  else
    client << (F("Never"));
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Max inverter temp:</td><td>")) << maxInvTemp << F("&deg;C @ ");
  if ((TimePeakTemp[0] != '0') && (TimePeakTemp[1] != '0'))
    client <<  TimePeakTemp;
  else
    client << (F("Never"));
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Min temperature:</td><td>")) << MinTemp << F("&deg;C @ ") << TimeMinTemp << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Max temperature:</td><td>")) << MaxTemp << F("&deg;C @ ") << TimeMaxTemp << (F("</td></tr>\r\n"));
  //  delay(5);
  client << (F("<tr><td>Lower string in shadow:</td><td>"));
  if ( shadow == 0 ) {
    client << F("no");
  }
  else
  {
    client << F("<span style=\"color:red\">today ") << shadow << F(" minutes</span>");
  }
  client << F("</td></tr>\r\n");
  client << (F("<tr><td>Start time shadow lower string:</td><td>")) << firstShadowTime << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Last time shadow lower string:</td><td>")) << lastShadowTime << (F("</td></tr>\r\n"));
  client <<  F("<tr><td>Max pluvio:</td><td>") << max_neerslagintensiteit << F("mm/hr @ ");
  if ( (maxpluvioTime_br != -4294967296) && (maxpluvioTime_br !=  0) )
    client << DateTime(maxpluvioTime_br);
  else
    client << (F("Never"));
  client << (F("</td></tr>\r\n"));
  client <<  F("<tr><td>Last pluvio:</td><td>") << last_neerslagintensiteit << F("mm/hr @ ");
  if ( (lastpluvioTime_br != -4294967296) && (lastpluvioTime_br != 0) )
    client << DateTime(lastpluvioTime_br);
  else
    client << (F("Never"));
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Next pluvio:</td><td>")) << nextpluvioTime_br << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Total expected pluvio:</td><td>until ")) << lastarrtime << F(" ") << totexpectedPluvio_br << (F("mm</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Actual DC tension upper:</td><td>")) << actDc2Voltage << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Actual DC current upper:</td><td>")) << actDc2Current << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Actual DC tension lower:</td><td>")) << actDc1Voltage << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Actual DC current lower:</td><td>")) << actDc1Current << (F("</td></tr>\r\n"));
    delay(10);
  }
  client << (F("<tr><td>Fan A status:</td><td>"));
  if (digitalRead(Relay_Power) == RELAY_ON)
  {
    client << F("on, since ") << runTimeThisCyclefanA / 60 << (F("h:")) << runTimeThisCyclefanA % 60 << 'm';
  }
  else
  {
    client << (F("off"));
  }
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Fan B status:</td><td>"));
  if (digitalRead(Relay_fanB) == RELAY_ON)
  {
    client << F("on, since ") << runTimeThisCyclefanB / 60 << F("h:") << runTimeThisCyclefanB % 60 << 'm';
  }
  else
  {
    client << F("off");
  }
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Fan C status:</td><td>"));
  if (digitalRead(Relay_fanC) == RELAY_ON)
  {
    client << (F("on, since ")) << runTimeThisCyclefanC / 60 << (F("h:")) << runTimeThisCyclefanC % 60 << (F("m"));
  }
  else
  {
    client << (F("off"));
  }
  client << (F("</td></tr>\r\n"));
  //  delay(5);
  client << (F("<tr><td>Today fan A \"On\" counter:</td><td>")) << fanAOnCount << (F(", runtime: ")) << runTimeThisDayfanA / 60 << "h:" << runTimeThisDayfanA % 60 << (F("m</td></tr>\r\n"));
  client << (F("<tr><td>Today fan B \"On\" counter:</td><td>")) << fanBOnCount << (F(", runtime: ")) << runTimeThisDayfanB / 60 << "h:" << runTimeThisDayfanB % 60 << (F("m</td></tr>\r\n"));
  client << (F("<tr><td>Today fan C \"On\" counter:</td><td>")) << fanCOnCount << (F(", runtime: ")) << runTimeThisDayfanC / 60 << "h:" << runTimeThisDayfanC % 60 << (F("m</td></tr>\r\n"));
  client << (F("<tr><td>Total fan A runtime</td><td>")) << runTimefanA / 60 << "h:" << runTimefanA % 60 << F("m cnt: ") << fanAOnTotalCount << F("</td></tr>\r\n");
  client << (F("<tr><td>Total fan B runtime</td><td>")) << runTimefanB / 60 << "h:" << runTimefanB % 60 << F("m cnt: ") << fanBOnTotalCount << F("</td></tr>\r\n");
  client << (F("<tr><td>Total fan C runtime</td><td>")) << runTimefanC / 60 << "h:" << runTimefanC % 60 << F("m cnt: ") << fanCOnTotalCount << F("</td></tr>\r\n");
  client << (F("<tr><td>Last poll weather:</td><td>")) << c_tijd_Temp << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Weather polled:</td><td>")) << todayCnt;
  if ( todayErrCnt > 0 ) {
    client << F(", Errors: ") << todayErrCnt;
  }
  client << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Weather processing time:</td><td>")) << weatherProcTime << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Last weather error:</td><td>"));
    if ( todayErrCnt == 0 )
      client << (F("No errors"));
    else
      client << weatherErrTime_br;
    client << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>Last poll Pluvio:</td><td>")) <<  c_tijd_Pluv << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Pluvio polled:</td><td>")) << todayCnt_br;
  if (todayErrCnt_br > 0) {
    client << (F(", Errors: ")) << todayErrCnt_br;
  }
  client << (F("</td></tr>\r\n"));
  if (HTTPlog) {
    sprintf(temptxtbuff, "\r\nethernet TX buffer available after sending \"Total errors since restart\": %d", client.availableForWrite());
    textlog(temptxtbuff, false);
  }
  client << (F("<tr><td>Pluvio errors since restart:</td><td>")) <<  totalErrCnt_br << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Pluvio processing time Br:</td><td>")) << pluvioProcTime_br  << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>Pluvio successive value used:</td><td>"));
  if ( pluviosucc_br == 0 ) {
    client << (F("None"));
  }
  else {
    client << pluviosucc_br << (F(", last index used: ")) << pluviosuccTime_br;
  }
  client << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Last Pluvio error Br:</td><td>"));
    if ( todayErrCnt_br == 0 )
      client << (F("No errors"));
    else {
      client << pluvioErrTime_br;
    }
    client << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Pluvio max fail error:</td><td>"));
    if ( MaxFailErrCnt_br == 0 )
      client << (F("No errors"));
    else {
      client << MaxFailErrCnt_br;
    }
    client << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>WolfSSL write errors:</td><td>")) << errorwolfsslwantwrite << (F("</td></tr>\r\n"));
  client << (F("<tr><td>WolfSSL read errors:</td><td>")) << errorwolfsslwantread << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Last Pluvio RX packet Size:</td><td>"));
    client << pluvio_last_RXdata_size << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Min Pluvio RX data Size:</td><td>"));
    client << pluvio_min_RXdata_size << (F(" @ ")) << DateTime(pluvio_min_RXdata_time) << F("</td></tr>\r\n");
    client << (F("<tr><td>Max Pluvio RX data Size:</td><td>"));
    client << pluvio_max_RXdata_size << (F(" @ ")) << DateTime(pluvio_max_RXdata_time) << F("</td></tr>\r\n");
    client << (F("<tr><td>Last Pluvio HTTP header Size:</td><td>"));
    client << pluvio_last_HTTP_headersize << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Min Pluvio HTTP header Size:</td><td>"));
    client << pluvio_min_HTTP_headersize << (F(" @ ")) << DateTime(pluvio_min_HTTP_header_time) << F("</td></tr>\r\n");
    client << (F("<tr><td>Max Pluvio HTTP header Size:</td><td>"));
    client << pluvio_max_HTML_headersize << (F(" @ ")) << DateTime(pluvio_max_HTTP_header_time) << F("</td></tr>\r\n");
    delay(5);
  }
  client << (F("<tr><td>Last PvAddStatus success:</td><td>")) << pv200Response << (F(" @ "));
  if ( pv200ResponseTime != 0 )
    client << DateTime(pv200ResponseTime);
  else
    client << (F("Never"));
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Last PvAddOutput success:</td><td>")) << pvAdd200Response << (F(" @ "));
  if ( pvAdd200ResponseTime != 0 )
    client << DateTime(pvAdd200ResponseTime);
  else
    client << (F("Never"));
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Today succesfull uploads:</td><td>")) << uploadsOk  << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Today upload errors:</td><td>")) << uploadErr << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Total succesfull uploads:</td><td>")) << totalUploadsOk  << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Total upload errors:</td><td>")) << totalUploadsErr << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Last PvOutput fail:</td><td>")) << pvFailResponse << (F(" @ "));
  if ( pvResponseTime != 0 )
    client << DateTime(pvResponseTime);
  else
    client << (F("Never"));
  client << (F("</td></tr>\r\n"));
  if (extended) {
    client << F("<tr><td>PVOutput AlreadyAdded status:</td><td>");
    if (AlreadyAdded) {
      client << F("True");
    }
    else {
      client << F("False");
    }
    client << (F("</td></tr>\r\n"));
    client << F("<tr><td>Email AlreadyDailySend status:</td><td>");
    if (AlreadyDailySend) {
      client << F("True");
    }
    else {
      client << F("False");
    }
    client << (F("</td></tr>\r\n"));
    client << F("<tr><td>Email AlreadyMonthSend status:</td><td>");
    if (AlreadyMonthSend) {
      client << F("True");
    }
    else {
      client << F("False");
    }
    client << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>Last mail status:</td><td>")) << mailResponse ;
  if ( mailResponseTime != 0 ) {
    client << (F("<br>@ ")) << DateTime(mailResponseTime);
  }
  else {
    client << (F("Never"));
  }
  client << " " << mailerrortype << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Last mail error:</td><td>")) << mailError << (F(" @ "));
    if ( mailErrorTime != 0 )
      client << DateTime(mailErrorTime);
    else
      client << (F("Never"));
    client << (F("</td></tr>\r\n"));
    delay(5);
  }
  client << (F("<tr><td>Last NTP update:</td><td>")) << DateTime(lastTimeUpdate) << " (in " << ntpRetry << (F("x)</td></tr>\r\n"));
  client << (F("<tr><td>NTP status:</td><td>"));
  switch (timeStatus()) {
    case timeNotSet: client << F("time not set");
      break;
    case timeNeedsSync: client << F("time needs synchronization");
      break;
    case timeSet: client << F("time is synchronized");
      break;
    default: client << F("time status unknown");
  }
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Today WAN IP checks:</td><td>")) << todayIPCnt << (F("</td></tr>\r\n"));
  client << (F("<tr><td>DNS status:</td><td>"));
  if (DnsStatus == 1)
  {
    sprintf(msgchararray, "Ok");
  }
  else {
    sprintf(msgchararray, "Error&rArr;%05d", DnsStatus);
  }
  client << msgchararray << (F("</td></tr>\r\n"));
  if (extended) {
    delay(25);
    client << (F("<tr><td>Last DynDNS status:</td><td>"));
    // cut the String dynDNSresponse in parts of 28 chars with the last line with a max 20
    int i = 0;
    while ((dynDNSresponse[i] != '\0') && (i < 64))
    {
      client << dynDNSresponse[i];  // dynDNSresponse is max 64 chars
      if ( i > 0 && i % 28 == 0) {
        client << (F("<br>"));
      }
      i++;
    }
    client << (F(" @ "));
    if ((dynDNSResponseTime == 0) || isnan(dynDNSResponseTime)) {
      client << (F("Never"));
    }
    else {
      client << DateTime(dynDNSResponseTime);
    }
    client << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>dynDNS updates:</td><td>")) << dynDNSCnt << (F("</td></tr>\r\n"));
  client << (F("<tr><td>HTTP displays:</td><td>")) << httpShown << (F("</td></tr>\r\n"));
  client << (F("<tr><td>HTTP connections with<br>binary protocol:</td><td>")) << errorHTMLnocharavlbl << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Unknown HTTP commands<br>received from whitelisted IP's:</td><td>")) << unkcmdfrmwhl << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Blocked known HTTP<br>commands received:</td><td>")) << unknownCmd_total << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Total HTTP commands<br>in blacklist:</td><td>")) << totalHTTPcmd << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>HTTP assaults received:</td><td>")) << httphacks << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Last HTTP assault received:</td><td>"));
    if ( httphacksTime != 0 )
      client << DateTime(httphacksTime);
    else
      client << (F("Never"));
    client << (F("</td></tr>\r\n"));
    client << (F("<tr><td>HTTP Rx buffer overflow:</td><td>")) << httpbuffoverflow << (F("</td></tr>\r\n"));
    client << (F("<tr><td>Time last HTTP buffer<br>overflow received:</td><td>"));
    if ( httpbuffoverflowTime != 0 )
      client << DateTime(httpbuffoverflowTime);
    else
      client << (F("Never"));
    client << (F("</td></tr>\r\n"));
  }
#if defined(ARCH_STM32)|| defined(ARDUINO_ARCH_ESP32) || defined(STM32F4xx)
  client << (F("<tr><td>Suspicious connection<br>requestsreceived:</td><td>")) << suspicious_total << (F("</td></tr>\r\n"));
  client << (F("<tr><td>IP's reported success:</td><td>")) << rprt2abuseipdb << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>IP's reported failed:</td><td>")) << rprt2abuseipdberr << (F("</td></tr>\r\n"));
    client << (F("<tr><td>AbuseIPDB errors:</td><td>")) <<  todayErrCnt_ab << (F("</td></tr>\r\n"));
    client << (F("<tr><td>AbuseIPDB errors since restart:</td><td>")) <<  totalErrCnt_ab << (F("</td></tr>\r\n"));
    client << (F("<tr><td>AbuseIPDB last response:</td><td>")) <<  ab_lastresponse << (F("</td></tr>\r\n"));
    client << (F("<tr><td>AbuseIPDB array scan time:</td><td>")) <<  abuse_array_scan_run_time << (F("ms</td></tr>\r\n"));
  }
  client << (F("<tr><td>AbuseIPDB array scans:</td><td>")) <<  abuse_well_known_scans << (F("</td></tr>\r\n"));
  client << (F("<tr><td>AbuseIPDB array scan hits:</td><td>")) <<  abuse_well_known_hits << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Total IP nets in whitelist:</td><td>")) <<  totalnets << (F("</td></tr>\r\n"));
  }
#endif
  client << (F("<tr><td>FIN_WAIT close requests:</td><td>")) << socket0x18close << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>FIN_WAIT Last socket:</td><td>")) << lastsocket18close << (F("</td></tr>\r\n"));
    client << (F("<tr><td>FIN_WAIT last time:</td><td>")) << lastsocket18closetime << (F("</td></tr>\r\n"));
  }
  client << (F("<tr><td>No socket available:</td><td>")) << nosockavail << (F("</td></tr>\r\n"));
  if (extended) {
    for (uint8_t sock = 0; sock < MAX_SOCK_NUM; sock++) {
      client << (F("<tr><td>Last disconnect socket ")) << sock << (F(":</td><td>")) << lastSock_DisCon[sock] << (F("</td></tr>\r\n"));
    }
  }
  client << (F("<tr><td>Flash errors:</td><td>")) << nr_flsh_error << (F("</td></tr>\r\n"));
  if (extended) {
    client << (F("<tr><td>Last flash error:</td><td>"));
    if (flasherror[0]  == 0 ) {
      client << (F("None"));
    }
    else {
      client << flasherror;
    }
    client  << (F("</td></tr>\r\n"));
    client << (F("<tr><td>SID:</td><td>")) << SID  <<  (F("</td></tr>\r\n"));
    /*
      client << (F("<tr><td>EEPROM writes limited:<td>"));
      if (EEPROM.getAllowedWrites() == 0)
      {
        client << (F("n&#47;a")); // n/a
      }
      else
      {
        client << EEPROM.getAllowedWrites();
      }
      client << (F("</tr>\r\n"));
      client << (F("<tr><td>EEPROM type writes:<td>")) << EEPROM.bytesWritten()  << (F("</tr>\r\n"));
    */
  }
  client << (F("<tr><td>Free RAM:</td><td>"));
  int fPreturn = freeRam();
  (fPreturn == -255) ?  client << (F("currently disabled")) : client << fPreturn;
  client << (F("</td></tr>\r\n"));
  client << (F("<tr><td>Number of free sockets:</td><td>")) << SerialXltStat() << F("</td></tr>\r\n");
  //===

  client << (F("</table>"));
  client << (F("</td>\r\n"));
  //  delay(5);
  // met de h parameter van onderstaande applets zijn de tabellen goed uit te lijnen
  client << (F("<td valign = \"Top\" align = \"center\" colspan=\"6\"><table border=\"1\" cellspacing=\"0\"><caption>Upload graphics</caption>\r\n"));
  client << (F("<tr><td style=\"padding-left:50px;\" colspan=\"4\"><script type='text/javascript' src=\"http://pvoutput.org/widget/inc.jsp\"></script>\r\n"));
  client << (F("<script type='text/javascript' src=\"http://pvoutput.org/widget/graph.jsp?sid=")) << SID << (F("&amp;n=1&amp;w=400&amp;h=102&amp;t=1&amp;c=1&amp;d=1&amp;e=1\"></script></td></tr>\r\n"));
  client << (F("<tr><td style=\"padding-left:175px;\" colspan=\"4\"><script type='text/javascript' src=\"http://pvoutput.org/widget/inc.jsp\"></script>\r\n"));
  client << (F("<script type='text/javascript' src=\"http://pvoutput.org/widget/outputs.jsp?sid=")) << SID << (F("&amp;w=400&amp;h=102&amp;c=2&amp;n=1\"></script></td></tr>\r\n"));
  client << (F("<tr><td align = \"center\" colspan=\"4\"><img alt=\"Graphic of sun radiation measured at Bilthoven today\" class=\"original\" src=\"http://www.rivm.nl/images/uvgif/UVI_NL.png\" title=\"Grafiek met de zonkracht zoals die vandaag in Bilthoven wordt gemeten\"></td></tr>\r\n"));
  //  delay(5);
  client << (F("<tr><td align = \"center\" colspan=\"4\"><a href=\"/\"><img src=\"http://windwatch.net/weer/items/knmiradar.gif\" alt=\"KNMI regenradar\" width=\"450\"></a></td></tr>\r\n"));

  //==
  client << (F("<tr><td align=\"center\" colspan=\"5\"><table border=\"1\" cellspacing=\"0\"> <caption>Ethernet interface status</caption>\r\n"));
  //  delay(5);
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    //    client << (F("<tr><td style=\"width:15%\">Socket#")) << i << (F(": "));
    client << (F("<tr><td style=\"width:40%\">Socket#")) << i << (F(": "));
    XltStat(client.statusfromsocket(i), client);
    client << F("</td><td>Last IP: ") << Ethernet.lastremoteIP(i) << (F("</td><td>DCN: ")) << Sock_DisCon[i] << (F("</td></tr>\r\n"));
  }
  client << (F("</table>\r\n"));
  client << (F("</table>\r\n"));
  //==
  client << "</td></tr>";

  if (extended) {
    client << (F("<tr><td valign=\"Top\" align=\"center\" colspan=\"11\"><table border=\"1\" cellspacing=\"0\"> <caption><b>Remote command issued and count or IP from remote sender</b></caption>\r\n"));
    client << (F("<tr><th style=\"text-align: left\">Command:</th><th style=\"width:10%\">Count:</th>"));
    client << (F("<th style=\"text-align: left\">IP from last remote sender:</th><th>Time from last request:</th></tr>\r\n"));

    client << (F("<tr><td style=\"text-align: left\">display_memoryinfo shown:</td><td style=\"width:10%\">")) << display_memoryinfoshown;
    client << (F("</td><td>")) << ipdisplay_memoryinfoshown[0] << '.' << ipdisplay_memoryinfoshown[1] << '.' << ipdisplay_memoryinfoshown[2] << '.' << ipdisplay_memoryinfoshown[3] << "</td><td>" << DateTime(timedisplay_memoryinfoshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>dumpyield shown:</td><td>")) << dumpyieldshown;
    client << (F("</td><td>")) << ipdumpyieldshown[0] << '.' << ipdumpyieldshown[1] << '.' << ipdumpyieldshown[2] << '.' << ipdumpyieldshown[3] << "</td><td>" << DateTime(timedumpyieldshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>erasecounters shown:</td><td>")) << erasecountersshown;
    client << (F("</td><td>")) << iperasecountersshown[0] << '.' << iperasecountersshown[1] << '.' << iperasecountersshown[2] << '.' << iperasecountersshown[3] << "</td><td>" << DateTime(timeerasecountersshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>ethernetlogging shown:</td><td>")) << ethernetloggingshown;
    client << (F("</td><td>")) << ipethernetloggingshown[0] << '.' << ipethernetloggingshown[1] << '.' << ipethernetloggingshown[2] << '.' << ipethernetloggingshown[3] << "</td><td>" << DateTime(timeethernetloggingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>fandefault shown:</td><td>")) << erasecountersshown;
    client << (F("</td><td>")) << ipfandefaultshown[0] << '.' << ipfandefaultshown[1] << '.' << ipfandefaultshown[2] << '.' << ipfandefaultshown[3] << "</td><td>" << DateTime(timefandefaultshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>flash_serial_dump_table shown:</td><td>")) << flash_serial_dump_tableshown;
    client << (F("</td><td>")) << ipflash_serial_dump_tableshown[0] << '.' << ipflash_serial_dump_tableshown[1] << '.' << ipflash_serial_dump_tableshown[2] << '.' << ipflash_serial_dump_tableshown[3] << "</td><td>" << DateTime(timeflash_serial_dump_tableshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>forceemail shown:</td><td>")) << forceemailshown;
    client << (F("</td><td>")) << ipforceemailshown[0] << '.' << ipforceemailshown[1] << '.' << ipforceemailshown[2] << '.' << ipforceemailshown[3] << "</td><td>" << DateTime(timeforceemailshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>goupdatedns shown:<td>")) << goupdatednsshown;
    client << (F("</td><td>")) << ipgoupdatednsshown[0] << '.' << ipgoupdatednsshown[1] << '.' << ipgoupdatednsshown[2] << '.' << ipgoupdatednsshown[3] << "</td><td>" << DateTime(timegoupdatednsshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>help shown:<td>")) << updatetimewebshown;
    client << (F("</td><td>")) << iphelpshown[0] << '.' << iphelpshown[1] << '.' << iphelpshown[2] << '.' << iphelpshown[3] << "</td><td>" << DateTime(timehelpshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>readvalue shown:<td>")) << readvalueshown;
    client << (F("</td><td>")) << ipreadvalueshown[0] << '.' << ipreadvalueshown[1] << '.' << ipreadvalueshown[2] << '.' << ipreadvalueshown[3] << "</td><td>" << DateTime(timereadvalueshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>savevalue shown:<td>")) << savevalueshown;
    client << (F("</td><td>")) << ipsavevalueshown[0] << '.' << ipsavevalueshown[1] << '.' << ipsavevalueshown[2] << '.' << ipsavevalueshown[3] << "</td><td>" << DateTime(timesavevalueshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>saveyield shown:<td>")) << saveyieldshown;
    client << (F("</td><td>")) << ipsaveyieldshown[0] << '.' << ipsaveyieldshown[1] << '.' << ipsaveyieldshown[2] << '.' << ipsaveyieldshown[3] << "</td><td>" << DateTime(timesaveyieldshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>send404 shown:<td>")) << send404shown;
    client << (F("</td><td>")) << ipsend404shown[0] << '.' << ipsend404shown[1] << '.' << ipsend404shown[2] << '.' << ipsend404shown[3] << "</td><td>" << DateTime(timesend404shown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>sendRobotstxt shown:<td>")) << sendRobotstxtshown;
    client << (F("</td><td>")) << ipsendRobotstxtshown[0] << '.' << ipsendRobotstxtshown[1] << '.' << ipsendRobotstxtshown[2] << '.' << ipsendRobotstxtshown[3] << "</td><td>" << DateTime(timesendRobotstxtshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>setabuslogging shown:<td>")) << setabusloggingshown;
    client << (F("</td><td>")) << ipsetabusloggingshown[0] << '.' << ipsetabusloggingshown[1] << '.' << ipsetabusloggingshown[2] << '.' << ipsetabusloggingshown[3] << "</td><td>" << DateTime(timesetabusloggingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>setbrcorrfact shown:<td>")) << setbrcorrfactshown;
    client << (F("</td><td>")) << ipsetbrcorrfactshown[0] << '.' << ipsetbrcorrfactshown[1] << '.' << ipsetbrcorrfactshown[2] << '.' << ipsetbrcorrfactshown[3] << "</td><td>" << DateTime(timesetbrcorrfactshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>setbrlogging shown:<td>")) << setbrloggingshown;
    client << (F("</td><td>")) << ipsetbrloggingshown[0] << '.' << ipsetbrloggingshown[1] << '.' << ipsetbrloggingshown[2] << '.' << ipsetbrloggingshown[3] << "</td><td>" << DateTime(timesetbrloggingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>sethttplogging shown:<td>")) << sethttploggingshown;
    client << (F("</td><td>")) << ipsethttploggingshown[0] << '.' << ipsethttploggingshown[1] << '.' << ipsethttploggingshown[2] << '.' << ipsethttploggingshown[3] << "</td><td>" << DateTime(timesethttploggingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>setinverterlog shown:<td>")) << setinverterlogshown;
    client << (F("</td><td>")) << ipsetinverterlogshown[0] << '.' << ipsetinverterlogshown[1] << '.' << ipsetinverterlogshown[2] << '.' << ipsetinverterlogshown[3] << "</td><td>" << DateTime(timesetinverterlogshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>setpluvioena shown:<td>")) << setpluvioenashown;
    client << (F("</td><td>")) << ipsetpluvioenashown[0] << '.' << ipsetpluvioenashown[1] << '.' << ipsetpluvioenashown[2] << '.' << ipsetpluvioenashown[3] << "</td><td>" << DateTime(timesetpluvioenashown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>setudpoff shown:<td>")) << setudpoffshown;
    client << (F("</td><td>")) << ipsetudpoffshown[0] << '.' << ipsetudpoffshown[1] << '.' << ipsetudpoffshown[2] << '.' << ipsetudpoffshown[3] << "</td><td>" << DateTime(timesetudpoffshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>setudpon shown:<td>")) << setudponshown;
    client << (F("</td><td>")) << ipsetudponshown[0] << '.' << ipsetudponshown[1] << '.' << ipsetudponshown[2] << '.' << ipsetudponshown[3] << "</td><td>" << DateTime(timesetudponshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>show_logging shown:<td>")) << show_loggingshown;
    client << (F("</td><td>")) << ipshow_loggingshown[0] << '.' << ipshow_loggingshown[1] << '.' << ipshow_loggingshown[2] << '.' << ipshow_loggingshown[3] << "</td><td>" << DateTime(timeshow_loggingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showbufferremaining shown:<td>")) << showbufferremainingshown;
    client << (F("</td><td>")) << ipshowbufferremainingshown[0] << '.' << ipshowbufferremainingshown[1] << '.' << ipshowbufferremainingshown[2] << '.' << ipshowbufferremainingshown[3] << "</td><td>" << DateTime(timeshowbufferremainingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showfan shown:<td>")) << showfanshown;
    client << (F("</td><td>")) << ipshowfanshown[0] << '.' << ipshowfanshown[1] << '.' << ipshowfanshown[2] << '.' << ipshowfanshown[3] << "</td><td>" << DateTime(timeshowfanshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showip shown:<td>")) << showipshown;
    client << (F("</td><td>")) << ipshowipshown[0] << '.' << ipshowipshown[1] << '.' << ipshowipshown[2] << '.' << ipshowipshown[3] << "</td><td>" << DateTime(timeshowipshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showiplog shown:<td>")) << showiplogshown;
    client << (F("</td><td>")) << ipshowiplogshown[0] << '.' << ipshowiplogshown[1] << '.' << ipshowiplogshown[2] << '.' << ipshowiplogshown[3] << "</td><td>" << DateTime(timeshowiplogshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showmaillogging shown:<td>")) << showmailloggingshown;
    client << (F("</td><td>")) << ipshowmailloggingshown[0] << '.' << ipshowmailloggingshown[1] << '.' << ipshowmailloggingshown[2] << '.' << ipshowmailloggingshown[3] << "</td><td>" << DateTime(timeshowmailloggingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showpluviourl shown:<td>")) << showpluviourlshown;
    client << (F("</td><td>")) << ipshowpluviourlshown[0] << '.' << ipshowpluviourlshown[1] << '.' << ipshowpluviourlshown[2] << '.' << ipshowpluviourlshown[3] << "</td><td>" << DateTime(timeshowpluviourlshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showpvidebug shown:<td>")) << showpvidebugshown;
    client << (F("</td><td>")) << ipshowpvidebugshown[0] << '.' << ipshowpvidebugshown[1] << '.' << ipshowpvidebugshown[2] << '.' << ipshowpvidebugshown[3] << "</td><td>" << DateTime(timeshowpvidebugshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showpvostring shown:<td>")) << showpvostringshown;
    client << (F("</td><td>")) << ipshowpvostringshown[0] << '.' << ipshowpvostringshown[1] << '.' << ipshowpvostringshown[2] << '.' << ipshowpvostringshown[3] << "</td><td>" << DateTime(timeshowpvostringshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showpvoutputlog shown:<td>")) << showpvoutputlogshown;
    client << (F("</td><td>")) << ipshowpvoutputlogshown[0] << '.' << ipshowpvoutputlogshown[1] << '.' << ipshowpvoutputlogshown[2] << '.' << ipshowpvoutputlogshown[3] << "</td><td>" << DateTime(timeshowpvoutputlogshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showsockstatus shown:<td>")) << showsockstatusshown;
    client << (F("</td><td>")) << ipshowsockstatusshown[0] << '.' << ipshowsockstatusshown[1] << '.' << ipshowsockstatusshown[2] << '.' << ipshowsockstatusshown[3] << "</td><td>" << DateTime(timeshowsockstatusshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showstatus shown:<td>")) << showstatusshown;
    client << (F("</td><td>")) << ipshowstatusshown[0] << '.' << ipshowstatusshown[1] << '.' << ipshowstatusshown[2] << '.' << ipshowstatusshown[3] << "</td><td>" << DateTime(timeshowstatusshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showextended shown:<td>")) << showstatusextshown;
    client << (F("</td><td>")) << ipshowstatusextshown[0] << '.' << ipshowstatusextshown[1] << '.' << ipshowstatusextshown[2] << '.' << ipshowstatusextshown[3] << "</td><td>" << DateTime(timeshowstatusextshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showsuspecturl shown:<td>")) << showsuspecturlshown;
    client << (F("</td><td>")) << ipshowsuspecturlshown[0] << '.' << ipshowsuspecturlshown[1] << '.' << ipshowsuspecturlshown[2] << '.' << ipshowsuspecturlshown[3] << "</td><td>" << DateTime(timeshowsuspecturlshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showunknowncmd shown:<td>")) << showunknowncmdshown;
    client << (F("</td><td>")) << ipshowunknowncmdshown[0] << '.' << ipshowunknowncmdshown[1] << '.' << ipshowunknowncmdshown[2] << '.' << ipshowunknowncmdshown[3] << "</td><td>" << DateTime(timeshowunknowncmdshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showversion shown:<td>")) << showversionshown;
    client << (F("</td><td>")) << ipshowversionshown[0] << '.' << ipshowversionshown[1] << '.' << ipshowversionshown[2] << '.' << ipshowversionshown[3] << "</td><td>" << DateTime(timeshowversionshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>showweatherlog shown:<td>")) << showweatherlogshown;
    client << (F("</td><td>")) << ipshowweatherlogshown[0] << '.' << ipshowweatherlogshown[1] << '.' << ipshowweatherlogshown[2] << '.' << ipshowweatherlogshown[3] << "</td><td>" << DateTime(timeshowweatherlogshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>starttftp shown:<td>")) << starttftpshown;
    client << (F("</td><td>")) << ipstarttftpshown[0] << '.' << ipstarttftpshown[1] << '.' << ipstarttftpshown[2] << '.' << ipstarttftpshown[3] << "</td><td>" << DateTime(timestarttftpshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>suspemail shown:<td>")) << suspemailshown;
    client << (F("</td><td>")) << ipsuspemailshown[0] << '.' << ipsuspemailshown[1] << '.' << ipsuspemailshown[2] << '.' << ipsuspemailshown[3] << "</td><td>" << DateTime(timesuspemailshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>switchlogging shown:<td>")) << switchloggingshown;
    client << (F("</td><td>")) << ipswitchloggingshown[0] << '.' << ipswitchloggingshown[1] << '.' << ipswitchloggingshown[2] << '.' << ipswitchloggingshown[3] << "</td><td>" << DateTime(timeswitchloggingshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>testhttpblacklist shown:<td>")) << testhttpblacklistshown;
    client << (F("</td><td>")) << iptesthttpblacklistshown[0] << '.' << iptesthttpblacklistshown[1] << '.' << iptesthttpblacklistshown[2] << '.' << iptesthttpblacklistshown[3] << "</td><td>" << DateTime(timetesthttpblacklistshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>testipwhitelist shown:<td>")) << testipwhitelistshown;
    client << (F("</td><td>")) << iptestipwhitelistshown[0] << '.' << iptestipwhitelistshown[1] << '.' << iptestipwhitelistshown[2] << '.' << iptestipwhitelistshown[3] << "</td><td>" << DateTime(timetestipwhitelistshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>unknowncmdemail shown:<td>")) << unknowncmdemailshown;
    client << (F("</td><td>")) << ipunknowncmdemailshown[0] << '.' << ipunknowncmdemailshown[1] << '.' << ipunknowncmdemailshown[2] << '.' << ipunknowncmdemailshown[3] << "</td><td>" << DateTime(timeunknowncmdemailshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>updatednskey shown:<td>")) << updatednskeyshown;
    client << (F("</td><td>")) << ipupdatednskeyshown[0] << '.' << ipupdatednskeyshown[1] << '.' << ipupdatednskeyshown[2] << '.' << ipupdatednskeyshown[3] << "</td><td>" << DateTime(timeupdatednskeyshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>updatetimeweb shown:<td>")) << updatetimewebshown;
    client << (F("</td><td>")) << ipupdatetimewebshown[0] << '.' << ipupdatetimewebshown[1] << '.' << ipupdatetimewebshown[2] << '.' << ipupdatetimewebshown[3] << "</td><td>" << DateTime(timeupdatetimewebshown) << (F("</td></tr>\r\n"));

    client << (F("<tr><td>wolfsslmailswitch shown:<td>")) << wolfsslmailswitchshown;
    client << (F("</td><td>")) << ipwolfsslmailswitchshown[0] << '.' << ipwolfsslmailswitchshown[1] << '.' << ipwolfsslmailswitchshown[2] << '.' << ipwolfsslmailswitchshown[3] << "</td><td>" << DateTime(timewolfsslmailswitchshown) << (F("</td></tr>\r\n"));
    client << (F("</table>\r\n</td>\r\n"));
  }
  //==
  client << (F("</table>\r\n"));
  //==

  // for debug purposes
  //  showPVIdebug(client);
  //  delay(5);
  client << (F("<div style=\"text-align: center;\">\r\n"));
  client << (F("<p><br>DNS name powered by: <a href=\"http://freedns.afraid.org/\">Free DNS</a></p>\r\n"));
  client << (F("<p><br>Weather references see: <a href=\"http://www.weerstation-houten.nl/\">http://www.weerstation-houten.nl</a></p>\r\n"));
  client << (F("<p><br>Weather powered by: <a href=\"http://weerlive.nl/\">KNMI Weergegevens via Weerlive.nl</a></p>\r\n"));
  client << (F("<p><a href=\"http://validator.w3.org/check?uri=referer\"><img src=\"http://www.w3.org/Icons/valid-html401\" alt=\"Valid HTML 4.01 Strict\" height=\"31\" width=\"88\"></a></p>\r\n"));
  client << (F("<p><br>contact: baai.hv@gmail.com</p>\r\n"));
  client << (F("</div>\r\n"));
  client << (F("</table>\r\n"));
  client << (F("</body>\r\n</html>"));
  if (HTTPlog) {
    sprintf(temptxtbuff, "\r\nethernet TX buffer available at end:  %d", client.availableForWrite());
    textlog(temptxtbuff, false);
  }
}

void showPVIdebug(EthernetClient client, IPAddress segm) {
  /*   client << (F("<table border=\"1\" cellspacing=\"0\"> <caption>CRC errors</caption>"));
     client << (F("<tr><th>GridVoltage<th>GridPower<th>TempBst<th>TempInv<th>Dc1Volt<th>Dc1Current<th>Dc2Volt<th>Dc2Current<th>Riso<th>EnergyDay<th>EnergyTotal<th>GridPowerPeakDay</tr>"));
     client << (F("<tr><td>")) << crcErrGridVoltage << (F("</td><td>")) << crcErrGridPower << (F("<td>")) << crcErrgetTempBst << (F("<td>")) << crcErrgetTempInv << (F("<td>")) << crcErrgetDc1Voltage;
     client << (F("<td>")) << crcErrgetDc1Current << (F("<td>")) << crcErrgetDc2Voltage << (F("<td>")) << crcErrgetDc2Current << (F("<td>")) << crcErrgetRiso << (F("<td>")) << crcErrgetEnergyDay;
     client << (F("<td>")) << crcErrgetEnergyTotal << (F("<td>")) << crcErrgetGridPowerPeakDay << (F("</tr></table><br>"));
  */
  //  delay(25);
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowpvidebugshown[iip] = segm[iip];
    }
    showpvidebugshown++;
    timeshowpvidebugshown = now();
  }
  client << (F("<table border=\"1\" cellspacing=\"0\">\r\n<caption>PVI actual status</caption>\r\n"));
  client << F("<tr><th>actTension<td>") << actTension << (F("</tr>\r\n"));
  client << F("<tr><th>actPower<td>") << GridPower << (F("</tr>\r\n"));
  client << F("<tr><th>actBstTemp<td>") << actBstTemp << (F("</tr>\r\n"));
  client << F("<tr><th>actInvTemp<td>") << actInvTemp << (F("</tr>\r\n"));
  client << F("<tr><th>actDc1Voltage<td>") << actDc1Voltage << (F("</tr>\r\n"));
  client << F("<tr><th>actDc1Current<td>") << actDc1Current << (F("</tr>\r\n"));
  client << F("<tr><th>actDc2Voltage<td>") << actDc2Voltage << (F("</tr>\r\n"));
  client << F("<tr><th>actDc2Current<td>") << actDc2Current << (F("</tr>\r\n"));
  client << F("<tr><th>Power1<td>") << Power1 << (F("</tr>\r\n"));
  client << F("<tr><th>Power2<td>") << Power2 << (F("</tr>\r\n"));
  client << F("<tr><th>invRiso<td>") << invRiso << (F("</tr>\r\n"));
  client << F("<tr><th>totalToday<td>") << totalToday << (F("</tr>\r\n"));
  client << F("<tr><th>cmltvPwr<td>") << cmltvPwr << (F("</tr>\r\n"));
  client << F("<tr><th>pwrPeak<td>") << pwrPeak << (F("</tr>\r\n"));
  client << F("<tr><th>PeakPowerObserved<td>") << PeakPowerObserved << (F("</tr>\r\n"));
  client << F("<tr><th>TimePeakPower<td>") << TimePeakPower << (F("</tr>\r\n"));
  client << (F("</table>\r\n</body>\r\n</html>"));
  delay(10);
}

void showPluvioURL(EthernetClient client, IPAddress segm) {
  client << F(("<p><br>pluvio data URL: <a href=\"https://gpsgadget.buienradar.nl/data/raintext/?lat=52.02&lon=5.17\">https://gpsgadget.buienradar.nl/data/raintext/?lat=52.02&lon=5.17</a></p>\r\n</body>\r\n</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowpluviourlshown[iip] = segm[iip];
    }
    showpluviourlshown++;
    timeshowpluviourlshown = now();
  }
}

void GoUpdateDNS(EthernetClient client, IPAddress segm) {
  client << F(("<p>Start update DynDNS entry, response ... "));
  updateDNS() ? client << F("Succes ") : client << F("Failed ");
  client << dynDNSresponse << (F(" @ ")) << DateTime(dynDNSResponseTime) << (F("</p>\r\n</body>\r\n</html>"));
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipgoupdatednsshown[iip] = segm[iip];
    }
    goupdatednsshown++;
    timegoupdatednsshown = now();
  }
}

void UpdateDNSKey(EthernetClient client, String cmdString, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipupdatednskeyshown[iip] = segm[iip];
    }
    updatednskeyshown++;
    timeupdatednskeyshown = now();
  }
  int i = cmdString.indexOf("?");
  if ( i != -1 ) {  // this may never occur, because the command is already determined
    int sl = cmdString.length();
    textStringLog("Command line: ", false);
    char cobuff[512];
    int cpybuflen;
    if (sl > 511) {
      cpybuflen = 511;
    }
    else {
      cpybuflen = sl;
    }
    cmdString.toCharArray(cobuff, cpybuflen + 1);
    //    cobuff[cpybuflen] = '\0'; // toCharArray sets terminator
    textlog(cobuff, true);
    textStringLog("Command line length: ", false);
    itoa(sl, cobuff, 10);
    textlog(cobuff, true);
    textStringLog("'?' found at position: ", false);
    itoa(i, cobuff, 10);
    textlog(cobuff, true);
    textStringLog("Valid length is(should be equal to command line length: 107", true);
    //GET /?UpdateDNSKey:Z2diMTF5QkZ2cER4a1hrZWtmRlM6MTY5ODYyNzE=Z2diMTF5QkZ2cER4a1hrZWtmRlM6MTY5ODYyNze HTTP/1.1
#ifndef OPTION_FLASH
    client << F(("<p>function currently not available due to lack of EEPROM on STM32</p>"));
#endif
    bool key_valid = true;
    int x;
    if (sl ==  107) {
      i = cmdString.indexOf(":"); //third check for valid command
      if (i == 18) {
        i++; //index on first position of old key
        char strd_char;
        for ( x = 0; x < 39; x++) {
#ifdef OPTION_FLASH
          strd_char = flash.readChar(DYNDNS_API_KEY_addr + x);
#endif
          if (  strd_char != cmdString[i + x]) {
            key_valid = false;
            textStringLog("Offered old API key invalid, value stored: ", false);
            char echar[1];
            for (int j = 0; j < 39; j++) {
#ifdef OPTION_FLASH
              echar[0] = flash.readChar(DYNDNS_API_KEY_addr + j);
#endif
              textlog(echar, false);
            }
            textStringLog("\r\nOffered old API key:                       ", false);
            for (int j = 0; j < 39; j++) {
              echar[0] = cmdString[i + j];
              textlog(echar, false);
            }
            Serial.println();
            break;
          }
        }
      }
      else {
        key_valid = false;
      }
    }
    else {
      key_valid = false;
    }
    i = cmdString.indexOf('='); //third check for valid command
    if ( key_valid && (i != -1) ) {
      textStringLog("Writing new key: ", false);
      i++; // Set pointer to new key
      String tempAPIstr;
      char echar[1];
      for (x = 0; x < 39; x++) {  // copy incl termination
        tempAPIstr[x] = cmdString[x + i];
        echar[0] = cmdString[x + i];
        textlog(echar, false);
        if (x == 38) {
          tempAPIstr[x + 1] = 0;
          delay(3);
        }
      }
#ifdef OPTION_FLASH
      flash.eraseSector(DYNDNS_API_KEY_addr);
      if (!flash.writeStr(DYNDNS_API_KEY_addr, tempAPIstr)) {
        flash_error = "writeStr DYNDNS_API_KEY_addr FAIL";
        StoreFlashError(flash_error, DYNDNS_API_KEY_addr);
      }
      delay(3);
#endif
      Serial.println();
      client << F(("<p>new key saved to EEPROM</p>\r\n"));
    }
    else {
      client << F(("<p>invalid command, current API key printed on serial port.</p>\r\n</body>\r\n</html>"));
#ifdef OPTION_FLASH
      String strd_char;
      textStringLog("\r\nCurrent stored DNS update API key: ", false);
      flash.readStr(DYNDNS_API_KEY_addr, strd_char);

      char cobuff[512];
      int cpybuflen;
      if (strd_char.length() > 511) {
        cpybuflen = 511;

      }
      else {
        cpybuflen = strd_char.length();
      }
      strd_char.toCharArray(cobuff, cpybuflen + 1);
      //      cobuff[cpybuflen] = '\0'; // toCharArray sets terminator
      textlog(cobuff, true);
#else
      textlog(DYNDNS_API_KEY, true);
#endif
    }
  }
  client << (F("</body><br>\r\n</html>"));
}

void ethernetlogging(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipethernetloggingshown[iip] = segm[iip];
    }
    ethernetloggingshown++;
    timeethernetloggingshown = now();
  }
  textStringLog("current Ethernetlogging: ", false);
  ethrlog ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" Ethernetlogging switched to: ", false);
  if (ethrlog == false) {
    ethrlog = true;
  }
  else {
    ethrlog = false;
  }
  itoa((uint8_t)ethrlog, cobuff, 10);
  textlog(cobuff, false);
  ethrlog ? textStringLog("->true", true) : textStringLog("->false", true);
  client.print(F("<p>The new status for Ethernet logging is: "));
  if (ethrlog == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(ethrlog_addr);
  if (!flash.writeByte(ethrlog_addr, ethrlog)) {
    flash_error = "writeByte ethrlog_addr FAIL";
    StoreFlashError(flash_error, ethrlog_addr);
  }
  delay(3);
#endif
}

void showBufferremaining(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowbufferremainingshown[iip] = segm[iip];
    }
    showbufferremainingshown++;
    timeshowbufferremainingshown = now();
  }
  textStringLog("current showbufferremaining: ", false);
  showbufferremaining ? textStringLog("->on", false) : textStringLog("->off", false);
  Serial.print(F(" showbufferremaining switched to: "));
  if (showbufferremaining == false) {
    showbufferremaining = true;
  }
  else {
    showbufferremaining = false;
  }
  showbufferremaining ? textStringLog("->on", true) : textStringLog("->off", true);
  client.print(F("<p>The new status for showbufferremaining logging is: "));
  if (showbufferremaining == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>\r\n"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(showbufferremaining_addr);
  if (!flash.writeByte(showbufferremaining_addr, showbufferremaining)) {
    flash_error = "writeByte showbufferremaining_addr FAIL";
    StoreFlashError(flash_error, showbufferremaining_addr);
  }
  delay(3);
#endif
}

void show_logging(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshow_loggingshown[iip] = segm[iip];
    }
    show_loggingshown++;
    timeshow_loggingshown = now();
  }
  client << (F("Serial logging:              ")) << (min_serial ? F("off") : F("on")) << F("<br>\r\n");
  client << (F("current Ethernetlogging:     ")) << (ethrlog ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("Weather logging:             ")) << (showbuffer ?  F("on") : F("off")) << F("<br>\r\n");
  client << (F("showbufferremaining:         ")) << (showbufferremaining ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("show abuslogging:            ")) << (abuslogging ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("show iplogging:              ")) << (iplog ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("show maillogging:            ")) << (maillogging ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("show pvoutputlog:            ")) << (pvoutputlog ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("dump logging to UDP:         ")) << (udpdump ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("show HTTP requests:          ")) << (HTTPlog ? F("on") : F("off")) << F("<br>\r\n");
  client << (F("</p>\r\n"));
  client << (F("</body>\r\n</html>"));
}

int freeRam()
{
#if defined(STM32F4xx)
  return (STM32freeRAM());
#else
  return 0;
#endif
}

void display_memoryinfo(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipdisplay_memoryinfoshown[iip] = segm[iip];
    }
    display_memoryinfoshown++;
    timedisplay_memoryinfoshown = now();
  }
#if defined (STM32F4xx)
  client.print(F("<p align=\"left\">========Allocated memory blocks========<br>\r\n"));
  display_mallinfo(client);
#endif
  client.print(F("</p>\r\n</body>\r\n</html>"));
}

void forceEmail(EthernetClient client, IPAddress segm) {
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipforceemailshown[iip] = segm[iip];
    }
    forceemailshown++;
    timeforceemailshown = now();
  }
  client << (F("<p align=\"left\">Sending forced daily mail with webcontent\r\n")) <<  br;
  if (sendEmail(9) != 1) {
    client.print(F("Error during mail sending: "));
    client.print(mailResponse);
    client.print(F(" at "));
    client.print(DateTime(mailErrorTime));
  }
  else {
    client << (F("forced daily mail send success")) << br;
  }
  client << (F("Sending forced monthly yield mail")) << br;
  if (sendEmail(3) != 1) {
    client.print(F("Error during mail sending: "));
    client.print(mailResponse);
    client.print(F(" at "));
    client.print(DateTime(mailErrorTime));
  }
  else {
    client << (F("forced monthly yield mail send success")) << br;
  }
  client.print(F("Email done\r\n</p>\r\n</body>\r\n</html>"));
}

void XltStat(uint8_t s, EthernetClient client) {
  switch (s) {
    case 0x00: client << F("CLOSED");
      break;
    case 0x13: client << F("INIT");
      break;
    case 0x14: client << F("LISTEN");
      break;
    case 0x15: client << F("SYNSENT");
      break;
    case 0x16: client << F("SYNRECV");
      break;
    case 0x17: client << F("ESTABLISHED");
      break;
    case 0x18: client << F("FIN_WAIT");
      break;
    case 0x1A: client << F("CLOSING");
      break;
    case 0x1B: client << F("TIME_WAIT");
      break;
    case 0x1C: client << F("CLOSE_WAIT");
      break;
    case 0x1D: client << F("LAST_ACK");
      break;
    case 0x22: client << F("UDP");
      break;
    case 0x32: client << F("IPRAW");
      break;
    case 0x42: client << F("MACRAW");
      break;
    case 0x5F: client << F("PPPOE");
      break;
  }
}

int serprintsockstat(int i, bool rnreq) {
  int nrfreesockets = 0;
  if (Ethernet.socketStatus(i) == 0) {
    nrfreesockets++;
  }
  if (ethrlog == true) {
    sprintf(temptxtbuff, "Socket# %d Status: 0x%02x", i, Ethernet.socketStatus(i)); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    switch (Ethernet.socketStatus(i)) {
      case 0x00: textStringLog(" = CLOSED", false);
        break;
      case 0x13: textStringLog(" = INIT", false);
        break;
      case 0x14: textStringLog(" = LISTEN", false);
        break;
      case 0x15: textStringLog(" = SYNSENT", false);
        break;
      case 0x16: textStringLog(" = SYNRECV", false);
        break;
      case 0x17: textStringLog(" = ESTABLISHED", false);
        break;
      case 0x18: textStringLog(" = FIN_WAIT", false);
        break;
      case 0x1A: textStringLog(" = CLOSING", false);
        break;
      case 0x1B: textStringLog(" = TIME_WAIT", false);
        break;
      case 0x1C: textStringLog(" = CLOSE_WAIT", false);
        break;
      case 0x1D: textStringLog(" = LAST_ACK", false);
        break;
      case 0x22: textStringLog(" = UDP", false);
        break;
      case 0x32: textStringLog(" = IPRAW", false);
        break;
      case 0x42: textStringLog(" = MACRAW", false);
        break;
      case 0x5F: textStringLog(" = PPPOE", false);
        break;
    }
    IPAddress rmIP = Ethernet.lastremoteIP(i);
    sprintf(temptxtbuff, " RemotePort: %d Remote IP:%d.%d.%d.%d", Ethernet.socketlastremotePort(i), rmIP[0], rmIP[1], rmIP[2], rmIP[3]); // temptxtbuff is max 1024
    textlog(temptxtbuff, rnreq);
  }
  return (nrfreesockets);
}

int SerialXltStat() {
  int nrfreesockets = 0;
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    nrfreesockets += serprintsockstat(i, true);
  }
  return (nrfreesockets);
}

void ShowSockStatus(EthernetClient client, IPAddress segm) {
  /*A socket status list:
    0X0 = available.
    0x14 = socket waiting for a connection
    0x17 = socket connected to a server.
    0x22 = UDP socket.
  */
  char cobuff[16];
  if (!isIPself(segm)) {
    for (int iip = 0; iip < 4; iip++) {
      ipshowsockstatusshown[iip] = segm[iip];
    }
    showsockstatusshown++;
    timeshowsockstatusshown = now();
  }
  client << (F("<p>"));
  textStringLog("Handling show Sockets", true);
  client << F("Value for disconnect timer: ") << DISCONNECT_TIMER << F("seconds<br>\r\n");
  textStringLog("Value for disconnect timer: ", false);
  itoa(DISCONNECT_TIMER, cobuff, 10);
  textlog(cobuff, false);
  textStringLog("seconds", true);
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    client << F("Socket#") << i ;
    uint8_t stat = client.statusfromsocket(i);
    //    socketStat[i] = stat;
    sprintf(msgchararray, "%02X ", stat); // msgchararray is max 64 char
    client << F(" Status: 0x") << msgchararray << F("&rArr; ");
    textStringLog(" Status: 0x", false);
    textlog(msgchararray, false);
    XltStat( stat, client);
    client << F(" readSnPort: ") << Ethernet.socketlastremotePort(i) << F(" Remote IP:");
    textStringLog(" readSnPort: ", false);
    //    itoa(Ethernet.socketlastremotePort(i), cobuff, 10);
    //    textlog(cobuff, false);
    //    textStringLog(" Remote IP:", false);
    IPAddress rmIP = Ethernet.lastremoteIP(i);
    //    String scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
    //    strcpy(cobuff, scobuff.c_str());
    sprintf(temptxtbuff, " RemotePort: %d Remote IP:%d.%d.%d.%d", Ethernet.socketlastremotePort(i), rmIP[0], rmIP[1], rmIP[2], rmIP[3]); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    client << '(' << Ethernet.socketlastremotePort(i) << F(") Forced Socket disconnects: ") << Sock_DisCon[i];
    textStringLog("(", false);
    itoa(Ethernet.socketlastremotePort(i), cobuff, 10);
    textlog(cobuff, false);
    textStringLog(") Forced Socket disconnects: ", false);
    itoa(Sock_DisCon[i], cobuff, 10);
    textlog(cobuff, false);
    if (G_SocketConnectionTimers[i] != 0 ) {
      client << F(", timer active, this status duration: ") << millis() - G_SocketConnectionTimers[i] << F("ms");
      textStringLog(", timer active, this status duration: ", false);
      ltoa((millis() - G_SocketConnectionTimers[i]), cobuff, 10);
      textlog(cobuff, false);
      textStringLog("ms", false);
    }
    else {
      client << F(", disconnect timer not active. Value of disconnection timer: ") << G_SocketConnectionTimers[i];
      textStringLog(", disconnect timer not active. Value of disconnection timer: ", false);
      ltoa(G_SocketConnectionTimers[i], cobuff, 10);
      textlog(cobuff, false);
    }
    client << F("<br>\r\n");
    textStringLog("\r\n", false);
  }
  client << (F("</p>\r\n</body>\r\n</html>"));
  textStringLog("Number of free sockets: ", false);
  itoa(SerialXltStat(), cobuff, 10);
  textlog(cobuff, false);
}

void writeP(const uint8_t *data, size_t length, EthernetClient client) {
  // copy data out of program memory into local storage, write out in
  // chunks of 32 bytes to avoid extra short TCP/IP packets
  uint8_t buffer[32];
  size_t bufferEnd = 0;

  if (length == 0 )
    return;
  while (length--)
  {
    if (bufferEnd == 32)
    {
      client.write(buffer, 32);
      bufferEnd = 0;
    }
    buffer[bufferEnd++] = pgm_read_byte(data++);
  }

  if (bufferEnd > 0)
    client.write(buffer, bufferEnd);
}

void favicon(EthernetClient client) {
  P(faviconIco) = WEBDUINO_FAVICON_DATA;
  writeP(faviconIco, sizeof(faviconIco), client);
}

bool selectIcon(EthernetClient client) {
  char optcmd[16];
  char cobuff[16];
  bool stat = true;
  optcmd[0] = '\0';
  int i = 0;
  // icon index selection
  for ( i = 0 ; i < numicon_string_table ; i++ ) {
    strcpy(optcmd, icon_string_table[i]); // Necessary casts and dereferencing, just copy.
    //    Serial.println(optcmd);
    if ( Command(optcmd, icon_temperature)) {
      break;
    }
  }
  if ( !(Command(icon_temperature, optcmd)) && (i == numicon_string_table ) ) {
    stat = false;
  }
  if (HTTPlog) {
    if ( stat ) {
      /*
            textStringLog(" actual icon name: ", false);
            textlog(icon_temperature, false);
            textStringLog(" -> Selected is: ", false);
            textlog(optcmd, false);
            textStringLog(" in index: ", false);
            itoa(i, cobuff, 10);
            textlog(cobuff, false);
            textStringLog(" ", false);
      */
      sprintf(temptxtbuff, "\r\nActual icon name: %s -> Selected is: %s in index: %d ", icon_temperature, optcmd, i); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    else {
      sprintf(temptxtbuff, "\r\nERROR: No iconname match, iconnname requested: %s", icon_temperature);
      textlog(temptxtbuff, false);
    }
  }
  return stat;
}

void showIcon(EthernetClient client) {
  char optcmd[16];
  char cobuff[16];
  bool stat = true;
  optcmd[0] = '\0';
  int i = 0;
  // icon index selection
  for ( i = 0 ; i < numicon_string_table ; i++ ) {
    strcpy(optcmd, icon_string_table[i]); // Necessary casts and dereferencing, just copy.
    //    Serial.println(optcmd);
    if ( Command(optcmd, icon_temperature)) break;
  }
  switch (i) {
    case 0:
      zonnigIcon(client);
      break;
    case 1:
      if (actTension > 0)
        bliksemIcon(client);
      else
        nbliksemIcon(client);
      break;
    case 2:
      regenIcon(client);
      break;
    case 3:
      if (actTension > 0)
        buienIcon(client);
      else
        nbuienIcon(client);
      break;
    case 4:
      hagelIcon(client);
      break;
    case 5:
      if (actTension > 0)
        mistIcon(client);
      else
        nmistIcon(client);
      break;
    case 6:
      sneeuwIcon(client);
      break;
    case 7:
      if (actTension > 0)
        bewolktIcon(client);
      else
        nbewolktIcon(client);
      break;
    case 8:
      if (actTension > 0)
        halfbewolktIcon(client);
      else
        nhalfbewolktIcon(client);
      break;
    case 9:
      zwaarbewolktIcon(client);
      break;
    case 10:
      nmistIcon(client);
      break;
    case 11:
      helderenachtIcon(client);
      break;
    case 12:
      wolkennachtIcon(client);
      break;
    case 13:
      lichtbewolktIcon(client);
      break;
    case 14:
      nachtbewolktIcon(client);
      break;
    default:
      sprintf(temptxtbuff, "\r\nERROR: no icon switch case number match for number %d", i);
      textlog(temptxtbuff, false);
      stat = false;
      break;
  }
  //  return stat;
}

void bewolktIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nbewolktIcon selected.", false);
  }
  P(bewolktIco) = WEBDUINO_BEWOLKT_DATA;
  writeP(bewolktIco, sizeof(bewolktIco) - 1, client);
}

void bliksemIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nbliksemIcon selected.", false);
  }
  P(bliksemIco) = WEBDUINO_BLIKSEM_DATA;
  writeP(bliksemIco, sizeof(bliksemIco) - 1, client);
}

void mistIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nmistIcon selected.", false);
  }
  P(mistIco) = WEBDUINO_MIST_DATA;
  writeP(mistIco, sizeof(mistIco) - 1, client);
}

void buienIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nbuienIcon selected.", false);
  }
  P(buienIco) = WEBDUINO_BUIEN_DATA;
  writeP(buienIco, sizeof(buienIco) - 1, client);
}

void hagelIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nhagelIcon selected.", false);
  }
  P(hagelIco) = WEBDUINO_HAGEL_DATA;
  writeP(hagelIco, sizeof(hagelIco) - 1, client);
}

void halfbewolktIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nhalfbewolktIcon selected.", false);
  }
  P(halfbewolktIco) = WEBDUINO_HALFBEWOLKT_DATA;
  writeP(halfbewolktIco, sizeof(halfbewolktIco) - 1, client);
}

void helderenachtIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nhelderenachtIcon selected.", false);
  }
  P(helderenachtIco) = WEBDUINO_HELDERENACHT_DATA;
  writeP(helderenachtIco, sizeof(helderenachtIco) - 1, client);
}

void regenIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nregenIcon selected.", false);
  }
  P(regenIco) = WEBDUINO_REGEN_DATA;
  writeP(regenIco, sizeof(regenIco) - 1, client);
}

void sneeuwIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nsneeuwIcon selected.", false);
  }
  P(sneeuwIco) = WEBDUINO_SNEEUW_DATA;
  writeP(sneeuwIco, sizeof(sneeuwIco) - 1, client);
}

void zonnigIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nzonnigIcon selected.", false);
  }
  P(zonnigIco) = WEBDUINO_ZONNIG_DATA;
  writeP(zonnigIco, sizeof(zonnigIco) - 1, client);
}

void zwaarbewolktIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nzwaarbewolktIcon selected.", false);
  }
  P(zwaarbewolktIco) = WEBDUINO_ZWAARBEWOLKT_DATA;
  writeP(zwaarbewolktIco, sizeof(zwaarbewolktIco) - 1, client);
}

void wolkennachtIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nwolkennachtIcon selected.", false);
  }
  P(wolkennachtIco) = WEBDUINO_WOLKENNACHT_DATA;
  writeP(wolkennachtIco, sizeof(wolkennachtIco) - 1, client);
}

void nbewolktIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nnbewolktIcon selected.", false);
  }
  P(nbewolktIco) = WEBDUINO_NBEWOLKT_DATA;
  writeP(nbewolktIco, sizeof(nbewolktIco) - 1, client);
}

void nbliksemIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nnbliksemIcon selected.", false);
  }
  P(nbliksemIco) = WEBDUINO_NBLIKSEM_DATA;
  writeP(nbliksemIco, sizeof(nbliksemIco) - 1, client);
}

void nbuienIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nn(acht)buienIcon selected.", false);
  }
  P(nbuienIco) = WEBDUINO_NBUIEN_DATA;
  writeP(nbuienIco, sizeof(nbuienIco) - 1, client);
}

void nhalfbewolktIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nnhalfbewolktIcon selected.", false);
  }
  P(nhalfbewolktIco) = WEBDUINO_NHALFBEWOLKT_DATA;
  writeP(nhalfbewolktIco, sizeof(nhalfbewolktIco) - 1, client);
}

void nmistIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nnmistIcon selected.", false);
  }
  P(nmistIco) = WEBDUINO_NMIST_DATA;
  writeP(nmistIco, sizeof(nmistIco) - 1, client);
}

void lichtbewolktIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nlichtbewolktIcon selected.", false);
  }
  P(lichtbewolktIco) = WEBDUINO_LICHTBEWOLKT_DATA;
  writeP(lichtbewolktIco, sizeof(lichtbewolktIco) - 1, client);
}

void nachtbewolktIcon(EthernetClient client) {
  if (!min_serial && HTTPlog) {
    textStringLog("\r\nnachtbewolktIcon selected.", false);
  }
  P(nachtbewolktIco) = WEBDUINO_NACHTBEWOLKT_DATA;
  writeP(nachtbewolktIco, sizeof(nachtbewolktIco) - 1, client);
}

/*
  Voorbeeld om RAM geheugen tot 0 te reduceren Wordt in de preprocessor opgelost
  Nadeel is wel dat elke string een eigen method vereist......
  // SERIAL PRINT
  // replace Serial.print("string") with SerialPrint("string")
  #define SerialPrint(x) SerialPrint_P(PSTR(x))
  void SerialPrint_P(PGM_P str, void (*f)(uint8_t) = SerialWrite ) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) (*f)(c);
  }

  // ETHERNET CLIENT
  // replace client.print("string") with ClientPrint("string",client)
  #define ClientPrint(x, y) SerialPrint_P(PSTR(x), y)
  void ClientPrint_P(PGM_P str, Client client) {
  for (uint8_t c; (c = pgm_read_byte(str)); str++) client.write(c);

  gebruik:
  SerialPrint("My debugging line goes here, and can be really quite long");
  ClientPrint("<html><head><title>Hello</title></head><body><h1>Hello</h1>...", client);

  =====================================================
  Een andere oplossing is printen via 1 buffer
  // Define as many of these as you like !
  const char exampleString[] PROGMEM = "Here is a big hello string";

  #define MAX_STRING 60
  char stringBuffer[MAX_STRING];
  char* getString(const char* str) {
 	strcpy_P(stringBuffer, (char*)str);
 	return stringBuffer;
  }

  Serial.println(getString(exampleString));
*/
