EthernetClient AbuseIPDBclient;

int AB_EthernetSend(WOLFSSL* ssl, char* msg, int sz, void* ctx) {
  int sent;
  if (wolfssllog) {
    Serial.print(F("\r\nEntering WolfSSL AB_EthernetSend, chars offered: "));
    Serial.print(sz);
    Serial.print(' ');
  }
  sent = AbuseIPDBclient.write((byte*)msg, sz);
  if (wolfssllog && abuslogging) {
    sprintf(mailcontent, "Number of bytes written to AbuseIPDBclient: %d", sz, sent);   //max is 80
    Serial.print(mailcontent);
  }
  if (sent == 0) {
    if (AbuseIPDBclient.connected()) {
      Serial.println();
      sprintf(mailbrstate, "ERROR: It seems that we have still connection to AbuseIPDB");    //max is 80
      Serial.print(mailbrstate);
    }
    else {
      Serial.println();
      sprintf(mailbrstate, "ERROR: It seems that we have lost connection to AbuseIPDB for some reasons");  //max is 80, this is 80 chars!!!
      Serial.print(mailbrstate);
    }
    if (!mailSendErrorflag && WolfSSLrwmail) {
      if (wolfssllog && abuslogging) {
        Serial.print(" -> Saving PVI vars and sending mail");
      }
      mailSendErrorflag = true;
      StoreDayTotal();
      if (!sendEmail(14)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail for WolfSSL send error.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
    errorwolfsslwantwrite++;
    return WOLFSSL_CBIO_ERR_WANT_WRITE;
  }
  if (wolfssllog && abuslogging) {
    Serial.print('<');
  }
  return sent;
}

int AB_EthernetReceive(WOLFSSL* ssl, char* reply, int sz, void* ctx) {
  int ret = 0;
  int tmt = 0;
  if (wolfssllog && abuslogging) {
    Serial.print(F("\r\nEntering WolfSSL EthernetReceive, chars requested: "));
    Serial.print(sz);
  }
  while ((tmt < 400) && (ret < sz)) {
    if (AbuseIPDBclient.available()) {
      reply[ret++] = AbuseIPDBclient.read();
    }
    else {
      if (wolfssllog && abuslogging) {
        Serial.print('.');
      }
      delay(5);
      tmt++;
    }
  }
  if ((ret > 0) && wolfssllog  && abuslogging) {
    Serial.print(" Number of bytes received from AbuseIPDB: ");
    Serial.print(ret);
  }
  if ((ret == 0) || (ret < sz)) {
    sprintf(mailbrstate, "\r\nERROR: Insufficient response, received: %d, expected bytes: %d", ret, sz);  //max is 80, this is 80 chars!!!
    Serial.println(mailbrstate);
    errorwolfsslwantread++;
    StoreDayTotal();
    if (!mailRcvErrorflag && WolfSSLrwmail) {
      if (wolfssllog && abuslogging) {
        Serial.print(" -> Saving PVI vars and sending mail");
      }
      mailRcvErrorflag = true;
      if (!sendEmail(15)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail Failed in WolfSSL receive function.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
    if (wolfssllog && abuslogging) {
      Serial.print(']');
    }
    return WOLFSSL_CBIO_ERR_WANT_READ;
  }
  if (wolfssllog && abuslogging) {
    Serial.print('>');
  }
  return ret;
}

bool Report2AbuseIPDB(int suspidx, int mode) {
  // mode 0 = suspicious, mode 1 = unknown command
  const static char server[] = "api.abuseipdb.com";
  char Abusedeepurl[1024];
  char errBuf[88];
  char IP2report[1024];
  char replybuff[4096];
  int aberr = 0;
  int input = 0;
  int ix;
  int startdata = 0;
  int tmo = 0;
  int total_input = 0;
  String abresp = "";
  String strIP2report = "";
  WOLFSSL_CTX* ctx = NULL;
  WOLFSSL_METHOD* method;
  WOLFSSL* ssl = NULL;

  ab_rslt = false;
#ifdef OPTION_NO_ABUSEIPDB
  textlog("WARNING: This version is compiled with definition OPTION_NO_ABUSEIPDB is true, so no output is uploaded to ABUSEIPDB", false);
  return false;
#endif
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Start Reporting to AbuseIPDB ", hour(tmp_tijd), minute(tmp_tijd), second(tmp_tijd));
  textlog(temptxtbuff, false);
  if (mode == 0) {
    sprintf(temptxtbuff, "%d: ", suspidx);
    String commentfld = String(msgEncode(temptxtbuff)) + String(msgEncode(suspicious_string[suspidx]));
    if ((commentfld.indexOf("ookie") != -1) || (commentfld.indexOf("ser") != -1) || (commentfld.indexOf("dmin") != -1)) {
      strIP2report = ",15,18,21";
    }
    input = commentfld.length();
    if (input > 255) {
      input = 255;
    }
    commentfld.toCharArray(Abusedeepurl, input + 1);
    input = 0;
    sprintf(IP2report, "comment=%s&ip=%d.%d.%d.%d&categories=14,23%s", Abusedeepurl, suspicious_ips[suspidx][0], suspicious_ips[suspidx][1], suspicious_ips[suspidx][2], suspicious_ips[suspidx][3], strIP2report.c_str());
    strIP2report = String(IP2report);
  }
  else if (mode == 1) {
    sprintf(temptxtbuff, "%d: ", suspidx);
    String commentfld = String(msgEncode(temptxtbuff)) + String(msgEncode(unknownCmd_string[suspidx]));
    if ((commentfld.indexOf("ookie") != -1) || (commentfld.indexOf("ser") != -1) || (commentfld.indexOf("dmin") != -1)) {
      strIP2report = ",18";
    }
    input = commentfld.length();
    if (input > 255) {
      input = 255;
    }
    commentfld.toCharArray(Abusedeepurl, input + 1);
    input = 0;
    sprintf(IP2report, "comment=%s&ip=%d.%d.%d.%d&categories=14,15,21,23%s", Abusedeepurl, unknownCmd_ips[suspidx][0], unknownCmd_ips[suspidx][1], unknownCmd_ips[suspidx][2], unknownCmd_ips[suspidx][3], strIP2report.c_str());
    strIP2report = String(IP2report);
  }
  sprintf(Abusedeepurl, "POST /api/v2/report HTTP/1.1\r\nHost: api.abuseipdb.com\r\nUser-Agent: curl/7.68.0\r\nAccept: application/json\r\nKey: %s\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n%s\r\n", ABUSEIPDB_KEY, strIP2report.length(), IP2report);
  if (abuslogging) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d We will try to POST to AbuseIPDB reporting:\r\n%s ", hour(tmp_tijd), minute(tmp_tijd), second(tmp_tijd), Abusedeepurl); // temptxtbuff is max 1024, time and time we searching for in de buienradar response
    textlog(temptxtbuff, false);
  }
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
  if (wolfssllog && abuslogging) {
    Serial.println(F("\r\nMemory usage at start AbuseIPDB:"));
    freeRam();
  }
#endif
#endif
  method = wolfTLSv1_2_client_method();
  if (method == NULL) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: AbuseIPDB; unable to get wolfTLSv1_2_client_method", hour(), minute(), second());
    textlog(temptxtbuff, false);
    return false;
  }
  else {
    if (wolfssllog && abuslogging) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d get wolfTLSv1_2_client_method: success -> ", hour(), minute(), second());
      textlog(temptxtbuff, false);
    }
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
    if (wolfssllog && abuslogging) {
      Serial.println(F("\r\nMemory after getting client method:"));
      freeRam();
    }
#endif
#endif
  }
  ctx = wolfSSL_CTX_new(method);
  if (ctx == NULL) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: unable to get wolfSSL ctx", hour(), minute(), second());
    textlog(temptxtbuff, false);
    return false;
  }
  else {
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
    if (wolfssllog && abuslogging) {
      Serial.println(F("\r\nMemory usage after loading ctx:"));
      freeRam();
    }
#endif
#endif
    if (wolfssllog && abuslogging) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d wolfSSL ctx get: success", hour(), minute(), second());
      textlog(temptxtbuff, false);
    }
  }
  //  String sniHostName = String(server);
  if (wolfSSL_CTX_UseSNI(ctx, WOLFSSL_SNI_HOST_NAME, server, (word16) XSTRLEN(server)) != WOLFSSL_SUCCESS) {
    wolfSSL_CTX_free(ctx); ctx = NULL;
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: UseSNI failed", hour(), minute(), second());
    textlog(temptxtbuff, false);
  }
  else if (wolfssllog  && abuslogging) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d UseSNI ON", hour(), minute(), second());
    textlog(temptxtbuff, false);
  }
  /* initialize wolfSSL using callback functions */
  wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);
  wolfSSL_SetIOSend(ctx, AB_EthernetSend);
  wolfSSL_SetIORecv(ctx, AB_EthernetReceive);
  unsigned long startAbuseProc = millis(); // start time before connect
  sprintf(temptxtbuff, "-> connecting to %s... ", server); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  auto start = millis();
  if (AbuseIPDBclient.connect(server, 443)) {
    if (abuslogging) {
      auto abcntime = millis() - start;
      sprintf(temptxtbuff, "%02d:%02d:%02d Took: %lums \r\nConnected, continue to setup TLS to: %s", hour(), minute(), second(), abcntime, server); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    // check if there is no old object we can reuse
    if (ssl != NULL) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB; Old SSL object exists, try to cleanup", hour(), minute(), second());
      textlog(temptxtbuff, false);
      wolfSSL_shutdown(ssl);
      if (wolfssllog && abuslogging) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d cleanup SSL object", hour(), minute(), second());
        textlog(temptxtbuff, false);
      }
      wolfSSL_free(ssl);
      if ((ssl == NULL) && wolfssllog && abuslogging) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SSL object cleared", hour(), minute(), second());
        textlog(temptxtbuff, false);
      }
      if (ssl != NULL) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB; SSL clear object forced", hour(), minute(), second());
        textlog(temptxtbuff, false);
        ssl = NULL;
      }
    }
    ssl = wolfSSL_new(ctx);
    if (ssl == NULL) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB; Unable to allocate SSL object ", hour(), minute(), second());
      textlog(temptxtbuff, false);
      todayErrCnt_ab++;
      sprintf(abErrTime_ab, "No SSL Object @%02d:%02d:%02d", hour(), minute(), second());  // pluvioErrTime_br is max 32
      return false;
    }
    else if (wolfssllog && abuslogging) {
#if not defined (WOLFSSL_TRACK_MEMORY) && not defined (WOLFSSL_DEBUG_MEMORY)
#if defined (DEBUG_WOLFSSL)
      Serial.println(F("Memory usage after getting SSL object:"));
      freeRam();
#endif
#endif
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Report2AbuseIPDB: SSL object allocate success ", hour(), minute(), second());
      textlog(temptxtbuff, false);
    }
    aberr = wolfSSL_connect(ssl);
    if (aberr == WOLFSSL_SUCCESS) {
      if (wolfssllog && abuslogging) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SSL version is %s, SSL cipher suite is %s ", hour(), minute(), second(), wolfSSL_get_version(ssl), wolfSSL_get_cipher(ssl)); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
      String strAbusedeepurl = String(Abusedeepurl);
      int strAbusedeepurlmsz = strAbusedeepurl.length();
      if ((wolfSSL_write(ssl, Abusedeepurl, strAbusedeepurlmsz)) == strAbusedeepurlmsz) {
        // wait for data
        tmo = 0;
        while (!AbuseIPDBclient.available() && (tmo < 500)) {
          tmo++;
          if (abuslogging) {
            if (tmo == 1) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Waiting for response from AbuseIPDB", hour(), minute(), second());
              textlog(temptxtbuff, false);
            }
            textStringLog(".", false);
          }
          delay(10);
        }
        if (tmo >= 500) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: No response from server ", hour(), minute(), second());
          textlog(temptxtbuff, false);
        }
        tmo = 0;
        total_input = 0;
        while ( AbuseIPDBclient.available() || wolfSSL_pending(ssl) ) {
          input = wolfSSL_read(ssl, replybuff, sizeof(replybuff) - 1);  // replybuff is max 4096 char
          total_input += input;
          if (abuslogging && showbufferremaining) {
            Serial.print("\r\nNumber of characters in replybuff: ");
            Serial.print(total_input);
          }
          if (input > 4095) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB; SSL RX packetbuffer too small ", hour(), minute(), second());
            textlog(temptxtbuff, false);
            sprintf(abErrTime_ab, "RXpckt2large@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // pluvioErrTime_br is max 32
            break;
          }
          if (input < 0) {
            aberr = wolfSSL_get_error(ssl, 0);
            wolfSSL_ERR_error_string_n(aberr, errBuf, 80);  // errBuf is max 88 char
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB; TLS Read Error: %s", hour(), minute(), second(), errBuf); // temptxtbuff is max 1024
            textlog(temptxtbuff, false);
            todayErrCnt_ab++;
            totalErrCnt_ab++;
            sprintf(abErrTime_ab, "TLS read err @%02d:%02d:%02d", hour(), minute(), second());  // pluvioErrTime_br is max 32
          }
          else if (input > 0) {
            //            if (abuslogging) {
            //              sprintf(temptxtbuff, "\r\nLast AbuseIPDB received data size: %d ", input); // temptxtbuff is max 1024
            //              textlog(temptxtbuff, false);
            //            }
            ab_rslt = true;
            ab_last_RXdata_size = input;
            replybuff[input] = '\0'; //terminate the response, this is really necessary for reliable conversion to string
            String restresp = String(replybuff);
            abresp += restresp;
          }
        }
      }
      else {
        aberr = wolfSSL_get_error(ssl, 0);
        wolfSSL_ERR_error_string_n(aberr, errBuf, 80); // errBuf is max 88 char
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB; TLS Write Error: ", hour(), minute(), second());
        textlog(temptxtbuff, false);
        textlog(errBuf, false);
      }
    }
    else {
      aberr = wolfSSL_get_error(ssl, 0);
      wolfSSL_ERR_error_string_n(aberr, errBuf, 80);  // errBuf is max 88 char
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB; TLS Connect Error: %s ", hour(), minute(), second(), errBuf); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      todayErrCnt_ab++;
      totalErrCnt_ab++;
      sprintf(abErrTime_ab, "TLS conn err @%02d:%02d:%02d", hour(), minute(), second());  // pluvioErrTime_br is max 32
    }

    if (ssl) {
      wolfSSL_shutdown(ssl);
      if (wolfssllog && abuslogging) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d cleanup TLS connection ", hour(), minute(), second());
        textlog(temptxtbuff, false);
      }
      wolfSSL_free(ssl);
      ssl = NULL;
      if (wolfssllog && abuslogging) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d SSL object cleared", hour(), minute(), second());
        textlog(temptxtbuff, false);
      }
    }
  }
  else {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02dERROR:  No connection with %s", hour(tmp_tijd), minute(tmp_tijd), second(tmp_tijd), server);
    textlog(temptxtbuff, false);
    sprintf(abErrTime_ab, "NoConn@%02d:%02d", hour(tmp_tijd), minute(tmp_tijd));  // abErrTime_ab is max 32
    todayErrCnt_ab++;
    totalErrCnt_ab++;
  }
  if (ctx) {
    wolfSSL_CTX_free(ctx);
    if (wolfssllog && abuslogging) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Report2AbuseIPDB: wolfSSL ctx should be cleaned to 0 ", hour(), minute(), second());
      textlog(temptxtbuff, false);
    }
  }
  AbuseIPDBclient.stop();
  //  if (wolfssllog && abuslogging) {
  //    textlog("\r\n", false);
  //  }
  textStringLog("-> Connection ended -> ", false);
  if (AbuseIPDBclient.connected()) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Report2AbuseIPDB after stop; unexpected connection!", hour(), minute(), second());
    textlog(temptxtbuff, false);
    AbuseIPDBclient.stop();
  }
  if (!ab_rslt) {
    textStringLog("error ", false);
    todayErrCnt_ab++;
    totalErrCnt_ab++;
  }
  else {
    textStringLog("ok ", false);
  }
  int startresponse = abresp.indexOf("HTTP/1.1 ") + 9;
  int endresponse = abresp.indexOf("\r\n");
  if (abuslogging && showbufferremaining) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Response from AbuseIPDB is:\r\n%s", hour(), minute(), second(), abresp.c_str());
    textlog(temptxtbuff, false);
    //    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d In response start HTTP status: %d, last character HTTP status: %d ", hour(), minute(), second(), startresponse, endresponse); // temptxtbuff is max 1024
    //    textlog(temptxtbuff, false);
  }
  for (ix = 0; startresponse < endresponse; ix++) {
    ab_lastresponse[ix] = abresp[startresponse];
    startresponse++;
  }
  ab_lastresponse[ix] = '\0';
  //  sprintf(temptxtbuff, "-> response: %s ", ab_lastresponse); // temptxtbuff is max 1024
  //  textlog(temptxtbuff, false);
  startdata = abresp.indexOf("\r\n\r\n"); // of twee x newline
  ab_last_HTML_headersize = startdata + 4;
  if (abuslogging && showbufferremaining) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Response headersize: %d, ab_lastresponse is: %s, position of OK is: %d", hour(), minute(), second(), ab_last_HTML_headersize, ab_lastresponse, abresp.indexOf("OK")); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  if (abresp.indexOf("OK") != 0) {
    startresponse = abresp.lastIndexOf(":") + 1;
    endresponse = abresp.lastIndexOf("}") - 1;
    if (abuslogging && showbufferremaining) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Response start abuseConfidenceScore: %d, last character position abuseConfidenceScore: %d, lengte van getal %d", hour(), minute(), second(), startresponse, endresponse, (endresponse - startresponse)); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
    if ((endresponse - startresponse) < 4) {
      char ab_lastperc[4];
      for (ix = 0; startresponse < endresponse; ix++) {
        ab_lastperc[ix] = abresp[startresponse];
        startresponse++;
      }
      if (mode == 0) {
        suspicious_perc[suspidx] = atoi(ab_lastperc);
        //      sprintf(temptxtbuff, "-> abuseConfidenceScore is: %d", suspicious_perc[suspidx]);
      }
      else if (mode == 1) {
        unknownCmd_perc[suspidx] = atoi(ab_lastperc);
        //      sprintf(temptxtbuff, "-> abuseConfidenceScore is: %d", unknownCmd_perc[suspidx]);
      }
      sprintf(temptxtbuff, "-> abuseConfidenceScore is: %d", atoi(ab_lastperc));
      textlog(temptxtbuff, false);
    }
  }
  return ab_rslt;
}

bool isIPequal(int idx1, int idx2) {
  if (abuslogging) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Comparing index: %d with index: %d -> ", hour(), minute(), second(), idx1, idx2);
    textlog(temptxtbuff, false);
  }
  if ((suspicious_ips[idx1][0] == suspicious_ips[idx2][0]) &&
      (suspicious_ips[idx1][1] == suspicious_ips[idx2][1]) &&
      (suspicious_ips[idx1][2] == suspicious_ips[idx2][2]) &&
      (suspicious_ips[idx1][3] == suspicious_ips[idx2][3])) {
    if (abuslogging) {
      textlog("equal ip address", false);
    }
    return true;
  }
  if (abuslogging) {
    textlog("different ip address", false);
  }
  return false;
}

void checkforIPabuse() {
  int nrofsuspip;
  int ix;
  int ix2rprt;
  // int ixmax;
  int list2report[SUSPARRAYDEPTH];
  int ixlist2report = 0;
  int pseudo_suspicious_index = 0;
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d CheckforIPAbuse... -> ", hour(), minute(), second());
  textlog(temptxtbuff, false);
  sprintf(temptxtbuff, "suspicious_index=%d, last_suspicious_index=%d -> ", suspicious_index, last_suspicious_index);
  textlog(temptxtbuff, false);
  if (suspicious_index != last_suspicious_index) {
#ifndef OPTION_REPORT_ALL_2_ABUSEIPDB
    /*
        below reports only unique IP adresses, not used anymore
    */
    if (suspicious_index < last_suspicious_index) {
      pseudo_suspicious_index = suspicious_index + SUSPARRAYDEPTH;
    }
    else {
      pseudo_suspicious_index = suspicious_index;
    }
    nrofsuspip = pseudo_suspicious_index - last_suspicious_index;
    if (nrofsuspip < 0 ) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: nrofsuspip is negative: %d -> exit", hour(), minute(), second(), nrofsuspip);
      textlog(temptxtbuff, false);
      return;
    }
    else {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Number of suspicious connections to report: %d", hour(), minute(), second(), nrofsuspip);
      textlog(temptxtbuff, false);
    }
    for (ix = 0; ix < SUSPARRAYDEPTH; ix++) {
      list2report[ix] = 0;
    }
    for (ix = 0; ix < SUSPARRAYDEPTH; ix++) {
      if (ix != 0) {
        // this is not the first entry
        if (isIPequal(last_suspicious_index + ix , last_suspicious_index + ix - 1)) {
          if (suspicious_length[last_suspicious_index + ix] > suspicious_length[last_suspicious_index + ix - 1]) {
            if (abuslogging) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d nonzero same ix2rprt longer changed to: %d, stored in index on pos: %d, length is: %d", hour(), minute(), second(), last_suspicious_index + ix, ixlist2report - 1, suspicious_length[last_suspicious_index + ix]);
              textlog(temptxtbuff, false);
            }
            list2report[ixlist2report - 1] = last_suspicious_index + ix;
          }
          else {
            if (abuslogging) {
              sprintf(temptxtbuff, "\r\n%02d:%02d:%02d nonzero same ix2rprt not longer, no change for line %d", hour(), minute(), second(), last_suspicious_index + ix);
              textlog(temptxtbuff, false);
            }
          }
        }
        else {
          // not the same as former entry, store to report
          list2report[ixlist2report] = last_suspicious_index + ix;
          if (abuslogging) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d nonzero not same suspstlinenumber: %d, stored in pos: %d, string length: %d, items in index: %d",
                    hour(), minute(), second(), list2report[ixlist2report], ixlist2report, suspicious_length[list2report[ixlist2report]], ixlist2report + 1);
            textlog(temptxtbuff, false);
          }
          ixlist2report++;
        }
      }
      else {
        list2report[ixlist2report] = last_suspicious_index;
        if (abuslogging) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d zero suspstlinenumber: %d, stored in pos: %d, string length: %d, items in index: %d",
                  hour(), minute(), second() , list2report[ixlist2report], ixlist2report, suspicious_length[list2report[ixlist2report]], ixlist2report + 1);
          textlog(temptxtbuff, false);
        }
        ixlist2report++;
      }
      if ((last_suspicious_index + ix + 1 < SUSPARRAYDEPTH) && isIPequal(last_suspicious_index + ix , last_suspicious_index + ix + 1)) {
        if (suspicious_length[last_suspicious_index + ix] < suspicious_length[last_suspicious_index + ix + 1]) {
          if (abuslogging) {
            sprintf(temptxtbuff, "\r\n%02d:%02d:%02d IPeq lenshort line<%d ix2rprt changed to: %d, stored in index on pos: %d, length is: %d", hour(), minute(), second(), SUSPARRAYDEPTH, last_suspicious_index + ix, ixlist2report, suspicious_length[last_suspicious_index + ix]);
            textlog(temptxtbuff, false);
          }
          list2report[ixlist2report - 1] = last_suspicious_index + ix + 1;
        }
      }
    }
    if (ixlist2report > 0) {
      for (ix = 0; ix < ixlist2report; ix++) {
        if (abuslogging) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Index %d to report: %d, length of comment to report: %d ", hour(), minute(), second(), ix, list2report[ix], suspicious_length[list2report[ix]]);
          textlog(temptxtbuff, false);
        }
        textlog("reporting to AbuseIPDB.", false);
        if (Report2AbuseIPDB(list2report[ix], 0)) {
          rprt2abuseipdb++;
        }
        else {
          rprt2abuseipdberr++;
        }
      }
      last_suspicious_index = suspicious_index; //suspicious index is always incremented after adding last entry. array is 0 to 24 so it is pointing to the last new entry
    }
#else
    /*
       Reports all suspicious connection one at a time every 20 minutes
    */
    if (abuslogging) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Index to report in one at a time OPTION_REPORT_ALL_2_ABUSEIPDB: %d from %d breaches.", hour(), minute(), second(), last_suspicious_index, suspicious_index);
      textlog(temptxtbuff, false);
    }
    if (Report2AbuseIPDB(last_suspicious_index, 0)) {
      rprt2abuseipdb++;
      if (last_suspicious_index != suspicious_index) {
        last_suspicious_index++;
        if (last_suspicious_index == SUSPARRAYDEPTH) {
          last_suspicious_index = 0;
        }
      }
      if (last_suspicious_index != suspicious_index) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d AbuseIPDB suspicious_index=%d, last_suspicious_index updated=%d", hour(), minute(), second(), suspicious_index, last_suspicious_index);
        textlog(temptxtbuff, false);
      }
      else {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d All intrusion attempts have been notified", hour(), minute(), second());
        textlog(temptxtbuff, false);
      }
    }
#endif
  }
  else if (unknownCmd_sema) {
    textlog("checking for queued unknown commands -> ", false);
    int ix;
    for (ix = 0; ix < UNKWNCMDARRAYDEPTH; ix++) {
      if (unknownCmd_queued[ix] == 1) { // report the first in the array
        break;
      }
    }
    if (ix == UNKWNCMDARRAYDEPTH) {
      unknownCmd_sema = false;
      sprintf(temptxtbuff, "All unknown commands have been notified -> ");
      textlog(temptxtbuff, false);
    }
    else  {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d unknownCmd_index=%d, with status queued reported to AbuseIPDB", hour(), minute(), second(), ix);
      textlog(temptxtbuff, false);
      if (Report2AbuseIPDB(ix, 1)) {
        unknownCmd_queued[ix] = 2;
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d unknownCmd_index=%d, successful reported to AbuseIPDB, status changed to already done", hour(), minute(), second(), ix);
        textlog(temptxtbuff, false);
        rprt2abuseipdb++;
      }
    }
  }
  else {
    textlog("nothing to report.", false);
  }
}
