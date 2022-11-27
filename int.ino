void showEthernetConfig() {
  char valbuff[16];
  Serial.print(F("Using MAC: "));
  Ethernet.MACAddress(macBuffer); // fill the buffer
  sprintf(webData, "%02X-%02X-%02X-%02X-%02X-%02X", macBuffer[0], macBuffer[1], macBuffer[2], macBuffer[3], macBuffer[4], macBuffer[5]);  //webdata is max 96
  Serial.println(webData);
  textStringLog("Current IP addres: ", false);
  IPAddress segm = Ethernet.localIP();
  String scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(valbuff, scobuff.c_str());
  textlog(valbuff, true);
  textStringLog("Subnetmask: ", false);
  segm = Ethernet.subnetMask();
  scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(valbuff, scobuff.c_str());
  textlog(valbuff, true);
  textStringLog("Default gateway: ", false);
  segm = Ethernet.gatewayIP();
  scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(valbuff, scobuff.c_str());
  textlog(valbuff, true);
  textStringLog("DNS-server: ", false);
  segm = Ethernet.dnsServerIP();
  scobuff = String() + segm[0] + "." + segm[1] + "." + segm[2] + "." + segm[3];
  strcpy(valbuff, scobuff.c_str());
  textlog(valbuff, true);
}

void switchEthNetw() {
  //    devider++;
  //    if (devider == 2) return;
  if (lanmode == 0) {
    Ethernet.begin(test_LAN_mac, test_LAN_ip, test_LAN_dnsserver, test_LAN_gateway, subnet);
    textStringLog("Ethernet using LAN test configuration", true);
    lanmode = 1;
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    //     devider = 1;
  }
  else {
    Ethernet.begin(LAN_mac, LAN_ip, LAN_dnsserver, LAN_gateway, subnet);
    textStringLog("Ethernet using LAN configuration", true);
    lanmode = 0;
    for (byte ipsel = 0; ipsel < 8; ipsel++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(150);
      digitalWrite(LED_BUILTIN, LOW);
      delay(150);
    }
    //      devider = 1;
  }
  showEthernetConfig();
  digitalWrite(LED_BUILTIN, HIGH);
}
