//
// Temperature
// Retrieves the actual temperature
EthernetClient buienradarClient;
/*
  void dump_tekst(String Stringbuff) {
  int sl = Stringbuff.length();
  byte termb = Stringbuff[sl + 1];
  textStringLog("\r\n", false);
  for (int il = 0; il <= sl + 1; il++) {
    sprintf(temptxtbuff, "char pos:%d = %c => 0x%02x", il, Stringbuff[il], (byte)Stringbuff[il]); // temptxtbuff is max 1024
    textlog(temptxtbuff, true);
  }
  sprintf(temptxtbuff, "\r\textcontent: stringlengte=%d, string terminator voor force=0x%2x, na force=0x%2x terminator placed at position %d, pos %d contains 0x%02x", sl, termb, (byte)Stringbuff[sl + 1], sl + 1, sl, (byte)Stringbuff[sl]); // temptxtbuff is max 1024
  textlog(temptxtbuff, true);
  }
*/

void init_Temperature()
{
  showbuffer = false;
  weatherStation = nrwthsttn;
  //  totPluvio_htn = 0.0;
  totPluvio_br = 0.00;
  last_neerslagintensiteit = 0.00;
  max_neerslagintensiteit = 0.00;
  lastpluvioTime_br = 0;
  maxpluvioTime_br = 0;
  pluviosucc_br = 0;
  resetmaxpluvio = false;
  //init the forecast array for next 2 hours
  int i = 0;
  for ( i = 0; i < 24 ; i++ ) {
    arrpluvio_br[i] = 0;
    arrhour_br[i] = 0;
    arrminute_br[i] = 0;
  }
}

void Begin_Temperature()
{
  if (MinTemp == 255.0) {
    MinTemp = actual;
    sprintf(TimeMinTemp, "%02d:%02d", hour(), minute());  // TimeMinTemp is max 16 char
  }
  if (MaxTemp == -255.0) {
    MaxTemp = actual;
    sprintf(TimeMaxTemp, "%02d:%02d", hour(), minute());  // TimeMaxTemp is max 16 char
  }
  todayCnt = 0;
  todayErrCnt = 0;
  todayCnt_br = 0;
  todayErrCnt_br = 0;
  MaxFailErrCnt_br = 0;
  pluviosucc_br = 0;
  hk = 0;
  force_temperature = false; // forces to disable update with large differences
}

void BR_Loop(int m)
{
  if ( (m % 5) == 4) {
    if (brlogging) {
      textStringLog("\r\nPluvio: Executing 5 minutes interval", false);
    }
    br_rslt = false;
    hk = 0;
    GetPluvio_Br(hk);
    if ( !br_rslt) {
      hk++;
    }
  }
  else {
    if (!br_rslt) {
      if (brlogging) {
        textStringLog("\r\nPluvio: executing 1 minute interval, due to previous error or startup sequence", false);
      }
      GetPluvio_Br(hk);
      if ( !br_rslt) {
        hk++;
      }
    }
    else {
      if (brlogging) {
        textStringLog("Pluvio: no action", false);
      }
    }
  }
  //  sprintf(c_tijd_Temp, "%02d:%02d:%02d", hour(), minute(), second());  // c_tijd_Temp is max 16 char
  // limit the weather temperature reading to once every 10 minutes because the source has the same interval
  // in case of previous error after retry 2 minutes
  if (((m % 10) == 6) || (!GetTemp_Ok && ((m % 10) == 8))) {
    GetTemperature();
  }
}


void ErrorTrapGetTemp(EthernetClient weerliveErrorclnt) {
  GetTemp_Ok = false;
  todayErrCnt++;
  while (weerliveErrorclnt.available()) {   //empty readbuffer
    weerliveErrorclnt.read();
  }
  weerliveErrorclnt.stop();
}
/*
   response from weerlive: curl http://weerlive.nl/api/json-10min.php?locatie=Houten
   { "liveweer": [{"plaats": "Houten", "temp": "7.3", "gtemp": "7.3", "samenv": "Mist", "lv": "98", "windr": "ZO", "windms": "0", "winds": "1", "windk": "0", "windkmh": "0", "luchtd": "1031.2", "ldmmhg": "773", "dauwp": "7", "zicht": "1", "verw": "Rustig najaarsweer met aanvankelijk nog enkele mistbanken", "sup": "08:11", "sunder": "18:36", "image": "mist", "d0weer": "halfbewolkt", "d0tmax": "15", "d0tmin": "4", "d0windk": "1", "d0windknp": "2", "d0windms": "1", "d0windkmh": "4", "d0windr": "Z", "d0neerslag": "0", "d0zon": "44", "d1weer": "bewolkt", "d1tmax": "16", "d1tmin": "6", "d1windk": "2", "d1windknp": "4", "d1windms": "2", "d1windkmh": "7", "d1windr": "ZW", "d1neerslag": "10", "d1zon": "30", "d2weer": "halfbewolkt", "d2tmax": "14", "d2tmin": "10", "d2windk": "2", "d2windknp": "6", "d2windms": "3", "d2windkmh": "11", "d2windr": "N", "d2neerslag": "20", "d2zon": "40", "alarm": "0"}]}
*/
void GetTemperature()
{
#ifdef OPTION_NO_WEATHER
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Retrieving weather information disabled in build.\r\n", hour(), minute(), second());
  textlog(temptxtbuff, false);
  return;
#endif

  char GTlookfor[48];  // buffer die string uit progmem bevat om te zoeken
  char fpbuff[16];
  char fpbuff1[16];
  char fpbuff2[16];
  String fpstring = "";
  String Stringbuff;
  EthernetClient weerliveClient;
  weerliveClient.setTimeout(5000);
  unsigned long startWeatherProc = millis();
  GetTemp_Ok = true;
  tmp_tijd = now();
  //  textStringLog("\r\n", false); // tijd is also  used in webpage, the new line is only used for the console port
  sprintf(c_tijd_Temp, "%02d:%02d:%02d", hour(tmp_tijd), minute(tmp_tijd), second(tmp_tijd));  // c_tijd_Temp is max 16 char
  sprintf(temptxtbuff, "\r\n%s Start collecting weather information -> ", c_tijd_Temp);  // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  // Get temperature from weerlive
  int sl = 0;
  byte termb;
  const static char host[] PROGMEM = "weerlive.nl";
  strcpy_P(GTlookfor, (char*) host);  // GTlookfor is max 48 char
  if (!min_serial) {
    sprintf(temptxtbuff, "connecting to host: %s", GTlookfor); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  if (weerliveClient.connect(GTlookfor, 80))
  {
    if (!min_serial) {
      textStringLog("-> Connected, starting weather update -> ", false);
    }
    // Initialize the client library
    weerliveClient << F("GET /api/json-data-10min.php?key=") << WEERLIVE_KEY << F("&locatie=") << GPS_LOC << F(" HTTP/1.1\r\n");
    weerliveClient << F("Accept: text/html\r\n");
    weerliveClient << F("Accept-Encoding: identity\r\n");
    weerliveClient << F("Accept-Language: en-US,en;q=0.5\r\n");
    weerliveClient << F("Host: weerlive.nl\r\n");
    weerliveClient << F("Upgrade-Insecure-Requests: 1\r\n");
    weerliveClient << F("Connection: close\r\n\r\n");
    if (showbuffer) {
      sprintf(temptxtbuff, "\r\nsearching weatherstation location: %s", weatherStation);  // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    const static char lookforweer[] PROGMEM = "liveweer";
    strcpy_P(GTlookfor, (char*) lookforweer);   // GTlookfor is max 48 char
    if (weerliveClient.find(GTlookfor)) {
      todayCnt++;
      /* een oplossing voor deprecated conversion from string constant to 'char*' is eerst een char definieren */
      const static char lookforMS[] PROGMEM = "plaats\": \"";
      strcpy_P(GTlookfor, (char*) lookforMS);  // GTlookfor is max 48 char
      if (weerliveClient.find(GTlookfor)) {
        // positioned, now read the station
        Stringbuff = weerliveClient.readStringUntil('"');
        sl = Stringbuff.length();
        if ( sl > 10 )
        {
          sprintf(temptxtbuff, " -> Readed weatherstation %s name length: %d ->  truncated to max length 10: ", Stringbuff, sl); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          sl = 10;
        }
        Stringbuff.toCharArray(meetStation, sl + 1);  // meetStation is max 16 char
        //        meetStation[sl] = '\0';
        if (showbuffer) {
          //          dump_tekst(Stringbuff);
          //          dump_tekst(meetStation);
          sprintf(temptxtbuff, " -> weatherstation found: %s", meetStation); // temptxtbuff is max 1024
          textlog(temptxtbuff, true);
        }
      }
      else {
        sprintf(weatherErrTime_br, "plaatsNF@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        sprintf(temptxtbuff, "ERROR plaats, Weather conditions for location %s not found.", weatherStation); // temptxtbuff is max 1024
        textlog(temptxtbuff, true);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforTGC[] PROGMEM = "temp\": \"";
      strcpy_P(GTlookfor, (char*)lookforTGC);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor)) {
        actual = weerliveClient.parseFloat();
        if (showbuffer) {
          //          fpstring = String(actual, 2); //necessary to convert the float to char array, %f does not function
          //          fpstring.toCharArray(fpbuff, 6);
          /* 4 is minimum width, 2 is precision; float value is copied onto fpbuff*/
          dtostrf(actual, 4, 2, fpbuff);
          sprintf(temptxtbuff, "temp found actual temperature: %s", fpbuff); // temptxtbuff is max 1024
          textlog(temptxtbuff, true);
          //          fpstring = String(prevActual, 2); //necessary to convert the float to char array, %f does not function
          //          fpstring.toCharArray(fpbuff, 6);
          dtostrf(prevActual, 4, 2, fpbuff);
          sprintf(temptxtbuff, "todayCnt=%d prevActual=%s", todayCnt, fpbuff); // temptxtbuff is max 1024
          textlog(temptxtbuff, true);
        }
        if ((todayCnt == 1) && (prevActual == 0.0) || isnan(prevActual)) {
          if (showbuffer) {
            textStringLog("\r\n", false);
          }
          prevActual = actual;
          textStringLog("First run, force prevActual to actual temperature. ", false);
        }
        float diff = actual - prevActual;
        float absdiff = abs(diff);
        // more than 2 degree fluctuation in 10 minutes is nearly unbelievable, but this protects against 0.0 misreads of the KNMI equipment
        if ((absdiff > 2) && (todayCnt != 1) && (actual == 0.0)) {
          //skipped at startup because at start of the day todayCnt==1
          // we use the previous value
          // absdiff = 0;
          // we use the previous value and let prevActual untouched
          actual = prevActual;
          textStringLog("ERROR To large temperature difference in 10 minutes interval in combination with 0.0 reading, force actual to prevActual temperature", true);
          sprintf(weatherErrTime_br, "unb0.0rd@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        }
        //        if ((absdiff < 2) && ((actual != 0.0) || (prevActual != 0.0)) ) {
        // absdiff adapted from 5 to 25 due to more reliable reading
        if (showbuffer) {
          fpstring = String(diff, 2); //necessary to convert the float to char array, %f does not function
          fpstring.toCharArray(fpbuff, 6);
          fpstring = String(absdiff, 2); //necessary to convert the float to char array, %f does not function
          fpstring.toCharArray(fpbuff1, 6);
          fpstring = String(actual, 2); //necessary to convert the float to char array, %f does not function
          fpstring.toCharArray(fpbuff2, 6);
          sprintf(temptxtbuff, "diff=%s absdiff=%s actual=%s", fpbuff, fpbuff1, fpbuff2); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        if (((absdiff <= 5) && (actual != 0.0)) || ((actual == 0.0) && (absdiff < 0.3)) || force_temperature) {
          if (force_temperature) {
            fpstring = String(actual, 2); //necessary to convert the float to char array, %f does not function
            fpstring.toCharArray(fpbuff, 6);
            sprintf(temptxtbuff, "\r\nForces min/max temperature updating, actual = %s", fpbuff); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            MinTemp = actual;
            MaxTemp = actual;
            sprintf(TimeMinTemp, "%02d:%02d", hour(), minute());  // TimeMinTemp is max 16 char
            sprintf(TimeMaxTemp, "%02d:%02d", hour(), minute());  // TimeMaxTemp is max 16 char
          }
          if (actual < MinTemp) {
            MinTemp = actual;
            sprintf(TimeMinTemp, "%02d:%02d", hour(), minute());  // TimeMinTemp is max 16 char
            if (showbuffer) {
              fpstring = String(MinTemp, 2); //necessary to convert the float to char array, %f does not function
              fpstring.toCharArray(fpbuff, 6);
              sprintf(temptxtbuff, "\r\nSet MinTemp to: %s @ %s", fpbuff, TimeMinTemp); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
            }
          }
          if (actual > MaxTemp) {
            MaxTemp = actual;
            sprintf(TimeMaxTemp, "%02d:%02d", hour(), minute());  // TimeMaxTemp is max 16 char
            if (showbuffer) {
              fpstring = String(MaxTemp, 2); //necessary to convert the float to char array, %f does not function
              fpstring.toCharArray(fpbuff, 6);
              sprintf(temptxtbuff, "\r\nSet MaxTemp to: %s @ %s", fpbuff, TimeMaxTemp); // temptxtbuff is max 1024
              textlog(temptxtbuff, false);
            }
          }
          prevActual = actual;
        }
        else {
          if (absdiff > 5) {
            fpstring = String(prevActual, 2); //necessary to convert the float to char array, %f does not function
            fpstring.toCharArray(fpbuff, 6);
            fpstring = String(actual, 2); //necessary to convert the float to char array, %f does not function
            fpstring.toCharArray(fpbuff1, 6);
            sprintf(temptxtbuff, "\r\nERROR: Set Min/MaxTemp skipped due to too large difference actual and previous actual temperature reading.\r\nPrevious actual = %s, Actual = %s", fpbuff, fpbuff1); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            sprintf(weatherErrTime_br, "tdiff>5@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
            prevActual = actual; // to prevent infinite denying a large temperature difference
            ErrorTrapGetTemp(weerliveClient);
            return;
          }
        }
      }
      else {
        sprintf(weatherErrTime_br, "tempNFnd@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR temp, Weather temperature not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforZIN[] PROGMEM = "samenv\": \"";
      strcpy_P(GTlookfor, (char*)lookforZIN);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor)) {
        Stringbuff = weerliveClient.readStringUntil('"');
        sl = Stringbuff.length();
        if (showbuffer) {
          char weathertemptxtbuff[1024];
          if (sl > 900) {
            sl = 900;
          }
          Stringbuff.toCharArray(weathertemptxtbuff, sl + 1);
          //          termb = weathertemptxtbuff[sl];
          //          weathertemptxtbuff[sl] = '\0';
          //          sprintf(temptxtbuff, "weathertemptxtbuff: stringlengte=%d, string terminator voor force=0x%02x, na force=0x%02x terminator placed at position %d", sl, termb, (byte)weathertemptxtbuff[sl], sl); // temptxtbuff is max 1024
          //          textlog(temptxtbuff, true);
          sprintf(temptxtbuff, "\r\nchar array converted weather to: %s, length weather resume: %d", weathertemptxtbuff, sl); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        if ((Stringbuff[0] != '"') || (sl != 0))   // check for empty string
        {
          if ( sl > 59 )
          {
            sprintf(temptxtbuff, "\r\nERROR: too long (> 59), current weather resume length: %d ->  truncated to max length 60: ", sl); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            sl = 59;
          }
          Stringbuff.toCharArray(weatherType, sl + 1); // weatherType is max 64 char
          termb = weatherType[sl];
          //          weatherType[sl] = '\0'; // toCharArray sets terminator
          if (showbuffer) {
            //            sprintf(temptxtbuff, "samenv: stringlengte=%d, string terminator voor force=0x%02x, na force=0x%02x terminator placed at position %d", sl, termb, (byte)weatherType[sl], sl); // temptxtbuff is max 1024
            //            Serial.println(temptxtbuff);
            sprintf(temptxtbuff, " -> Weather actueel in char array: %s", weatherType); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
        }
      }
      else {
        sprintf(weatherErrTime_br, "nosamenv@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR: samenv, Weather temperature not found.", true);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforLVH[] PROGMEM = "lv\": \"";
      strcpy_P(GTlookfor, (char*)lookforLVH);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor)) {
        lv_phi = weerliveClient.parseInt();
        if (showbuffer) {
          sprintf(temptxtbuff, "\r\nlv found, actual humidity: %d", lv_phi); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "no_lv@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR: lv, relative air humidity not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforWR[] PROGMEM = "windr\": \"";
      strcpy_P(GTlookfor, (char*)lookforWR);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor))
      {
        Stringbuff = weerliveClient.readStringUntil('"');
        sl = Stringbuff.length();
        if ( sl > 5 )
        {
          sprintf(temptxtbuff, "\r\nERROR: windr, Wind direction length: %d ->  truncated to max length 5: ", sl); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          sl = 5;
        }
        Stringbuff.toCharArray(windrichting, sl + 1); // windrichting is max 8
        //        windrichting[sl] = '\0'; // toCharArray sets terminator
        if (showbuffer) {
          sprintf(temptxtbuff, "\r\nwindr found, char array wind direction: %s", windrichting); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "nowindr@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR windr, Wind direction not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforWSHMS[] PROGMEM = "windms\": \"";
      strcpy_P(GTlookfor, (char*)lookforWSHMS);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor))
      {
        windsnelheidMS = weerliveClient.parseFloat();
        if (showbuffer) {
          fpstring = String(windsnelheidMS, 2); //necessary to convert the float to char array, %f does not function
          fpstring.toCharArray(fpbuff, 6);
          sprintf(temptxtbuff, "\r\nwindms found, actual wind speed (m/s): %s", fpbuff); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "nowindms@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("ERROR: Wind speed in m/s not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforWSHBft[] PROGMEM = "winds\": \"";
      strcpy_P(GTlookfor, (char*)lookforWSHBft);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor)) {
        windkrachtBft = weerliveClient.parseInt();
        if (showbuffer) {
          sprintf(temptxtbuff, "\r\nwinds found, actual wind force: %dBft", windkrachtBft); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "nowinds@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("ERROR winds, wind force in Bft not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforLD[] PROGMEM = "luchtd\": \"";
      strcpy_P(GTlookfor, (char*)lookforLD);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor))
      {
        luchtdruk = weerliveClient.parseFloat();
        if (showbuffer) {
          //          fpstring = String(luchtdruk, 2); //necessary to convert the float to char array, %f does not function
          //          fpstring.toCharArray(fpbuff, 7);
          dtostrf(luchtdruk, 4, 2, fpbuff);
          sprintf(temptxtbuff, "\r\nluchtd found, air pressure (mbar): %s", fpbuff); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "noluchtd@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR: luchtd, air pressure not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforZM[] PROGMEM = "zicht\": \"";
      strcpy_P(GTlookfor, (char*)lookforZM);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor)) {
        visibility = weerliveClient.parseInt();
        visibility *= 1000;
        if (showbuffer) {
          sprintf(temptxtbuff, "\r\nzicht, visibility (meters) found: %d", visibility); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "nozicht@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR: zicht, sight distance not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforSVT[] PROGMEM = "verw\": \"";
      strcpy_P(GTlookfor, (char*)lookforSVT);
      if (GetTemp_Ok && weerliveClient.find(GTlookfor)) {
        Stringbuff = weerliveClient.readStringUntil('"');
        sl = Stringbuff.length();
        if (showbuffer) {
          char strverw[1024];
          if (sl > 900) {
            sl = 900;
          }
          Stringbuff.toCharArray(strverw, sl + 1);
          //          strverw[sl] = '\0'; // toCharArray sets terminator
          sprintf(temptxtbuff, "\r\nverw found, length: %d -> String Weather expectation: %s", sl, strverw); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        if ( sl > 382 )
        {
          sprintf(temptxtbuff, "\r\nERROR, Weather expectation length: %d ->  truncated to max length 382.", sl); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          sl = 382;
        }
        Stringbuff.toCharArray(samenvatting, sl + 1);  // samenvatting is max 384 char
        //        samenvatting[sl] = '\0'; // toCharArray sets terminator
        if (showbuffer ) {
          sprintf(temptxtbuff, "\r\nverw found converted to char array, length: %d -> Weather expectation: %s", sl, samenvatting); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "noverw@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR: verw, Weather forecast not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      const static char lookforX30[] PROGMEM = "image\": \"";
      strcpy_P(GTlookfor, (char*)lookforX30);  // GTlookfor is max 48 char
      if (GetTemp_Ok && weerliveClient.find(GTlookfor)) {
        Stringbuff = weerliveClient.readStringUntil('"');
        sl = Stringbuff.length();
        if (showbuffer) {
          char strimage[32];
          if (sl > 31) {
            sl = 31;
          }
          Stringbuff.toCharArray(strimage, sl + 1);
          //          strimage[sl] = '\0'; // toCharArray sets terminator
          sprintf(temptxtbuff, "\r\niconname in stringBuff: %s", strimage); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
        //        sl = Stringbuff.length();
        if ( sl > 12 )
        {
          sprintf(temptxtbuff, "\r\nERROR -> Icon name length: %d ->  truncated to max length 12: ", sl); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          sl = 12;
        }
        Stringbuff.toCharArray(icon_temperature, sl + 1);  // icon_temperature is max 16 char
        //        icon_temperature[sl] = '\0'; // toCharArray sets terminator
        if (showbuffer) {
          sprintf(temptxtbuff, " -> char array filename weather icon: %s ", icon_temperature); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
      else {
        sprintf(weatherErrTime_br, "noimage@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
        textStringLog("\r\nERROR: image, icon image name not found.", false);
        ErrorTrapGetTemp(weerliveClient);
        return;
      }
      unsigned long p_tijd = millis() - startWeatherProc;
      //      Serial.print(F("weather processing time in ms: "));
      //      Serial.println(p_tijd);
      int minut = p_tijd / 60000;
      p_tijd = p_tijd - ( minut * 60000);
      //      Serial.println(p_tijd);
      int sec = p_tijd / 1000;
      p_tijd = p_tijd - ( sec * 1000);
      //      Serial.println(p_tijd);
      sprintf(weatherProcTime, "%02d:%02d:%03d", minut, sec, p_tijd);  // weatherProcTime is max 16 char
      if (showbuffer) {
        sprintf(temptxtbuff, "\r\nWeather updated. Weather processing time: %s -> ", weatherProcTime); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      while (weerliveClient.available()) {   //empty readbuffer
        weerliveClient.read();
      }
      if (showbuffer) {
        textStringLog("Ready -> ", false);
      }
    }
    else {
      sprintf(weatherErrTime_br, "lwnotFnd@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
      textStringLog("\r\nERROR: liveweer, datagram start not found -> ", false);
      ErrorTrapGetTemp(weerliveClient);
      return;
    }
    weerliveClient.stop();
  }
  else {
    sprintf(weatherErrTime_br, "WLnoConn@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // weatherErrTime_br is max 24 char
    textStringLog("\r\nERROR: No connection with weerlive.nl", false);
    GetTemp_Ok = false;
    todayErrCnt++;
  }
  if (weerliveClient.connected()) {
    textStringLog("\r\nERROR: GetTemperature stop unexpected connection", false);
    weerliveClient.stop();
  }
  textStringLog("get weather result -> ", false);
  if (!GetTemp_Ok) {
    textStringLog("error", false);
  }
  else {
    textStringLog("ok", false);
  }
}

int EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx) {
  int sent;
  if (wolfssllog) {
    Serial.print(F("\r\nEntering WolfSSL EthernetSend, chars offered: "));
    Serial.print(sz);
    Serial.print(' ');
  }
  sent = buienradarClient.write((byte*)msg, sz);
  if (wolfssllog) {
    sprintf(mailcontent, "Number of bytes written to wtlsclient: %d", sz, sent);   //max is 80
    Serial.print(mailcontent);
  }
  if (sent == 0) {
    if (buienradarClient.connected()) {
      Serial.println();
      sprintf(mailbrstate, "ERROR: It seems that we have still connection to wolftlsclient");    //max is 80
      Serial.print(mailbrstate);
    }
    else {
      Serial.println();
      sprintf(mailbrstate, "ERROR: It seems that we have lost connection to wolftlsclient for some reasons");  //max is 80, this is 80 chars!!!
      Serial.print(mailbrstate);
      SerInpRcdv = true; // set flag that we eventually print the polling pvi due to this time delay
    }
    if (!mailSendErrorflag && WolfSSLrwmail) {
      if (wolfssllog) {
        Serial.print(" -> Storing vars and sending mail");
      }
      mailSendErrorflag = true;
      if (!sendEmail(12)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail for WolfSSL send error.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
    errorwolfsslwantwrite++;
    StoreDayTotal();
    return WOLFSSL_CBIO_ERR_WANT_WRITE;
  }
  if (wolfssllog) {
    Serial.print('<');
  }
  return sent;
}

int EthernetReceive(WOLFSSL* ssl, char* reply, int sz, void* ctx) {
  int ret = 0;
  int tmt = 0;
  if (wolfssllog) {
    Serial.print(F("\r\nEntering WolfSSL EthernetReceive, chars requested: "));
    Serial.print(sz);
  }
  /* Onderstaande 2 whiles lijken in combinatie goed te werken */
  /*
      while ((buienradarClient.available() < sz) && (tmt < 200)) {
        Serial.print('.');
        delay(5);
        tmt++;
      }
      Serial.print('+');
      while (buienradarClient.available() > 0 && ret < sz) {
        reply[ret++] = buienradarClient.read();
      }

    /* De onderstaande while werkt nu ook */
  while ((tmt < 400) && (ret < sz)) {
    if (buienradarClient.available()) {
      reply[ret++] = buienradarClient.read();
    }
    else {
      if (wolfssllog) {
        Serial.print('.');
      }
      delay(5);
      tmt++;
    }
  }
  if ((ret > 0) && wolfssllog) {
    Serial.print(" Number of bytes received from buienradar: ");
    Serial.print(ret);
  }
  if ((ret == 0) || (ret < sz)) {
    sprintf(mailbrstate, "\r\nERROR: Insufficient response, received: %d, expected bytes: %d", ret, sz);  //max is 80, this is 80 chars!!!
    Serial.println(mailbrstate);
    errorwolfsslwantread++;
    StoreDayTotal();
    if (!mailRcvErrorflag && WolfSSLrwmail) {
      if (wolfssllog) {
        Serial.print('m');
      }
      mailRcvErrorflag = true;
      if (!sendEmail(13)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail Failed in WolfSSL receive function.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
    if (wolfssllog) {
      Serial.print(']');
    }
    return WOLFSSL_CBIO_ERR_WANT_READ;
  }
  if (wolfssllog) {
    Serial.print('>');
  }
  return ret;
}

void GetPluvio_Br(int hrk)
{
  bool read_valid = false;
  unsigned int buffcharleft;
  int err = 0;
  int input = 0;
  int total_input = 0;
  int remainmin;
  int startdata = 0;
  int sepa = 0;
  byte arrpos = 0;
  const static char server[] = "gpsgadget.buienradar.nl";
  const static char deepurl[] = "/data/raintext/?lat=52.02&lon=5.17 HTTP/1.1";
  char errBuf[88];
  char replybuff[4096];
  char GPBfpbuff[16];
  char GPBfpbuff2[16];
  String GPBfpstring = "";
  String restresp = "";
  char GPBmsg[] = "GET /data/raintext/?lat=52.02&lon=5.17 HTTP/1.1\r\nUser-Agent: SSLClientOverEthernet\r\nHost: gpsgadget.buienradar.nl\r\nConnection: close\r\n\r\n";
  int GPBmsgSz  = (int)strlen(GPBmsg);
  int tmo = 0;
  int minuten, reqminute, uur, requur, Neerslag, oldNeerslag, pl;
#ifdef OPTION_NO_PLUVIO
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d INFORMATION: Retrieving precipitation information disabled in build.\r\n", hour(), minute(), second());
  textlog(temptxtbuff, false);
  return;
#endif
  if (pluvio_ena == false) {
    textlog("INFORMATION: Reading pluvio from buienradar.nl is disabled", false);
    br_rslt = true;
    return;
  }
  buienradarClient.setTimeout(15000);
  WOLFSSL_CTX* ctx = NULL;
  WOLFSSL_METHOD* method;
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
  if (wolfssllog) {
    Serial.println(F("\r\nMemory usage at start getpluvio:"));
    freeRam();
  }
#endif
#endif
  method = wolfTLSv1_2_client_method();
  if (method == NULL) {
    textlog("\r\nERROR: unable to get wolfTLSv1_2_client_method", false);
    return;
  }
  else {
    if (wolfssllog) {
      textlog("\r\nget wolfTLSv1_2_client_method: success -> ", false);
    }
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
    if (wolfssllog) {
      Serial.println(F("\r\nMemory after getting client method:"));
      freeRam();
    }
#endif
#endif
  }
  ctx = wolfSSL_CTX_new(method);
  if (ctx == NULL) {
    textlog("\r\n ERROR: unable to get wolfSSL ctx", false);
    return;
  }
  else {
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
    if (wolfssllog) {
      Serial.println(F("\r\nMemory usage after loading ctx:"));
      freeRam();
    }
#endif
#endif
    if (wolfssllog) {
      textlog(" wolfSSL ctx get: success", false);
    }
  }
  /* initialize wolfSSL using callback functions */
  wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);
  wolfSSL_SetIOSend(ctx, EthernetSend);
  wolfSSL_SetIORecv(ctx, EthernetReceive);


  tmp_tijd = now();
  sprintf(br_tijd, "%02d:%02d:%02d", hour(tmp_tijd), minute(tmp_tijd), second(tmp_tijd));  // br_tijd is max 16 char
  // adapt the time to the nearest 5 minutes interval
  requur = hour(tmp_tijd);
  /*
    if ( minute(tmp_tijd) >= 58 ) {
      reqminute = 0;
      requur++;
      if ( requur > 23 ) {
        requur = 0;
      }
    }
  */
  remainmin = minute(tmp_tijd) % 5;
  if ( (remainmin > 2) || (upTime == 0) ) {
    reqminute = minute(tmp_tijd) + 5 - remainmin;
    if ( reqminute == 60 ) {
      reqminute = 0;
      requur++;
      if ( requur > 23 ) {
        requur = 0;
      }
    }
  }
  else {
    reqminute = minute(tmp_tijd) - remainmin;
  }
  sprintf(temptxtbuff, "\r\n%s Collecting rain information for time: %02d:%02d ", br_tijd, requur, reqminute); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
  textlog(temptxtbuff, false);
  //  if (brlogging) {
  //    textStringLog("\r\n", false);
  //  }
  unsigned long startPluvioProc = millis(); // start time before connect
  if (brlogging) {
    sprintf(temptxtbuff, "-> connecting to %s... ", server); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  auto start = millis();
  WOLFSSL* ssl = NULL;
  if (buienradarClient.connect(server, 443)) {
    if (brlogging) {
      auto brcntime = millis() - start;
      sprintf(temptxtbuff, "Took: %lums", brcntime); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    int contLen = 0;
    if (brlogging) {
      sprintf(temptxtbuff, "\r\nConnected, start reading from: %s%s", server, deepurl); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    // check if there is no old object we can reuse
    if (ssl != NULL) {
      textStringLog("\r\nERROR: Old SSL object exists, try to cleanup", false);
      wolfSSL_shutdown(ssl);
      if (wolfssllog) {
        textStringLog("\r\ncleanup SSL object", false);
      }
      wolfSSL_free(ssl);
      if ((ssl == NULL) && brlogging) {
        textStringLog("\r\nSSL object cleared", false);
      }
      if (ssl != NULL) {
        textStringLog("\r\nERROR: SSL clear object forced", false);
        ssl = NULL;
      }
    }
    ssl = wolfSSL_new(ctx);
    if (ssl == NULL) {
      textStringLog("\r\nERROR: Unable to allocate SSL object ", false);
      todayErrCnt_br++;
      sprintf(pluvioErrTime_br, "No SSL Object @%02d:%02d:%02d", hour(), minute(), second());  // pluvioErrTime_br is max 32
      return;
    }
    else if (wolfssllog) {
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
      Serial.println(F("Memory usage after getting SSL object:"));
      freeRam();
#endif
#endif
      textStringLog("\r\nSSL object allocate success ", false);
    }
    err = wolfSSL_connect(ssl);
    if (err != WOLFSSL_SUCCESS) {
      err = wolfSSL_get_error(ssl, 0);
      wolfSSL_ERR_error_string_n(err, errBuf, 80);  // errBuf is max 88 char
      sprintf(temptxtbuff, "\r\nERROR: TLS Connect Error: %s ", errBuf); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      todayErrCnt_br++;
      totalErrCnt_br++;
      sprintf(pluvioErrTime_br, "TLS conn err @%02d:%02d:%02d", hour(), minute(), second());  // pluvioErrTime_br is max 32
    }
    else if (wolfssllog) {
      sprintf(temptxtbuff, "\r\nSSL version is %s, SSL cipher suite is %s ", wolfSSL_get_version(ssl), wolfSSL_get_cipher(ssl)); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    if ((wolfSSL_write(ssl, GPBmsg, GPBmsgSz)) == GPBmsgSz) {
      // wait for data
      tmo = 0;
      while (!buienradarClient.available() && (tmo < 500)) {
        tmo++;
        if (brlogging) {
          if (tmo == 1) {
            textStringLog("\r\nWaiting for response", false);
          }
          textStringLog(".", false);
        }
        delay(10);
      }
      if (tmo >= 500) {
        textStringLog("\r\nERROR: No response from server ", false);
      }
      arrpos = 0;
      restresp = "";
      tmo = 0;
      while ( buienradarClient.available() || wolfSSL_pending(ssl) ) {
        tmo++;
        input = wolfSSL_read(ssl, replybuff, sizeof(replybuff) - 1);  // replybuff is max 4096 char
        total_input += input;
        if (input > 4095) {
          textStringLog("\r\nERROR: SSL RX packetbuffer too small ", false);
          sprintf(pluvioErrTime_br, "RXpckt2large@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // pluvioErrTime_br is max 32
          break;
        }
        if (input < 0) {
          err = wolfSSL_get_error(ssl, 0);
          wolfSSL_ERR_error_string_n(err, errBuf, 80);  // errBuf is max 88 char
          sprintf(temptxtbuff, "\r\nERROR: TLS Read Error: %s", errBuf); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
          todayErrCnt_br++;
          totalErrCnt_br++;
          sprintf(pluvioErrTime_br, "TLS read err @%02d:%02d:%02d", hour(), minute(), second());  // pluvioErrTime_br is max 32
          break;
        }
        else if (input > 0) {
          if (brlogging) {
            sprintf(temptxtbuff, "\r\nLast pluvio received data size: %d ", input); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
          }
          pluvio_last_RXdata_size = input;
          /* remember largest received packet size */
          if (pluvio_max_RXdata_size <= pluvio_last_RXdata_size) {
            pluvio_max_RXdata_size = pluvio_last_RXdata_size;
            pluvio_max_RXdata_time = now();
          }
          /* remember smallest received packet size */
          if (pluvio_min_RXdata_size >= pluvio_last_RXdata_size) {
            pluvio_min_RXdata_size = pluvio_last_RXdata_size;
            pluvio_min_RXdata_time = now();
          }
          replybuff[input] = '\0'; //terminate the response, this is really necessary for reliable conversion to string
          String pluvresp = String(replybuff);
          pluvresp = restresp + pluvresp;
          startdata = pluvresp.lastIndexOf("\r\n\r\n"); // of twee x newline
          pluvio_last_HTTP_headersize = startdata + 4;
          /* remember largest received HTML header size */
          if (pluvio_max_HTML_headersize <= startdata + 4) {
            pluvio_max_HTML_headersize = startdata + 4;
            pluvio_max_HTTP_header_time = now();
          }
          /* remember smallest received HTML jheader size */
          if (pluvio_min_HTTP_headersize >= startdata + 4) {
            pluvio_min_HTTP_headersize = startdata + 4;
            pluvio_min_HTTP_header_time = now();
          }
          pluvresp.remove(0, startdata + 4);
          if (pluvresp.length() > 9) {
            int len2remove = 0;
            for (pl = arrpos; pl < 24; pl++) {
              Neerslag = pluvresp.toInt();
              sepa = pluvresp.indexOf('|');
              pluvresp.remove(0, sepa + 1);
              uur = pluvresp.toInt(); //read the hour
              sepa = pluvresp.indexOf(':');
              pluvresp.remove(0, sepa + 1);
              minuten = pluvresp.toInt(); //read the minutes from the first line
              if (pluvresp.length() < 4 ) {
                len2remove = pluvresp.length();
              }
              else {
                len2remove = 4;
              }
              pluvresp.remove(0, len2remove);
              // test if the values are valid?
              if (len2remove != 4) {
                sprintf(temptxtbuff, "\r\nDue to invalid response length, response string is shortened by %d", len2remove); // temptxtbuff is max 1024
                textlog(temptxtbuff, false);
              }
              if (showbuffer) {
                sprintf(temptxtbuff, "\r\nPluvio read for: %03d@%02d:%02d ", Neerslag, uur, minuten ); // temptxtbuff is max 1024
                textlog(temptxtbuff, false);
              }
              if ( (((uur == requur) && (minuten == reqminute)) || (read_valid == true)) && (isinf(Neerslag) != 1)  ) {
                read_valid = true;
                br_rslt = true;
                arrpluvio_br[arrpos] = Neerslag;
                arrhour_br[arrpos] = uur;
                arrminute_br[arrpos] = minuten;
                if (brlogging) {
                  sprintf(temptxtbuff, "\r\nvalue %d saved, for time: %02d:%02d at array position: %d", arrpluvio_br[arrpos], arrhour_br[arrpos], arrminute_br[arrpos], arrpos ); // temptxtbuff is max 1024
                  textlog(temptxtbuff, false);
                  if (wolfssllog) {
                    delay(3);
                  }
                  if (showbufferremaining) {
                    sprintf(temptxtbuff, "\r\nremaining bytes in response to process: %d", pluvresp.length() ); // temptxtbuff is max 1024
                    textlog(temptxtbuff, false);
                    if (wolfssllog) {
                      delay(3);
                    }
                  }
                }
                arrpos++;
                if ((pluvresp.length() < 9) && (pluvresp.length() != 0)) {
                  sprintf(temptxtbuff, "\r\nresidual length is: %d, too few characters in responsebuffer, waiting for next datapacket ", pluvresp.length() ); // temptxtbuff is max 1024
                  textlog(temptxtbuff, false);
                  restresp = pluvresp;
                  break; //wait for next packet
                }
                if (pluvresp.length() == 0) {
                  sprintf(c_tijd_Pluv, "%02d:%02d:%02d", hour(), minute(), second());  // c_tijd_Pluv is max 16 char
                  if (brlogging) {
                    sprintf(temptxtbuff, "\r\nAll data in buffer is processed @%s.", c_tijd_Pluv ); // temptxtbuff is max 1024
                    textlog(temptxtbuff, false);
                  }
                  todayCnt_br++;
                }
              }
            }
            if (pl >= 24) {
              if (brlogging) {
                sprintf(temptxtbuff, "\r\n%d positions processed, no more data needed.", pl ); // temptxtbuff is max 1024
                textlog(temptxtbuff, false);

              }
#ifdef DEBUG_WOLFSSL
              delay(20);
#endif
              break;
            }
          }
        }
        else if ((input == 0) && (arrpos == 0)) {
          textStringLog("\r\nERROR: content length in response is 0 ", false);
          sprintf(pluvioErrTime_br, "Contentlen=0@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // pluvioErrTime_br is max 32
          break;
        }
        if (tmo == 5) {
          textStringLog("\r\nERROR: While loop broken due to infinite loop (too much data in response. Unexpected, check reponse from buienradar.nl)", false);
          sprintf(pluvioErrTime_br, "infinWhile@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // pluvioErrTime_br is max 32
          break;
        }
      }
    }
    else {
      err = wolfSSL_get_error(ssl, 0);
      wolfSSL_ERR_error_string_n(err, errBuf, 80); // errBuf is max 88 char
      textStringLog("\r\nERROR: TLS Write Error: ", false);
      textlog(errBuf, false);
    }
    if (ssl) {
      wolfSSL_shutdown(ssl);
      if (wolfssllog) {
        textStringLog("\r\ncleanup TLS connection ", false);
      }
      wolfSSL_free(ssl);
      ssl = NULL;
      if (wolfssllog) {
        textStringLog("\r\nSSL object cleared", false);
      }
    }
  }
  else {
    textStringLog("\r\nERROR: No connection with gpsgadget.buienradar.nl\r\n", false);
    sprintf(pluvioErrTime_br, "NoConn@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // pluvioErrTime_br is max 32
    todayErrCnt_br++;
    totalErrCnt_br++;
  }
  if (ctx) {
    wolfSSL_CTX_free(ctx);
    if (wolfssllog) {
      textlog("\r\nwolfSSL ctx should be cleaned to 0 -> ", false);
    }
  }
  buienradarClient.stop();
  if (wolfssllog) {
    textlog("\r\n", false);
  }
  if (brlogging) {
    textStringLog("Connection ended -> ", false);
  }
  if (buienradarClient.connected()) {
    textStringLog("\r\nERROR GetPluvio_Br stop unexpected connection", false);
    buienradarClient.stop();
  }
  Neerslag = 0;  //reset value
  for (pl = 0; pl < 24 ; pl++) {
    if ( (arrhour_br[pl] == requur) && (arrminute_br[pl] == reqminute) ) {
      Neerslag = arrpluvio_br[pl];
      if ( pl > 0 ) {
        sprintf(temptxtbuff, "\r\nSucceeding value used from previous succesfull reading due to current errors from br-app for time: %02d:%02d", arrhour_br[pl], arrminute_br[pl]); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        sprintf(pluviosuccTime_br, "%d@%02d:%02d", pl, hour(tmp_tijd), minute(tmp_tijd));  // pluviosuccTime_br is max 16 char
        pluviosucc_br++;
      }
      //      if (brlogging) {
      //        sprintf(temptxtbuff, "Value found in array position: %d -> ", pl); // temptxtbuff is max 1024
      //        textlog(temptxtbuff, false);
      //      }
      break;
    }
  }
  if ((hrk == 4) && (pl == 24)) {
    sprintf(pluvioErrTime_br, "MaxFail@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // pluvioErrTime_br is max 32
    MaxFailErrCnt_br++;
  }
  if ( (Neerslag > 65) && br_rslt ) { // min level required to prevent inaccurate readings in webpage
    pluvio_br = (float)pow( (float)10.0, (float)((float)(Neerslag - 109.0) / 32.0) );
    GPBfpstring = String(pluvio_br, 2); //necessary to convert the float to char array, %f does not function
    pluvio_br *= BrCorrFact;
    totPluvio_br += (pluvio_br / (float)12.0); // the output counts to 1/12 of an hour we poll every 5 minutes
    if (brlogging || !min_serial) {
      //      delay(25);
      String GPBfpstring2 = String(pluvio_br, 2); //necessary to convert the float to char array, %f does not function
      GPBfpstring.toCharArray(GPBfpbuff, GPBfpstring.length() + 1);  // GPBfpbuff is max 16 char
      GPBfpstring2.toCharArray(GPBfpbuff2, GPBfpstring2.length() + 1);  // GPBfpbuff is max 16 char
      //      GPBfpbuff[GPBfpstring.length()] = '\0'; // toCharArray sets terminator
      sprintf(temptxtbuff, " -> Value %d calculating to regenMMPU: %s, adapted to: %s ", Neerslag, GPBfpbuff, GPBfpbuff2); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      if (todayCnt != 0) { // do not print at first run
        //        delay(25);
        GPBfpstring = String(totPluvio_br, 2); //necessary to convert the float to char array, %f does not function
        GPBfpstring.toCharArray(GPBfpbuff, GPBfpstring.length() + 1); // GPBfpbuff is max 16 char
        //        GPBfpbuff[GPBfpstring.length()] = '\0'; // toCharArray sets terminator
        sprintf(temptxtbuff, "-> Calculated total rain this day: %s ", GPBfpbuff); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
    last_neerslagintensiteit = pluvio_br;
    lastpluvioTime_br = now();
    if ((pluvio_br > max_neerslagintensiteit) || (day(maxpluvioTime_br) != day()) || (month(maxpluvioTime_br) != month()) ) {
      max_neerslagintensiteit = pluvio_br;
      maxpluvioTime_br = now();
    }
  }
  else {
    pluvio_br = 0.00;
    if (brlogging  || !min_serial) {
      //      GPBfpstring = to_string(pluvio_br);
      GPBfpstring = String(totPluvio_br, 2); //necessary to convert the float to char array, %f does not function 2 is decimal places
      GPBfpstring.toCharArray(GPBfpbuff, GPBfpstring.length() + 1); // GPBfpbuff is max 16 char
      //      GPBfpbuff[GPBfpstring.length()] = '\0'; // toCharArray sets terminator
      sprintf(temptxtbuff, "-> No new pluvio (0 MMPU). Calculated total rain this day: %s", GPBfpbuff); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  totexpectedPluvio_br = 0;
  //  Serial.print("Requested pluvio (var: pl) found @ position ");
  //  Serial.print(pl);
  //  Serial.print(" -> ");
  if (pl < 24) { //compute the expected pluvio for the next 2 hours
    int npluur = 0;
    int nplminute = 0;
    float expectedpluvio_br = 0.0;
    int nplNeerslag;
    int npltotNeerslag = 0;
    int nplfreq = 0;
    byte lastarrpos;
    if (arrpos > 1) {
      lastarrpos = arrpos - 1;
      //      sprintf(temptxtbuff, "\r\npluvio value in array position %d is from time %02d:%02d with pluvio value: %d", lastarrpos, arrhour_br[lastarrpos], arrminute_br[lastarrpos], arrpluvio_br[lastarrpos]); // temptxtbuff is max 1024
      //      textlog(temptxtbuff, true);
      //    String GPBfpstring1;
      //    char GPBfpbuff1[16];
      sprintf(lastarrtime, "%02d:%02d", arrhour_br[lastarrpos], arrminute_br[lastarrpos]);
      totexpectedPluvio_br = 0.0;
      for (int npl = lastarrpos; npl > pl ; npl--) {
        //      sprintf(temptxtbuff, "pluvio value on %02d:%02d is: %d", arrhour_br[npl], arrminute_br[npl], arrpluvio_br[npl]); // temptxtbuff is max 1024
        //      textlog(temptxtbuff, true);
        if (arrpluvio_br[npl] != 0) {
          nplNeerslag = arrpluvio_br[npl];
          npluur = arrhour_br[npl];
          nplminute = arrminute_br[npl];
          expectedpluvio_br = (float)pow( (float)10.0, ((float)((float)(nplNeerslag - 109.0) / 32.0)) );
          totexpectedPluvio_br += (expectedpluvio_br / (float)12.0); // the output counts to 1/12 of an hour we poll every 5 minutes
          //        GPBfpstring1 = String(expectedpluvio_br, 2);
          //        GPBfpstring1.toCharArray(GPBfpbuff1, GPBfpstring.length() + 1);
          //        GPBfpstring = String(totexpectedPluvio_br, 2);
          //        GPBfpstring.toCharArray(GPBfpbuff, GPBfpstring.length() + 1);
          //        sprintf(temptxtbuff, "\r\nCalculated rain for %02d:%02d is %s, accumulated for next periode %smm", npluur, nplminute, GPBfpbuff1, GPBfpbuff); // temptxtbuff is max 1024
          //        textlog(temptxtbuff, false);
        }
      }

      if ( (npluur == 0) && (nplminute == 0) ) {
        sprintf(nextpluvioTime_br, "> 2 hours");
      }
      else {
        sprintf(nextpluvioTime_br, "@ %02d:%02d", npluur, nplminute);
      }
      if (nplfreq > 0) {
        expectedpluvio_br = (float)pow( (float)10.0, (float)((float)((float)(npltotNeerslag / nplfreq) - 109.0) / 32.0) );
        totexpectedPluvio_br = nplfreq * (expectedpluvio_br / (float)12.0); // the output counts to 1/12 of an hour we poll every 5 minutes
      }
      if (brlogging || !min_serial) {
        GPBfpstring = String(totexpectedPluvio_br, 2);
        GPBfpstring.toCharArray(GPBfpbuff, GPBfpstring.length() + 1);
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Next expected pluvio %s, total expected until %s = %smm ", hour(), minute(), second(), nextpluvioTime_br, lastarrtime, GPBfpbuff);
        textlog(temptxtbuff, false);
      }
    }
    else {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: No valid pluvio found in array ", hour(), minute(), second());
      textlog(temptxtbuff, false);
    }
    //    if (brlogging) {
    //      textStringLog("Buienradar app expected pluvio ended -> ", false);
    //    }
  }


  unsigned long p_tijd = millis() - startPluvioProc;
  int min = p_tijd / 60000;
  p_tijd = p_tijd - ( min * 60000);
  int sec = p_tijd / 1000;
  p_tijd = p_tijd - ( sec * 1000);
  sprintf(pluvioProcTime_br, "%02d:%02d:%03d", min, sec, p_tijd);  // pluvioProcTime_br is max 16 char
  textStringLog("-> get pluvio result -> ", false);
  if (!br_rslt) {
    textStringLog("error", false);
    todayErrCnt_br++;
    totalErrCnt_br++;
  }
  else {
    textStringLog("ok", false);
  }
}
