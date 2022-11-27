#ifdef USE_MAIL
EthernetClient mailClient;
char mailResponse[32];
char mailError[32];

time_t mailResponseTime;
time_t mailErrorTime;
// bool secondSend;
// bool ok;
const static char mailpmstr_0[] PROGMEM = "startup/WAN IP change\0"; // strings for email
const static char mailpmstr_1[] PROGMEM = "Inverter time handling error!\0";
const static char mailpmstr_2[] PROGMEM = "Error: Timeskew to large!\0";
const static char mailpmstr_3[] PROGMEM = "Time: Time change successful\0";
const static char mailpmstr_4[] PROGMEM = "Too large power difference!\0";
const static char mailpmstr_5[] PROGMEM = "DATA\0";
const static char mailpmstr_6[] PROGMEM = "RCPT TO\0";
const static char mailpmstr_7[] PROGMEM = "MAIL FROM\0";
const static char mailpmstr_8[] PROGMEM = "HELO Blackpill\0";
const static char mailpmstr_9[] PROGMEM = "no response on connect\0";
const static char mailpmstr_10[] PROGMEM = "no connection\0";
const static char mailpmstr_11[] PROGMEM = "Intended to be blank\0";

void sendBootContent() {
#ifdef OPTION_NO_PVOUTPUT
  mailClient << F("WARNING: This version is compiled with definition OPTION_NO_PVOUTPUT is true, so no output is uploaded to PVORG.") << F("<br>");
#endif
  sprintf(webData, "%02X-%02X-%02X-%02X-%02X-%02X", macBuffer[0], macBuffer[1], macBuffer[2], macBuffer[3], macBuffer[4], macBuffer[5]);  //webdata is max 96
  mailClient << F("MAC Address: ") << webData << F("<br>");
  mailClient << F("\r\nIP addres: ") << Ethernet.localIP() << F("<br>");
  mailClient << F("\r\nSubnetmask: ") << Ethernet.subnetMask() << F("<br>");
  mailClient << F("\r\nDefault gateway: ") << Ethernet.gatewayIP() << F("<br>");
  mailClient << F("\r\nDNS - server: ") << Ethernet.dnsServerIP() << F("<br>");
  mailClient << F("\r\nWAN IP address: ") << myIP << F("<br>");
  mailClient << F("\r\nDynDNS response: ") << dynDNSresponse << F("<br>") << endl << "\r\n";
  if ( maillogging) {
    textStringLog("DynDNS response voor mail: ", false);
    textlog(dynDNSresponse, true);
  }
}

void sendWebcontent()
{
  ShowStatus(mailClient, true, Ethernet.localIP());
  mailClient << F("<br>");
  mailClient << F("Todays runtime for fan A: ") << runTimeThisDayfanA << F("<br>");
  mailClient << F("Todays runtime for fan B: ") << runTimeThisDayfanB << F("<br>");
  mailClient << F("Todays runtime for fan C: ") << runTimeThisDayfanC << F("<br>");
  mailClient << F("last maillog: ") << maillog << F("<br>\r\n");
#if defined (USE_LOGGING) || defined (USE_PWRLOGGING)
  dumpLogToMail();
#endif
}

void powerMail()
{
  byte i;
  unsigned long myield = 0;
  unsigned long dyield;
  byte monthIndex = (month() - 1) % 3;
  // calculate the total yield for this month
  for (i = 1; i <= day(); i++)
  {
#ifdef OPTION_FLASH
    dyield = flash.readULong(monthBase_addr + (((monthIndex * 31) + i - 1) * EEPROM_PAGE_SIZE));
    if (dyield != 4294967295)
      myield += dyield;
#else
    myield += dayYield[(i - 1) + (monthIndex * 31)];
#endif
  }
  sprintf(mailsubj, "monthly %luW, total output: %lukWh", myield, MaxcmltvPwr);
  sendEnvelope(false);
  for (i = 1; i <= day(); i++)
  {
#ifdef OPTION_FLASH
    dyield = flash.readULong(monthBase_addr + (((monthIndex * 31) + i - 1) * EEPROM_PAGE_SIZE));
#else
    dyield = dayYield[i - 1 + (monthIndex * 31)];
#endif
    if ((dyield != 0) && (dyield != 4294967295))
    {
      // we are misusing the mailsubj array
      sprintf(mailsubj, "%02d-%02d-%04d;%d;<br>\r\n", i, month(), year(), dyield);
      mailClient << mailsubj;
    }
  }
  mailClient <<  F("\r\n");
  //  endMail();
}

void sendEnvelope(bool html)
{
  mailClient << F("Subject: ") << mailsubj << endl; // insert subject
  if (html)
    mailClient << F("MIME-Version: 1.0\r\n");
  mailClient << F("Content-type: text/html; charset=iso-8859-1\r\n");
  mailClient << F("<html><body>");
}

//===================================================================================

byte sendEmail(int mailtype)
{
#ifdef OPTION_NO_EMAIL
  return 0;
#endif
  byte thisByte = 0;
  byte respCode;

  if (mailClient.connect(MAIL_SERVER, MAIL_PORT) == 1) {
    if ( maillogging)
      textStringLog("connected", true);
  } else {
    if ( maillogging)
      textStringLog("connection failed", true);
    return 0;
  }

  if (!eRcv()) return 0;

  if ( maillogging)
    textStringLog("Sending hello", true);
  mailClient.println("EHLO Blackpill");
  if (!eRcv()) return 0;

  if ( maillogging)
    textStringLog("Sending auth login", true);
  mailClient.println("auth login");
  if (!eRcv()) return 0;

  if ( maillogging)
    textStringLog("Sending User", true);
  // Change to your base64 encoded user
  mailClient.println(MAIL_USER);


  if (!eRcv()) return 0;

  if ( maillogging)
    textStringLog("Sending Password", true);
  // change to your base64 encoded password
  mailClient.println(MAIL_AUTH);


  if (!eRcv()) return 0;

  // change to your email address (sender)
  if ( maillogging) {
    textStringLog("Sending From: ", false);
    textStringLog(MAIL_FROM, true);
  }
  mailClient.print("MAIL From: ");
  mailClient.println(MAIL_FROM);
  if (!eRcv()) return 0;

  // change to recipient address
  if ( maillogging) {
    textStringLog("Sending To: ", false);
    textStringLog(MAIL_TO, true);
  }
  mailClient.print("RCPT To: ");
  mailClient.println(MAIL_TO);
  if (!eRcv()) return 0;

  if ( maillogging)
    textStringLog("Sending DATA", true);
  mailClient.println("DATA");
  if (!eRcv()) return 0;
  //tot hier is mailheader/startmail
  if ( maillogging)
    textStringLog("Sending email", false);

  // change to recipient address
  mailClient.print("To: ");
  mailClient.print(MAIL_TTO);
  mailClient.println(MAIL_TO);

  // change to your address
  mailClient.print("From: ");
  mailClient.print(MAIL_FFROM);
  mailClient.println(MAIL_FROM);
  // mailtype content must ended with a \r\n
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  switch (mailtype) {
    case 1:
      textStringLog("sending startup/ipchange mail", false);
      strcpy_P(mailsubj, (char*)mailpmstr_0);
      sendEnvelope(false);
      sendBootContent();
      break;
    case 2:
      textStringLog("Sending daily mail with webcontent", false);
      sprintf(mailsubj, "PVI yield %uWh today ", totalToday);
      sendEnvelope(true);
      sendWebcontent();
      break;
    case 3:
      textStringLog("Send mail containing yield for this month", false);
      powerMail();
      break;
    case 4:
      textStringLog("MAIL: Error: Timeskew to large!", false);
      strcpy_P(mailsubj, (char*)mailpmstr_2);
      sendEnvelope(false);
      mailClient << F("Current timeskew: ") << timeDiff << F(" seconds\r\n");
      break;
    case 5:
      textStringLog("MAIL: Time: Time change successful", false);
      strcpy_P(mailsubj, (char*)mailpmstr_3);
      sendEnvelope(false);
      mailClient << F("Inverter time error: Inverter time and real time have too large timeskew! Current timeskew: ") << timeDiff << F(" seconds\r\n");
      /*    Serial.print(F("Inverter time error: Inverter time and real time have too large timeskew! Current timeskew: "));
          Serial.print(timeD);
          Serial.print(F(" seconds\r\n"));
      */
      mailClient << F("Inverter time: ") << DateTime(currentInvtime) << F(" changed to actual time: ") << DateTime(invDateAndTime)  << F("\r\n");
      /*    Serial.print(F("Inverter time: "));
          Serial.print(DateTime(oldtime));
          Serial.print(F(" changed to actual time: "));
          Serial.print(DateTime(newtime));
          Serial.print(F("\r\n"));
      */
      break;
    case 6:
      textStringLog("MAIL: Too large power difference!", false);
      strcpy_P(mailsubj, (char*)mailpmstr_4);
      sendEnvelope(true);
      mailClient << F("--Start of content--") << F("<br>");
      showPVIdebug(mailClient, Ethernet.localIP());
      mailClient << (F("--End of message--")) << F("\r\n");
      break;
    case 7:
      textStringLog("MAIL: Inverter time handling error!", false);
      strcpy_P(mailsubj, (char*)mailpmstr_1);
      sendEnvelope(false);
      mailClient << F("Failed to set new inverter time") << F("\r\n");
      break;
    case 8:
      textStringLog("MAIL: Inverter time handling error!", false);
      strcpy_P(mailsubj, (char*)mailpmstr_1);
      sendEnvelope(false);
      mailClient << F("Failed to read initial inverter time in setTime") << F("\r\n");
      break;
    case 9:
      textStringLog("MAIL: Sending forced daily mail with webcontent", false);
      sprintf(mailsubj, "PVI forced email up till now %02d-%02d yield = %uWh", day(), month(), totalToday);
      sendEnvelope(true);
      sendWebcontent();
      break;
    case 10:
      textStringLog("MAIL: Too many errors while reading pluvio", false);
      sprintf(mailsubj, "PVI: Too many errors reading pluvio");
      sendEnvelope(false);
      mailClient << F("To many errors resulting in failed pluvio read from buienradar") << F("\r\n");
      mailClient << F("Actual triggered by ") << todayErrCnt << F(" errors") << F("\r\n");
      mailClient << F("\r\n");
      break;
#if defined(ARCH_STM32)|| defined(ARDUINO_ARCH_ESP32) || defined(STM32F4xx)
    case 11:
      textStringLog("MAIL: Suspicious HTTP command received", false);
      sendEnvelope(false);
      mailClient << F("Following command is received, which cold not be correct interpreted:") << F("<br>\r\n");
      //      for (int i = 0; i < suspicious_index; i++) {
      if ((byte)suspicious_string[suspicious_index][0] > 0) {
        mailClient <<  suspicious_index << F(": Time: ") << DateTime(suspicious_time[suspicious_index]) << F(" ") << F("From IP: ");
        for (int j = 0; j < 4; j++) {
          mailClient << suspicious_ips[suspicious_index][j];
          if (j < 3) mailClient << ".";
        }
        mailClient << F(" suspicious: ");
        if (suspicious_perc[suspicious_index] > 0) {
          mailClient << suspicious_perc[suspicious_index] << '%';
        }
        else {
          mailClient << F("unknown");
        }
        mailClient << F(" command length: ") << suspicious_length[suspicious_index] << F(" cmd: ") << suspicious_string[suspicious_index] << F("<br>\r\n");
      }
      //      }
      mailClient << F("\r\n");
      /*
            if (suspicious_index == 25) {
              checkforIPabuse();
              for (int i = 0; i < 25; i++) {
                for (int j = 0; j < SUSPCMDSIZE; j++) {
                  suspicious_string[i][j] = '\0';
                }
                for (int j = 0; j < 4; j++) {
                  suspicious_ips[i][j] = 0;
                }
                suspicious_length[i] = 0;
                suspicious_perc[i] = 0;
                suspicious_time[i] = 0;
              }
              suspicious_index = 0;
              last_suspicious_index = 0;
            }
      */
      break;
#endif
    case 12:
      textStringLog("ERROR: Mail WolfSSL tried to send a datagram to buienradar.nl but socket reject. ", false);
      sprintf(mailsubj, "ERROR: WolfSSL experiented send datagram problem");
      sendEnvelope(false);
      mailClient << F("WolfSSL has tried to send at least a zero byte packet or expected troubles by sending a packet to buienradar.nl") << F("\r\n");
      mailClient << mailcontent << F("\r\nReason: ") << mailbrstate << F("\r\n");
      ShowSockStatus(mailClient, Ethernet.localIP());
      mailClient << F("\r\n");
      break;
    case 13:
      textStringLog("ERROR: Mail WolfSSL tried to receive a datagram but nothing received. ", false);
      sprintf(mailsubj, "ERROR: WolfSSL experiented receive datagram problem");
      sendEnvelope(false);
      mailClient << F("WolfSSL has experienced at least a timeout or a zero byte packet by receiving a packet") << F("\r\n");
      mailClient << mailcontent << F("\r\nReason: ") << mailbrstate << F("\r\n");
      ShowSockStatus(mailClient, Ethernet.localIP());
      mailClient << F("\r\n");
      break;
    case 14:
      textStringLog("ERROR: Mail WolfSSL tried to send a datagram to AbuseIPDB but socket reject. ", false);
      sprintf(mailsubj, "ERROR: WolfSSL experiented send datagram problem to AbuseIPDB");
      sendEnvelope(false);
      mailClient << F("WolfSSL has tried to send at least a zero byte packet or expected troubles by sending a packet") << F("\r\n");
      mailClient << mailcontent << F("\r\nReason: ") << mailbrstate << F("\r\n");
      ShowSockStatus(mailClient, Ethernet.localIP());
      mailClient << F("\r\n");
      break;
    case 15:
      textStringLog("ERROR: Mail WolfSSL tried to receive a datagram from AbuseIPDB but nothing received. ", false);
      sprintf(mailsubj, "ERROR: WolfSSL experiented receive datagram problem from AbuseIPDB");
      sendEnvelope(false);
      mailClient << F("WolfSSL has experienced at least a timeout or a zero byte packet by receiving a packet from AbuseIPDB") << F("\r\n");
      mailClient << mailcontent << F("\r\nReason: ") << mailbrstate << F("\r\n");
      ShowSockStatus(mailClient, Ethernet.localIP());
      mailClient << F("\r\n");
      break;
    case 16:
      textStringLog("MAIL: Unknown HTTP commands received", false);
      sprintf(mailsubj, "baaisolar: Unknown HTTP commands received");
      sendEnvelope(false);
      mailClient << F("Following command is received, which cold not be correct interpreted:") << F("\r\n");
      /*
           for (int i = 0; i < UNKWNCMDARRAYDEPTH; i++) {
              if ((byte)unknownCmd_string[i][0] > 0) {
                mailClient << F("From IP: ");
                for (int j = 0; j < 4; j++) {
                  mailClient << unknownCmd_ips[i][j];
                  if (j < 3) mailClient << ".";
                }
                mailClient << F(" command length: ") << unknownCmd_length[i] << F(" cmd: ") << unknownCmd_string[i] << F("\r\n");
              }
            }
      */
      mailClient << F("From IP: ");
      for (int j = 0; j < 4; j++) {
        mailClient << unknownCmd_ips[unknownCmd_index][j];
        if (j < 3) mailClient << ".";
      }
      mailClient << F(" command length: ") << unknownCmd_length[unknownCmd_index] << F(" cmd: ") << unknownCmd_string[unknownCmd_index] << F("\r\n");
      mailClient << F("\r\n");
      break;
    default:
      textStringLog("\n\rERROR unknown mailtype requested", false);
      sprintf(mailsubj, "mail from solarsystem PVI logger");
      sendEnvelope(false);
      mailClient << F("ERROR unknown mailtype: ") << mailtype << F("requested\r\n");;
  }
  mailClient.println(".");
  if (!eRcv()) return 0;
  if ( maillogging)
    textStringLog("Sending QUIT", true);
  mailClient.println("QUIT");
  if (!eRcv()) return 0;
  mailClient.stop();
  if ( maillogging)
    textStringLog("disconnected", false);
  return 1;
}

byte eRcv() {
  byte respCode;
  byte thisByte;
  int loopCount = 0;
  int mrpos = 0;
  while (!mailClient.available()) {
    delay(1);
    loopCount++;
    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      mailClient.stop();
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: eRcv: Timeout when waiting for SMTP response", hour(), minute(), second());
      textlog(temptxtbuff, false);
      sprintf(mailResponse, "Timeout\0");
      return 0;
    }
  }
  respCode = mailClient.peek();
  while (mailClient.available())
  {
    thisByte = mailClient.read();
    if ( maillogging)
      Serial.write(thisByte);
    if (mrpos < 30 ) {
      mailResponse[mrpos] = thisByte;
      mrpos++;
      mailResponse[mrpos] = '\0';
    }
  }
  mailResponseTime = now();
  if (respCode >= '4')
  {
    efail();
    return 0;
  }
  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;
  mailClient.println(F("QUIT"));
  while (!mailClient.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      mailClient.stop();
      textStringLog("\r\nTimeout", true);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: efail: Timeout when waiting for SMTP response", hour(), minute(), second());
      textlog(temptxtbuff, false);
      mailErrorTime = now();
      return;
    }
  }
  while (mailClient.available())
  {
    thisByte = mailClient.read();
    Serial.write(thisByte);
  }
  sprintf(mailError, " disconnection forced");
  mailErrorTime = now();
  textStringLog("disconnected after error", true);
}

//========================================================================


#if defined (USE_LOGGING) || defined (USE_PWRLOGGING)
void dumpLogToMail()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(4)) {
    textStringLog("Card failed, or not present", true);
    // don't do anything more:
    return;
  }
  textStringLog("card initialized.", true);
  char fileName[20];

  // create file name
  sprintf(fileName, "%02d-%02d.csv", year(), month());
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fileName);

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      mailClient.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    textStringLog("error opening datalog.txt", true);
  }
}
#endif
#endif
