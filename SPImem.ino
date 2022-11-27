#ifdef OPTION_FLASH
void printUniqueID(void) {
  long long _uniqueID = flash.getUniqueID();
  if (_uniqueID) {
    Serial.print("Unique ID: ");
    Serial.print(uint32_t(_uniqueID / 1000000L));
    Serial.print(uint32_t(_uniqueID % 1000000L));
    Serial.print(", ");
    Serial.print("0x");
    Serial.print(uint32_t(_uniqueID >> 32), HEX);
    Serial.print(uint32_t(_uniqueID), HEX);
  }
}

bool getIDSPImem() {
  Serial.println();
  Serial.print("SPIMemory Library version: ");
//#ifdef LIBVER
  uint8_t _ver, _subver, _bugfix;
  flash.libver(&_ver, &_subver, &_bugfix);
  Serial.print(_ver);
  Serial.print(".");
  Serial.print(_subver);
  Serial.print(".");
  Serial.println(_bugfix);
//#else
//  Serial.println(F("< 2.5.0"));
//#endif
  Serial.println();
  uint32_t JEDEC = flash.getJEDECID();
  if (!JEDEC) {
    Serial.println("No comms. Check wiring. Is chip supported? If unable to fix, raise an issue on SPIMemory Github");
    return false;
  }
  else {
    Serial.print("JEDEC ID: 0x");
    Serial.println(JEDEC, HEX);
    Serial.print("Man ID: 0x");
    Serial.println(uint8_t(JEDEC >> 16), HEX);
    Serial.print("Memory ID: 0x");
    Serial.println(uint8_t(JEDEC >> 8), HEX);
    Serial.print("Capacity: ");
    Serial.println(flash.getCapacity());
    Serial.print("Max Pages: ");
    Serial.println(flash.getMaxPage());
    printUniqueID();

  }
  return true;
}
#endif
