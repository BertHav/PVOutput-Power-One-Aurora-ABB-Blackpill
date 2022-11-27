uint8_t ipsetudponshown[4];
int setudponshown;
time_t timesetudponshown = 0;
uint8_t ipsetudpoffshown[4];
int setudpoffshown;
time_t timesetudpoffshown = 0;
uint8_t ipsethttploggingshown[4];
int sethttploggingshown = 0;
time_t timesethttploggingshown = 0;
uint8_t ipunknowncmdemailshown[4];
int unknowncmdemailshown = 0;
time_t timeunknowncmdemailshown = 0;
uint8_t ipsuspemailshown[4];
int suspemailshown = 0;
time_t timesuspemailshown = 0;
uint8_t ipsetabusloggingshown[4];
int setabusloggingshown = 0;
time_t timesetabusloggingshown = 0;
uint8_t ipsetbrloggingshown[4];
int setbrloggingshown = 0;
time_t timesetbrloggingshown = 0;
uint8_t ipsetinverterlogshown[4];
int setinverterlogshown = 0;
time_t timesetinverterlogshown = 0;
uint8_t ipsetpluvioenashown[4];
int setpluvioenashown = 0;
time_t timesetpluvioenashown = 0;



void setHTTPlogging(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  for (int iip = 0; iip < 4; iip++) {
    ipsethttploggingshown[iip] = segm[iip];
  }
  sethttploggingshown++;
  timesethttploggingshown = now();
  textStringLog("current HTTP logging: ", false);
  HTTPlog ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" HTTP logging switched to: ", false);
  if (HTTPlog == false) {
    HTTPlog = true;
  }
  else {
    HTTPlog = false;
  }
  HTTPlog ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for HTTP logging is: "));
  if (HTTPlog == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(HTTPlog_addr);
  if (!flash.writeByte(HTTPlog_addr, HTTPlog)) {
    flash_error = "writeByte HTTPlog_addr FAIL";
    StoreFlashError(flash_error, HTTPlog_addr);
  }
  delay(3);
#endif
}

void setwolfSSLMail(EthernetClient client) {
  char cobuff[16];
  textStringLog("Send email for first WolfSSL read/write error: ", false);
  WolfSSLrwmail ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" WolfSSL read/write error mail set to: ", false);
  if (WolfSSLrwmail == false) {
    WolfSSLrwmail = true;
  }
  else {
    WolfSSLrwmail = false;
  }
  WolfSSLrwmail ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for sending email in case WolfSSL read/write error is: "));
  if (WolfSSLrwmail == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(wolfSSLMail_addr);
  if (!flash.writeByte(wolfSSLMail_addr, WolfSSLrwmail)) {
    flash_error = "writeByte wolfSSLMail_addr FAIL";
    StoreFlashError(flash_error, wolfSSLMail_addr);
  }
  delay(3);
#endif
}

void setPluvioEna(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  for (int iip = 0; iip < 4; iip++) {
    ipsetpluvioenashown[iip] = segm[iip];
  }
  setpluvioenashown++;
  timesetpluvioenashown = now();
  textStringLog("current pluvio reading from Buienradar.nl: ", false);
  pluvio_ena ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" pluvio reading switched to: ", false);
  if (pluvio_ena == false) {
    pluvio_ena = true;
  }
  else {
    pluvio_ena = false;
  }
  pluvio_ena ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for reading pluvio from buienradar.nl is: "));
  if (pluvio_ena == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(pluvio_ena_addr);
  if (!flash.writeByte(pluvio_ena_addr, pluvio_ena)) {
    flash_error = "writeByte pluvio_ena_addr FAIL";
    StoreFlashError(flash_error, pluvio_ena_addr);
  }
  delay(3);
#endif
}

void setInverterlog(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  for (int iip = 0; iip < 4; iip++) {
    ipsetinverterlogshown[iip] = segm[iip];
  }
  setinverterlogshown++;
  timesetinverterlogshown = now();
  textStringLog("current inverter logging: ", false);
  invlogging ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" inverter logging switched to: ", false);
  if (invlogging == false) {
    invlogging = true;
  }
  else {
    invlogging = false;
  }
  invlogging ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for inverter logging is: "));
  if (invlogging == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(invlogging_addr);
  if (!flash.writeByte(invlogging_addr, invlogging)) {
    flash_error = "writeByte invlogging_addr FAIL";
    StoreFlashError(flash_error, invlogging_addr);
  }
  delay(3);
#endif
}

void setbrlogging(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  for (int iip = 0; iip < 4; iip++) {
    ipsetbrloggingshown[iip] = segm[iip];
  }
  setbrloggingshown++;
  timesetbrloggingshown = now();
  textStringLog("buienradar.nl logging: ", false);
  brlogging ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" buienradar.nl logging switched to: ", false);
  if (brlogging == false) {
    brlogging = true;
  }
  else {
    brlogging = false;
  }
  brlogging ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for buienradar.nl logging is: "));
  if (brlogging == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(brlogging_addr);
  if (!flash.writeByte(brlogging_addr, brlogging)) {
    flash_error = "writeByte brlogging_addr FAIL";
    StoreFlashError(flash_error, brlogging_addr);
  }
  delay(3);
#endif
}

void setabuslogging(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  for (int iip = 0; iip < 4; iip++) {
    ipsetabusloggingshown[iip] = segm[iip];
  }
  setabusloggingshown++;
  timesetabusloggingshown = now();
  textStringLog("AbuseIPDB logging: ", false);
  abuslogging ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" AbuseIPDB logging switched to: ", false);
  if (abuslogging == false) {
    abuslogging = true;
  }
  else {
    abuslogging = false;
  }
  abuslogging ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for AbuseIPDB logging is: "));
  if (abuslogging == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(abuslogging_addr);
  if (!flash.writeByte(abuslogging_addr, abuslogging)) {
    flash_error = "writeByte abuslogging_addr FAIL";
    StoreFlashError(flash_error, abuslogging_addr);
  }
  delay(3);
#endif
}

void SuspEmail(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  for (int iip = 0; iip < 4; iip++) {
    ipsuspemailshown[iip] = segm[iip];
  }
  suspemailshown++;
  timesuspemailshown = now();
  textStringLog("Suspicious email sending: ", false);
  suspicious_mail ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" Suspicious email sending switched to: ", false);
  if (suspicious_mail == false) {
    suspicious_mail = true;
  }
  else {
    suspicious_mail = false;
  }
  suspicious_mail ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for Suspicious email sending is: "));
  if (suspicious_mail == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(suspicious_mail_addr);
  if (!flash.writeByte(suspicious_mail_addr, suspicious_mail)) {
    flash_error = "writeByte suspicious_mail_addr FAIL";
    StoreFlashError(flash_error, suspicious_mail_addr);
  }
  delay(3);
#endif
}

void UnknownCmdEmail(EthernetClient client, IPAddress segm) {
  char cobuff[16];
  for (int iip = 0; iip < 4; iip++) {
    ipunknowncmdemailshown[iip] = segm[iip];
  }
  unknowncmdemailshown++;
  timeunknowncmdemailshown = now();
  textStringLog("Unknown command email sending: ", false);
  unknownCmd_mail ? textStringLog("on", false) : textStringLog("off", false);
  textStringLog(" Unknown command email sending switched to: ", false);
  if (unknownCmd_mail == false) {
    unknownCmd_mail = true;
  }
  else {
    unknownCmd_mail = false;
  }
  unknownCmd_mail ? textStringLog("on -> true", true) : textStringLog("off -> false", true);
  client.print(F("<p>The new status for unknown command email sending is: "));
  if (unknownCmd_mail == true)
    client.print(F("on"));
  else
    client.print(F("off"));
  client.println(F(".</p>"));
  client << (F("</body>\r\n</html>"));
#ifdef OPTION_FLASH
  flash.eraseSector(unknownCmd_mail_addr);
  if (!flash.writeByte(unknownCmd_mail_addr, unknownCmd_mail)) {
    flash_error = "writeByte unknownCmd_mail_addr FAIL";
    StoreFlashError(flash_error, unknownCmd_mail_addr);
  }
  delay(3);
#endif
}

void setUDPon(EthernetClient client, IPAddress segm) {
  for (int iip = 0; iip < 4; iip++) {
    ipsetudponshown[iip] = segm[iip];
  }
  setudponshown++;
  timesetudponshown = now();
  UDPclient.begin(localPort);
  UDPSendTo = client.remoteIP();
  if (UDPSendTo[0] == 192) {
    udpdump = true;
#ifdef OPTION_FLASH
    flash.eraseSector(udpdump_addr);
    if (!flash.writeByte(udpdump_addr, udpdump)) {
      char msgerrflsh[] = "writeByte udpdump_addr FAIL";
      textlog(msgerrflsh, true);
      client << msgerrflsh << (F("<br>\r\n"));
      StoreFlashError(msgerrflsh, udpdump_addr);
    }
    delay(3);
#endif
    client << (F("<p>UDP Client started listening on port: ")) << localPort <<  (F("<br>\r\n"));
    client << (F("UDP client for remote server IP address: ")) << UDPSendTo;
    client << (F(" sending to port: ")) << Port <<  (F("<br>\r\n"));

    client << (F("Start the linuxclient with: nc  -k -l -u -p 5555 -v\r\n\r\n\r\n")) <<  (F("<br>\r\n"));
    client << (F("BE AWARE     BE AWARE      LOGGING COULD LEAD TO ERRORS IN EEPROM PROGRAMMING")) <<  (F("<br>\r\n"));
    client << (F("that is the reaseon udpdump is forced disabled at startup at 3-2-2022")) <<  (F("<br>\r\n"));
    sprintf(temptxtbuff, "\r\nUDP logging has started for remote ip: %d.%d.%d.%d\r\n", UDPSendTo[0], UDPSendTo[1], UDPSendTo[2], UDPSendTo[3]);  // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  else {
    client << ("<p>You are not entitled to start logging from a remote network<br>\r\n<br>\r\n");
    client << ("Request denied!");
    sprintf(temptxtbuff, "\r\nERROR UDP logging request denied, due to remote network request from remote ip: %d.%d.%d.%d\r\n", UDPSendTo[0], UDPSendTo[1], UDPSendTo[2], UDPSendTo[3]);  // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  client << F("</p><br>");
  client << (F("<br>\r\n</body>\r\n</html>"));
}

void setUDPoff(EthernetClient client, IPAddress segm) {
  for (int iip = 0; iip < 4; iip++) {
    ipsetudpoffshown[iip] = segm[iip];
  }
  setudpoffshown++;
  timesetudpoffshown = now();
  textStringLog("\r\nUDP is closing down!\r\n", true);
  udpdump = false;
#ifdef OPTION_FLASH
  flash.eraseSector(udpdump_addr);
  if (!flash.writeByte(udpdump_addr, udpdump)) {
    char msgerrflsh[] = "writeByte udpdump_addr FAIL";
    textlog(msgerrflsh, true);
    client << msgerrflsh << (F("<br>\r\n"));
    StoreFlashError(msgerrflsh, udpdump_addr);
  }
  delay(3);
#endif
  UDPclient.stop();
  client << (F("<p>UDP client stopped</p>"));
  client << (F("<br>\r\n</body>\r\n</html>"));
}

void textlog(char msgline[], bool nline) {
  if (nline) {
    Serial.println(msgline);
  }
  else {
    Serial.print(msgline);
  }
  //hier controleren flag en socket voor UDP geopend is.
  if (udpdump && hasUDPsocket()) {
    UDPclient.beginPacket(UDPSendTo, Port);
    UDPclient.write(msgline);
    UDPclient.endPacket();
    if (nline) {
      UDPclient.beginPacket(UDPSendTo, Port);
      UDPclient.write("\r\n");
      UDPclient.endPacket();
    }
  }
}

void textStringLog(String strmsgline, bool nline) {
  char msgline[2048];
  char char2screen[64];
  int strlngth = strmsgline.length();
  if (strlngth > 2047 ) {
    strlngth = 2047;
  }
  /*
    Serial.print("string to write: ");
    Serial.println(strmsgline);
    Serial.print("determined string length: ");
    Serial.println(strlngth);
    for (int a=0; a <= strlngth+3; a++) {
    msgline[a]= 0x61;
    }
  */
  strmsgline.toCharArray(msgline, strlngth + 1);
  /*

    /Serial.print("chararray to print: ");
    Serial.println(msgline);
    sprintf(char2screen, "strlngth pos %d is byte: 0x%02x\r\n", strlngth, msgline[strlngth]);
    Serial.print(char2screen);
    //  msgline[strlngth] = '\0';
    sprintf(char2screen, "strlngth pos %d msgline[%d] is byte: 0x%02x\r\n", strlngth, strlngth, msgline[strlngth]);
    Serial.print(char2screen);
    for (int i = 0; i <= strlngth + 3; i++) {
    if (msgline[i] != 0) {
      sprintf(char2screen, "pos %d byte=0x%02x, char is: %c\r\n", i, msgline[i], msgline[i]);
    }
    else {
      sprintf(char2screen, "pos %d byte=0x%02x\r\n", i, msgline[i]);
    }
    Serial.print(char2screen);
    }
  */
  textlog(msgline, nline);
}

typedef void (*wolfSSL_Logging_cb)(const int logLevel, const char *const logMessage);

void PVISSLlog(const int lgLevel, const char *const lgMessage) {
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d wolfSSLlog: %d, %s", hour(), minute(), second(), lgLevel, lgMessage);
  textlog(temptxtbuff, false);
  delay(3);  // give the slow samsung some time to write the logmessage
}

int registerWolfSSLDebug() {
  int rsltwlfdbg = -255;
#ifdef DEBUG_WOLFSSL
  if (wolfssllog) {
    rsltwlfdbg = wolfSSL_SetLoggingCb(PVISSLlog);
    Serial.print("result from registering WolfSSL logging call back (0=success): ");
    Serial.println(rsltwlfdbg);
    /* wolfSSL debugging on */
    if (rsltwlfdbg == 0) {
      wolfSSL_Debugging_ON();
      Serial.println("wolfSSL Debugging is switched on.");
    }
  }
#else
  Serial.println("wolfSSL Debugging is not compiled in this version.");
#endif
  return rsltwlfdbg;
}

// interpreter for serial commands
void SerInterpret(char co) {
  Serial.println();
  if (co == '-') {
#ifdef OPTION_FLASH
    unknownCmd_mail = flash.readByte(unknownCmd_mail_addr);
#endif
    if (unknownCmd_mail == false) {
      unknownCmd_mail = true;
    }
    else {
      unknownCmd_mail = false;
    }
    Serial.print(F("The new status for unknown command mail sending is: "));
    if (unknownCmd_mail == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(unknownCmd_mail_addr);
    if (!flash.writeByte(unknownCmd_mail_addr, unknownCmd_mail)) {
      flash_error = "writeByte unknownCmd_mail_addr FAIL";
      StoreFlashError(flash_error, unknownCmd_mail_addr);
    }
#endif
  }
  else if (co == '.') {
#ifdef OPTION_FLASH
    suspicious_mail = flash.readByte(suspicious_mail_addr);
#endif
    if (suspicious_mail == false) {
      suspicious_mail = true;
    }
    else {
      suspicious_mail = false;
    }
    Serial.print(F("The new status for suspicious mail sending is: "));
    if (suspicious_mail == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(suspicious_mail_addr);
    if (!flash.writeByte(suspicious_mail_addr, suspicious_mail)) {
      flash_error = "writeByte suspicious_mail_addr FAIL";
      StoreFlashError(flash_error, suspicious_mail_addr);
    }
#endif
  }
  else if (co == ',') {
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    else {
      Serial.print(F("A (re)new request for an IP address is send"));
#ifdef USE_TEST_LAN_IP
      if (Ethernet.begin(test_LAN_mac) == 0) {
#else
      if (Ethernet.begin(LAN_mac) == 0) {
#endif
        Serial.println("Failed to configure Ethernet using DHCP");
      }
    }
  }
  else if (co == '/') {
#ifdef OPTION_FLASH
    abuslogging = flash.readByte(abuslogging_addr);
#endif
    if (abuslogging == false) {
      abuslogging = true;
    }
    else {
      abuslogging = false;
    }
    Serial.print(F("The new status for AbuseIPDB logging is: "));
    if (abuslogging == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(abuslogging_addr);
    if (!flash.writeByte(abuslogging_addr, abuslogging)) {
      flash_error = "writeByte abuslogging_addr FAIL";
      StoreFlashError(flash_error, abuslogging_addr);
    }
#endif
  }
  else if (co == '0') {
    Serial.print(F("Start storing variables for this day to prepare for restore."));
    StoreDayTotal();
    Serial.print(F("Variables for this day stored for restore."));
  }
  else if (co == '1') {
#ifdef OPTION_FLASH
    ethrlog = flash.readByte(ethrlog_addr);
#endif
    Serial.print("current Ethernetlogging: ");
    ethrlog ? Serial.println("on") : Serial.println("off");
    Serial.print(" Ethernetlogging switched to: ");
    if (ethrlog == false) {
      ethrlog = true;
    }
    else {
      ethrlog = false;
    }
    ethrlog ? Serial.print("->true (on)") : Serial.print("->false (off)");
#ifdef OPTION_FLASH
    flash.eraseSector(ethrlog_addr);
    if (!flash.writeByte(ethrlog_addr, ethrlog)) {
      flash_error = "writeByte ethrlog_addr FAIL";
      StoreFlashError(flash_error, ethrlog_addr);
    }
#endif
  }
  else if (co == '2') {
#ifdef OPTION_FLASH
    min_serial = flash.readByte(min_serial_addr);
#endif
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
#endif
    Serial.print(F("The new status for Serial logging is: "));
    if (min_serial == false) {
      Serial.print(F("on"));
    }
    else {
      Serial.print(F("off"));
    }
    if (!min_serial && (lastSecond > 5)) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Polling PVI: ", hour(), minute(), second());  // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  else if (co == '3') {
#ifdef OPTION_FLASH
    showbuffer = flash.readByte(showbuffer_addr);
#endif
    if (showbuffer == false) {
      showbuffer = true;
    }
    else {
      showbuffer = false;
    }
#ifdef OPTION_FLASH
    flash.eraseSector(showbuffer_addr);
    if (!flash.writeByte(showbuffer_addr, showbuffer)) {
      flash_error = "writeByte showbuffer_addr FAIL";
      StoreFlashError(flash_error, showbuffer_addr);
    }
#endif
    Serial.print("The new status for weather logging is: ");
    if (showbuffer == true)
      Serial.print("on");
    else
      Serial.print("off");
  }
  else if (co == '4') {
#ifdef OPTION_FLASH
    iplog = flash.readByte(iplog_addr);
#endif
    if (iplog == false) {
      iplog = true;
    }
    else {
      iplog = false;
    }
    Serial.print(F("The new status for iplogging is: "));
    if (iplog == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(iplog_addr);
    if (!flash.writeByte(iplog_addr, iplog)) {
      flash_error = F("writeByte iplog_addr FAIL");
      StoreFlashError(flash_error, iplog_addr);
    }
#endif
  }
  else if (co == '5') {
#ifdef OPTION_FLASH
    maillogging = flash.readByte(maillogging_addr);
#endif
    if (maillogging == false) {
      maillogging = true;
    }
    else {
      maillogging = false;
    }
    Serial.print(F("The new status for maillogging is: "));
    if (maillogging == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(maillogging_addr);
    if (!flash.writeByte(maillogging_addr, maillogging)) {
      flash_error = F("writeByte maillogging_addr FAIL");
      StoreFlashError(flash_error, maillogging_addr);
    }
#endif
  }
  else if (co == '6') {
#ifdef OPTION_FLASH
    pvoutputlog = flash.readByte(pvoutputlog_addr);
#endif
    if (pvoutputlog == false) {
      pvoutputlog = true;
    }
    else {
      pvoutputlog = false;
    }
    Serial.print(F("The new status for pvoutputlogging is: "));
    if (pvoutputlog == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(pvoutputlog_addr);
    if (!flash.writeByte(pvoutputlog_addr, pvoutputlog)) {
      flash_error = F("writeByte pvoutputlog_addr FAIL");
      StoreFlashError(flash_error, pvoutputlog_addr);
    }
#endif
  }
  else if (co == '7') {
#ifdef OPTION_FLASH
    showbufferremaining = flash.readByte(showbufferremaining_addr);
#endif
    if (showbufferremaining == false) {
      showbufferremaining = true;
    }
    else {
      showbufferremaining = false;
    }
    Serial.print(F("The new status for showbufferremaining logging is: "));
    if (showbufferremaining == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(showbufferremaining_addr);
    if (!flash.writeByte(showbufferremaining_addr, showbufferremaining)) {
      flash_error = "writeByte showbufferremaining_addr FAIL";
      StoreFlashError(flash_error, showbufferremaining_addr);
    }
#endif
  }
  else if (co == '8') {
#ifdef OPTION_FLASH
    udpdump = flash.readByte(udpdump_addr);
#endif
    if (udpdump == false) {
      udpdump = true;
    }
    else {
      udpdump = false;
    }
    Serial.print(F("The new status for udpdump logging is: "));
    if (udpdump == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(udpdump_addr);
    if (!flash.writeByte(udpdump_addr, udpdump)) {
      String msgerrflsh = "writeByte udpdump_addr FAIL";
      StoreFlashError(msgerrflsh, udpdump_addr);
    }
#endif
  }
  else if (co == '9') {
    bool oldethrlog = ethrlog;
    ethrlog = true;
    Serial.print("Value for disconnect timer: ");
    Serial.print(DISCONNECT_TIMER);
    Serial.println("seconds");
    for (uint8_t l_sock = 0; l_sock < MAX_SOCK_NUM; l_sock++) {
      // first disable running timers in case CheckSocketConnections has not run uptil this request
      uint8_t l_status = Ethernet.socketStatus(l_sock);
      if ((l_status == 0x22) || (l_status == 0x14) || (l_status == 0x00)) { //only listen and closed are allowed for long time - Force timer reset
        G_SocketConnectionTimers[l_sock] = 0;
      }
      else if (G_SocketConnectionTimers[l_sock] == 0) {
        //If the socket timer is not active but it is currently connected
        //then we start the socket timer and record the current (start) time.
        G_SocketConnectionTimers[l_sock] = millis(); // or with now()
      }
      serprintsockstat(l_sock, false);
      Serial.print(" Forced Socket disconnects: ");
      Serial.print(Sock_DisCon[l_sock]);
      if (G_SocketConnectionTimers[l_sock] != 0 ) {
        Serial.print(", timer active, this status duration: ");
        Serial.print(millis() - G_SocketConnectionTimers[l_sock]);
        Serial.println("ms");
      }
      else {
        Serial.print(", disconnect timer not active. Value disconnection timer: ");
        Serial.print(G_SocketConnectionTimers[l_sock]);
      }
      Serial.print(". Last disconnect time for this socket: ");
      Serial.println(lastSock_DisCon[l_sock]);
    }
    ethrlog = false;
    Serial.print("Number of free sockets: ");
    Serial.print(SerialXltStat());
    ethrlog = oldethrlog;

  }
  else if (co == ':') {
    Serial.print("all sockets are checked, unnecessary sockets are disconnected");
    for (int i = 0; i < MAX_SOCK_NUM; i++) {
      if (( Ethernet.socketStatus(i) != 0 ) && ( Ethernet.socketStatus(i) != 0x14 ) && ( Ethernet.socketStatus(i) != 0x22 )) {
        Ethernet.socketDisconnect(i);
        Sock_DisCon[i]++;
        sprintf(lastSock_DisCon[i], "%s",  DateTime(now()));
        sprintf(temptxtbuff, "\r\nForce disconnect socket: %d", i); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        serprintsockstat(i, true);
      }
    }
  }
  else if (co == ';') {
#ifdef OPTION_FLASH
    HTTPlog = flash.readByte(HTTPlog_addr);
#endif
    if (HTTPlog == false) {
      HTTPlog = true;
    }
    else {
      HTTPlog = false;
    }
    Serial.print(F("The new status for HTTP logging is: "));
    if (HTTPlog == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(HTTPlog_addr);
    if (!flash.writeByte(HTTPlog_addr, HTTPlog)) {
      flash_error = "writeByte HTTPlog_addr FAIL";
      StoreFlashError(flash_error, HTTPlog_addr);
    }
#endif
  }
  else if (co == '<') {
#ifdef OPTION_FLASH
    brlogging = flash.readByte(brlogging_addr);
#endif
    if (brlogging == false) {
      brlogging = true;
    }
    else {
      brlogging = false;
    }
    Serial.print(F("The new status for buienradar.nl (pluvio) logging is: "));
    if (brlogging == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(brlogging_addr);
    if (!flash.writeByte(brlogging_addr, brlogging)) {
      flash_error = "writeByte brlogging_addr FAIL";
      StoreFlashError(flash_error, brlogging_addr);
    }
#endif
  }

  else if (co == '=') {
#ifdef OPTION_FLASH
    WolfSSLrwmail = flash.readByte(wolfSSLMail_addr);
#endif
    if (WolfSSLrwmail == false) {
      WolfSSLrwmail = true;
    }
    else {
      WolfSSLrwmail = false;
    }
    Serial.print(F("The new status for sending email on WolfSSL read/write errors is: "));
    if (WolfSSLrwmail == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(wolfSSLMail_addr);
    if (!flash.writeByte(wolfSSLMail_addr, WolfSSLrwmail)) {
      flash_error = "writeByte wolfSSLMail_addr FAIL";
      StoreFlashError(flash_error, wolfSSLMail_addr);
    }
#endif
  }
  else if (co == '>') {
    if ( wolfssllog == true) {
      wolfssllog = false;
#ifdef DEBUG_WOLFSSL
      wolfSSL_Debugging_OFF();
#else
      Serial.print("The WolfSSL library is compiled without debugging option.");
#endif
    }
    else {
#ifdef DEBUG_WOLFSSL
      if (registerWolfSSLDebug() == 0) {
#endif
        wolfssllog = true;
#ifdef DEBUG_WOLFSSL
      }
#endif
    }
#ifdef OPTION_FLASH
    flash.eraseSector(wolfSSLdbg_addr);
    if (!flash.writeByte(wolfSSLdbg_addr, wolfssllog)) {
      flash_error = "writeByte wolfSSLdbg_addr FAIL";
      StoreFlashError(flash_error, wolfSSLdbg_addr);
    }
    wolfssllog = flash.readByte(wolfSSLdbg_addr);
#endif
    Serial.print("The new status of wolfSSL Debugging is: ");
    Serial.print(wolfssllog ? F("on") : F("off"));
  }
  else if (co == '?') {
    Serial.print("/ - toggles AbuseIPDB logging on/off, actual status: ");
    Serial.println(abuslogging ? "on" : "off");
    Serial.println("0 - stores the day total for restore");
    Serial.print("1 - toggles Ethernet socket logging on/off, actual status: ");
    Serial.println(ethrlog ? "on" : "off");
    Serial.print("2 - toggles Serial logging logging on/off, actual status: ");
    Serial.println(min_serial ? "off (minimal logging)" : "on (extended logging)");
    Serial.print("3 - toggles Show weather logging on/off, actual status: ");
    Serial.println(showbuffer ? "on" : "off");
    Serial.print("4 - toggles iplogging logging on/off, actual status: ");
    Serial.println(iplog ? "on" : "off");
    Serial.print("5 - toggles maillogging logging on/off, actual status: ");
    Serial.println(maillogging ? "on" : "off");
    Serial.print("6 - toggles pvoutputlogging logging on/off, actual status: ");
    Serial.println(pvoutputlog ? "on" : "off");
    Serial.print("7 - toggles showbufferremaining logging on/off, actual status: ");
    Serial.println(showbufferremaining ? "on" : "off");
    Serial.print("8 - toggles udpdump logging on/off, actual status: ");
    Serial.println(udpdump ? "on" : "off");
    Serial.println("9 - show socketstatus");
    Serial.println(": - disconnect sockets");
    Serial.print("; - toggles HTTP traffic logging on/off, actual status: ");
    Serial.println(HTTPlog ? "on" : "off");
    Serial.print("< - toggles buienradar.nl (pluvio) logging on/off, actual status: ");
    Serial.println(brlogging ? "on" : "off");
    Serial.print("= - toggles email on WolfSSL read/write errors on/off, actual status: ");
    Serial.println(WolfSSLrwmail ? "on" : "off");
    Serial.print("> - toggles WolfSSL debug off/on, actual status: ");
    Serial.println(wolfssllog ? "on" : "off");
    Serial.print(". - toggles setsuspicious email sending off/on, actual status: ");
    Serial.println(suspicious_mail ? "on" : "off");
    Serial.println(", - forces a new DHCP request ");
    Serial.print("- - toggles set unknown command email sending off/on, actual status: ");
    Serial.println(unknownCmd_mail ? "on" : "off");
    Serial.print("@ - toggles the logging for TFTP import export (*IPtable*.txt or *HTTP*.txt) off/on, actual status: ");
    Serial.println(InExlog ? "on" : "off");
    if (!min_serial && (lastSecond > 5)) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Polling PVI: ", hour(), minute(), second());  // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  else if (co == '@') {
#ifdef OPTION_FLASH
    InExlog = flash.readByte(InExlog_addr);
#endif
    if (InExlog == false) {
      InExlog = true;
    }
    else {
      InExlog = false;
    }
    Serial.print(F("The new status for TFTP import/export logging is: "));
    if (InExlog == true)
      Serial.print(F("on"));
    else
      Serial.print(F("off"));
#ifdef OPTION_FLASH
    flash.eraseSector(InExlog_addr);
    if (!flash.writeByte(InExlog_addr, InExlog)) {
      flash_error = "writeByte InExlog_addr FAIL";
      StoreFlashError(flash_error, InExlog_addr);
    }
#endif
  }
}


/*
  #if defined (USE_LOGGING) || defined (USE_PWRLOGGING)
  bool SD_OK;
  // set up variables using the SD utility library functions:
  Sd2Card card;


  void SetupSD()
  {
  // make sure that the default chip select pin of SDI is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  // disable w5100 SPI
  digitalWrite(10, HIGH);
  pinMode(53, OUTPUT);
  //     digitalWrite(4, LOW);
  Serial.print(F("\nInitializing SD card..."));
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, 4)) {
    Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("* is a card is inserted?"));
    Serial.println(F("* Is your wiring correct?"));
    Serial.println(F("* did you change the chipSelect pin to match your shield or module?"));
    SD_OK = false;
    return;
  }
  else {
    Serial.println(F("Wiring is correct and a card is present."));
    SdFile::dateTimeCallback(dateTime); // set callback for SD file date
    SD_OK = true;
  }
        if (!SD.begin(4))
      {
          SD_OK = false;
      }
      SdFile::dateTimeCallback(dateTime); // set callback for SD file date
      SD_OK = true;

    digitalWrite(4, HIGH); // deselect SD

  }

  void OpenLogFile()
  {
  if(SD_OK)
  {
  digitalWrite(10, HIGH); // deselect ethernet
    char fileName[20];
    int fileCounter = 0;
    // scan card until file not found
    do
    {
      fileCounter++;
      sprintf(fileName,"%02d%02d_%03d.csv",month(),day(),fileCounter);
    }
    while (SD.exists(fileName) && fileCounter<100);
    // now create the new filename
    logFile = SD.open(fileName, O_CREAT | O_WRITE); // do not use FILE_WRITE. It is 100x slower
    WriteDateToLog();
  digitalWrite(4, HIGH); // deselect SD

  }
  }

  void writePowerLogFile()
  {
  if(SD_OK)
  {
      digitalWrite(10, HIGH); // deselect ethernet

    Serial.println(F("SD found"));
    char fileName[20];
    int fileCounter = 0;
    // scan card until file not found
    do
    {
      fileCounter++;
      sprintf(fileName,"%02d-%02d.csv",year(),month());
      Serial.print(F("Filename: "));
      Serial.println(fileName);
    }
    while (SD.exists(fileName) && fileCounter<100);
    // now create the new filename

    logFile = SD.open(fileName, FILE_WRITE); // do not use FILE_WRITE. It is 100x slower
    WriteDateToLog();
    WriteDatePowerToLog();
    CloseLogFile();
      digitalWrite(4, HIGH); // deselect SD

  }
  }

  void CloseLogFile()
  {
  if(SD_OK)
  {
    logFile.close();
  }
  Serial.println(F("file closed"));
  }

  void WriteDatePowerToLog()
  {
  logFile << totalToday;
  logFile.println(";");
  logFile.flush();
  }
  void WriteDateToLog()
  {
  if(SD_OK)
  {
    Serial.println(F("write date to logfile"));
    logFile << day() << "-" << month() << "-" << year() << ";";
    logFile.flush();
  }
  }

  // this function is called by SD if it wants to know the time
  void dateTime(uint16_t* date, uint16_t* time)
  {
  // return date using FAT_DATE macro to format fields
   date = FAT_DATE(year(), month(), day());
  // return time using FAT_TIME macro to format fields
   time = FAT_TIME(hour(), minute(), second());
  }
  #endif
*/
