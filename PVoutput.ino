#include <avr/pgmspace.h>
IPAddress remote_addr;
IPAddress ip_pvoutput;
time_t pvAdd200ResponseTime;
time_t pv200ResponseTime;
time_t pvResponseTime;
int DnsStatus;
float previous = -1;
char pvFailResponse[264];
char pv200Response[32];
char pvAdd200Response[32];
const static char RespTO2ca[] PROGMEM = "Response timeout";
const static char noConn2ca[] PROGMEM = "No connection";
const static char noInfo2ca[] PROGMEM = "No info available";
int mslen = 0;


// This function will contact the DNS server and ask for an IP address of PvOutput
// If successfull, this address will be used
// If not, keep using the previous found address
// In this way, we can still update to pvoutput if the dns timeouts.
void CheckIpPv()
{
  char cobuff[32];
  DNSClient dns;
  // Look up the host first
  dns.begin(Ethernet.dnsServerIP());
  DnsStatus = dns.getHostByName("pvoutput.org", remote_addr);
  if (DnsStatus == 1) {
    ip_pvoutput = remote_addr; // if success, copy new
    sprintf(temptxtbuff, "host pvoutput.org DNS resolved to: %d.%d.%d.%d ", remote_addr[0], remote_addr[1], remote_addr[2], remote_addr[3]); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  else {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: pvoutput.org DNS resolve error: ", hour(), minute(), second());    // temptxtbuff is max 1024 char
    textlog(temptxtbuff, false);
    itoa(DnsStatus, cobuff, 10);
    textlog(cobuff, false);
    switch (DnsStatus) {
      case -1:
        textStringLog(" -> TIMED_OUT", true);
        break;
      case -2:
        textStringLog(" -> INVALID_SERVER", false);
        break;
      case -3:
        textStringLog(" -> TRUNCATED", false);
        break;
      case -4:
        textStringLog(" -> INVALID_RESPONSE", false);
        break;
      case -5:
        textStringLog(" -> Truncated or invalid response", false);
        break;
      case -6:
        textStringLog(" -> No answer -> invalid response", false);
        break;
      case -9:
        textStringLog(" -> Weird size -> invalid response", false);
        break;
      case -10:
        textStringLog(" -> We haven't found an answer -> invalid response", false);
        break;
      default:
        textStringLog(" -> Unknown", true);
    }
  }
}

// This function updates all registered sensors to pvoutput
void SendToPvOutput()
{
#ifdef OPTION_NO_PVOUTPUT
  Serial.println(F("WARNING: This version is compiled with definition OPTION_NO_PVOUTPUT is true, so no output is uploaded to PVORG."));
  return;
#endif
  String fpstring = "";
  char fpbuff[16];
  EthernetClient pvout;
  unsigned int sid = SID;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start 5 minutes output update -> ", hour(), minute(), second()); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  // create a total for each variable that can be used in pvoutput
  // !! The index in this array starts at 0 while the pvoutput vars start at 1
  float v[12]; // data sum
  bool b[12]; // data present flags
  // start with 0
  for (byte n = 0; n < 12; n++)
  {
    v[n] = 0;             // init the array
    b[n] = false;
  }
  pvout.setTimeout(5000);

  CheckIpPv(); // update the ipaddress via DNS

  if (sid > 0) // only upload if the sid is valid
  {
    int lastPvResponse;
    int res = pvout.connect(ip_pvoutput, 80);
    if (res == 1) // connection successful
    {
      if (!min_serial) {
        textStringLog("-> connection ok -> ", false);
      }
      pvout.setTimeout(2000);
      pvout << F("GET /service/r2/addstatus.jsp");
      if (pvoutputlog) {
        textStringLog("GET /service/r2/addstatus.jsp", false);
      }
      sprintf(temptxtbuff, "?key=%s", PVOUTPUT_API_KEY); // temptxtbuff is max 1024
      pvout << temptxtbuff;
      if (pvoutputlog) {
        textlog(temptxtbuff, false);
      }
      sprintf(temptxtbuff, "&sid=%d", sid); // temptxtbuff is max 1024
      pvout << temptxtbuff;
      if (pvoutputlog) {
        textlog(temptxtbuff, false);
      }
      sprintf(webData, "&d=%04d%02d%02d", year(), month(), day());  //webdata is max 96
      pvout << webData;
      if (pvoutputlog) {
        textlog(webData, false);
      }
      sprintf(webData, "&t=%02d:%02d", hour(), minute());  //webdata is max 96
      pvout << webData;
      if (pvoutputlog) {
        textlog(webData, false);
      }
      int intInvTemp = int(actInvTemp);
      if (int((actInvTemp - intInvTemp) * 10) >= 5) ++intInvTemp;  //round up

      sprintf(webData, "&c1=0&v1=%05u&v2=%04d&v4=%02d", maxtotalToday, peakpower5min, intInvTemp);  //webdata is max 96
      pvout << webData;  // this is the data from the mainloop
      if (pvoutputlog) {
        textlog(webData, false);
      }
      pvout << F("&v5=") << actual << F("&v6=") << actTension;
      if (pvoutputlog) {
        //            Serial.print(F("&v5="));
        //            Serial.print(actual);
        //            Serial.print(F("&v6="));
        //            Serial.print(actTension);
        fpstring = String(actual, 2); //necessary to convert the float to char array, %f does not function
        fpstring.toCharArray(fpbuff, 6);
        fpstring = String(actTension, 2); //necessary to convert the float to char array, %f does not function
        char fpbuff1[16];
        fpstring.toCharArray(fpbuff1, 6);
        sprintf(temptxtbuff, "&v5=%s&v6=%s ", fpbuff, fpbuff1);  // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      pvout << F("\r\nHost: pvoutput.org\r\n\r\n");
      // give pvoutput some time to process the request
      // read the response code. 200 means ok. 0 means that there is no response yet
      lastPvResponse = pvout.parseInt();
      // By some reason pvoutput responds with first two times a '1'and after that with the well known 200
      if (lastPvResponse != 1) {
        sprintf(temptxtbuff, "\r\nERROR lastPvResponse: %d, should be '1'", lastPvResponse); // temptxtbuff is max 1024
        textlog(temptxtbuff, true);
      }
      if (lastPvResponse == 1)
      {
        lastPvResponse = pvout.parseInt();
        if (lastPvResponse != 1) {
          sprintf(temptxtbuff, "\r\nERROR lastPvResponse: %d, should be '1'", lastPvResponse); // temptxtbuff is max 1024
          textlog(temptxtbuff, true);
        }
        if (lastPvResponse == 1)
        {
          lastPvResponse = pvout.parseInt();
          if ((!min_serial || pvoutputlog) && lastPvResponse == 200) {
            sprintf(temptxtbuff, "lastPvResponse: %d ", lastPvResponse); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
        }
      }
      if (lastPvResponse == 200)
      {
        sprintf(pv200Response, "%03d Ok\0", lastPvResponse);
        pv200ResponseTime = now();
        uploadsOk++;
        totalUploadsOk++;
        if (pvoutputlog)
          textStringLog(" -> upload success ", false);
      }
      if (lastPvResponse == 0)
      {
        strcpy_P(pvFailResponse, (char*)RespTO2ca);
        pvResponseTime = now();
        sprintf(temptxtbuff, "\r\nERROR upload may have failed ->  %s\r\n", pvFailResponse); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      else if (lastPvResponse != 200)
      {
        char tempbuff[76];
        size_t numchars = pvout.readBytes(tempbuff, 75);
        if ( numchars > 75 ) {
          numchars = 75;
        }
        tempbuff[numchars] = '\0'; // terminate the string
        sprintf(pvFailResponse, "%03d %s", lastPvResponse, tempbuff);
        EmptyRXbuffer(pvout);
        pvResponseTime = now();
        sprintf(temptxtbuff, "\r\nERROR upload may have failed ->  %s\r\n", pvFailResponse); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      pvout.stop();
    }
    else // cannot connect
    {
      strcpy_P(pvFailResponse, (char*)noConn2ca);
      pvResponseTime = now();
      uploadErr++;
      totalUploadsErr++;
      sprintf(temptxtbuff, "\r\n%sERROR: upload error -> %s ", DateTime(pvResponseTime), pvFailResponse); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);

    }
    if (pvout.connected()) {
      sprintf(temptxtbuff, "\r\n%sERROR: SendToPvOutput stop unexpected connection ", DateTime(pvResponseTime)); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      pvout.stop();
    }
  }
  textStringLog("-> end", false);
}

void AddToPvOutput()
{
#ifdef OPTION_NO_PVOUTPUT
  Serial.print(F("\r\nWARNING: This version is compiled with definition OPTION_NO_PVOUTPUT is true, so no output is uploaded to PVORG."));
  return;
#endif
  EthernetClient pvout;
  unsigned int sid = SID;
  if (sid > 0) // only upload if the sid is valid
  {
    // give pvoutput some time to process the request
    pvout.setTimeout(5000);
    sprintf(temptxtbuff, "\r\n%s Addoutput to PVOutput.org initiated -> ", DateTime(now()));  // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    CheckIpPv(); // update the ipaddress via DNS
    int res = pvout.connect(ip_pvoutput, 80);
    if (res == 1) {
      if (pvoutputlog) {
        textStringLog("connection ok -> ", false);
      }
      char strout0[16];
      String mailstr;
      mailstr.reserve(512);
      mailstr = F("GET /service/r2/addoutput.jsp");
      pvout << mailstr;
      pvout << F("?key=" PVOUTPUT_API_KEY);
      mailstr +=  F("?key=");
      mailstr += PVOUTPUT_API_KEY;
      pvout << F("&sid=") << sid;
      mailstr += F("&sid=");
      itoa(sid, strout0, 10);
      mailstr += strout0;
      sprintf(webData, "&d=%04d%02d%02d", year(), month(), day());  //webdata is max 96
      mailstr += webData;
      pvout << webData;
      sprintf(webData, "&pp=%04d&pt=", PeakPowerObserved);  //webdata is max 96
      mailstr += webData;
      pvout << webData << TimePeakPower;
      mailstr += TimePeakPower;

      int TempMin = int(MinTemp);
      float Temptmp = MinTemp - TempMin;
      if ((abs(Temptmp) * 10) >= 5) {
        if (MinTemp >= 0.0) {
          ++TempMin;
        }
        else {
          --TempMin;
        }
      }
      int TempMax = int(MaxTemp);
      Temptmp = MaxTemp - TempMax;
      if ((abs(Temptmp) * 10) >= 5) {
        if (MaxTemp >= 0.0) {
          ++TempMax;
        }
        else {
          --TempMax;
        }
      }

      pvout <<  F("&tm=") << TempMin << F("&tx=") << TempMax;
      mailstr += F("&tm=");
      mailstr += String(TempMin);
      mailstr += F("&tx=");
      mailstr += String(TempMax);
      pvout << F("&cm=phi=") << luchtvochtigheid << F("%20Riso=") << invRiso << F("%20r=") << totPluvio_br << F("%20t-@") << TimeMinTemp << F("%20") << F("shdw:") << shadow << F("%20");
      if (shadow > 0) {
        pvout <<  firstShadowTime << F("%20") << lastShadowTime << F("%20");
      }
      pvout << string2HTML(weatherCondition1300) << F("@13:00") ;
      mailstr += F("&cm=phi=");
      dtostrf(luchtvochtigheid, 5, 0, strout0);
      mailstr +=  strout0;
      mailstr += F("%20Riso=");
      dtostrf(invRiso, 4, 2, strout0);
      mailstr += strout0;
      mailstr += F("%20r=");
      dtostrf(totPluvio_br, 4, 2, strout0);
      mailstr += strout0;
      mailstr += F("%20t-@");
      mailstr += TimeMinTemp;
      mailstr += F("%20");
      mailstr += F("shdw:");
      mailstr += shadow;
      if (shadow > 0) {
        mailstr += F("%20");
        mailstr += firstShadowTime;
        mailstr += F("%20");
        mailstr += lastShadowTime;
      }
      mailstr += F("%20");
      mailstr += string2HTML(weatherCondition1300);
      mailstr += F("@13:00\r\n\0");
      mslen = mailstr.length();
      if (pvoutputlog) {
        sprintf(temptxtbuff, "mail string length: %d\r\n", mslen); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      if ( mslen >= 511 )
      {
        //        String tempstr = F("\r\nERROR: mailstr to long, truncated");
        //        tempstr.toCharArray( mailerrortype, tempstr.length() + 1);  // Die + 1 kan hier blijven omdat de terminator niet geforceerd zoals bij aanpassing van de string
        sprintf(mailerrortype, "\r\nERROR: mailstr to long %d, truncated to 510", mslen);  // mailerrortype is max 64 chars
        textlog(mailerrortype, true);
        mslen = 511;
      }
      mailstr.toCharArray( maillog, mslen + 1);  // maillog is max 512 chars
      //      maillog[mslen] = '\0'; //toCHarArray sets terminator
      pvout << F("\r\nHost: pvoutput.org\r\n\r\n");
      if (pvoutputlog) {
        textStringLog("Upload Add to PVoutput posted, waiting for response code\r\n", false);
      }
      // read the response code. 200 means ok. 0 means that there is no response yet
      int lastResponse = pvout.parseInt();
      // in feb 2016 the responsecode changed to 001.1 200
      if (lastResponse == 1)
      {
        lastResponse = pvout.parseInt();
        if (lastResponse == 1)
        {
          lastResponse = pvout.parseInt();
        }
      }
      if (lastResponse == 200)
      {
        sprintf(pvAdd200Response, "%03d Ok\0", lastResponse);
        pvAdd200ResponseTime = now();
        uploadsOk++;
        totalUploadsOk++;
        AlreadyAdded = true;
        //store the flag if already updated output
#ifdef OPTION_FLASH
        flash.eraseSector(AlreadyAdded_addr);
        if (!flash.writeByte(AlreadyAdded_addr, AlreadyAdded)) {
          flash_error = F("writeByte AlreadyAdded_addr FAIL");
          StoreFlashError(flash_error, AlreadyAdded_addr);
        }
        delay(3);
#endif
        textStringLog("upload ok -> ", false);
      }
      if (lastResponse != 200)
      {
        if (lastResponse == 0)
        {
          strcpy_P(pvFailResponse, (char*)RespTO2ca);
        }
        else {
          textStringLog("\r\nERROR: Upload error, collecting error message -> ", false);
          char tempbuff[256];
          if (pvout.available() == 0) {
            strcpy_P(tempbuff, (char*)noInfo2ca);
          }
          else {
            size_t numchars = pvout.readBytes(tempbuff, 255);
            if (numchars > 255) {
              textStringLog("\r\nERROR: unexpected number of response characters", true);
              numchars = 255;
            }
            tempbuff[numchars] = '\0'; // terminate the string
          }
          sprintf(pvFailResponse, "%03d %s", lastResponse, tempbuff);
        }
        pvResponseTime = now();
        uploadErr++;
        totalUploadsErr++;
        sprintf(temptxtbuff, "\r\nERROR upload error -> %s\r\n", pvFailResponse); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      EmptyRXbuffer(pvout);
      pvout.stop();
    }
    else // cannot connect
    {
      strcpy_P(pvFailResponse, (char*)noConn2ca);
      pvResponseTime = now();
      sprintf(temptxtbuff, "\r\n%s ERROR: connection error -> %s\r\n", DateTime(pvResponseTime), pvFailResponse); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    if (pvout.connected()) {
      textStringLog("\r\nERROR: AddToPvOutput stop unexpected connection\r\n", false);
      pvout.stop();
    }
  }
  else {
    textStringLog("\r\nERROR: PVoutput SID is zero, check userdefs.h\r\n", false);
  }
  textStringLog("end", false);
}

void EmptyRXbuffer(EthernetClient pvout)
{

  if (pvoutputlog && showbufferremaining) {
    sprintf(temptxtbuff, "%d characters available, emptying buffer -> ", pvout.available()); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  while (pvout.available()) {   //empty readbuffer
    if (pvoutputlog && showbufferremaining) {
      int nrofchr2read;
      while (pvout.available()) {
        nrofchr2read = pvout.available();
        if (nrofchr2read > 1000) {
          nrofchr2read = 1000;
          pvout.readString().toCharArray(temptxtbuff, nrofchr2read); // temptxtbuff is max 1024
        }
        else
          pvout.readString().toCharArray(temptxtbuff, nrofchr2read + 1); // temptxtbuff is max 1024
        if (pvoutputlog && showbufferremaining) {
          textlog(temptxtbuff, true);
          sprintf(temptxtbuff, "\r\n%d characters remaining", pvout.available()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
    }
    else {
      pvout.read();
    }
  }
}
