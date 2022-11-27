/*
  ----------------------------
  A r d a S o l   Project
  ----------------------------

  Version: 5.0
  Version Date: 10.12.2013

  Creation Date: 10.5.2013
  Author: Heinz Pieren
  Email: heinz.pieren@semprevacanze.it

  Aurora PVI communication
  ------------------------
  Checks via RS485 Interface the availability of
  a Power one Aurora Photovoltaic Inverter PVI-3.0-TL-OUTD
  The RS485 Interface is driven by SoftwareSerial Routines

  pin 2 = Rx,  Pin 3 = TX
  pin 5 = RTS, for HDX operation, High = Transmit, Low = Receive

  PVI parameters:
  Baudrate 19200, 8 Data, no parity, 1 stop
  RS485 PVI-ID address = 2 (default)

  Command Packet structure is a 10 Byte command
  1	address
  2	operation type
  3	data1
  4	data2
  5	data3
  6	data4
  7	data5
  8	data6
  9	crc low
  10	crc high

  Example: Get total amount of produced energy
  02 4E 05 00 00 00 00 00 BC DD
 			----- crc = 0xDDBC
  -- Accumulated data type 5 = total
  -- Get accumulated data command = 78 (0x4E)
  -- PVI address = 2

  PVI response 8 byte data packet:
  1 State
  2 MState
  3 Param1
  4 Param2
  5 Param3
  6 Param4
  7 crc low
  8 crc high

  Example: Response to above request
  00 06 00 00 36 34 5A 62
 		  ----- crc = 0x625A
  ----------- power in kwh (32Bit) = 0x3634 = 13876 Wh
  -- MState = 6 ?
  -- State = 0 ?
*/

// Aurora PVI commands definitions

#define pviadr            2

#define cmdGridVoltage    0x3B01		//grid voltage
//#define cmdGridVoltage    0x3B20		//average grid voltage

#define cmdGridPower      0x3B03
#define cmdTempInv        0x3B15
#define cmdTempBst        0x3B16
#define cmdDc1Voltage     0x3B17
#define cmdDc1Current     0x3B19
#define cmdDc2Voltage     0x3B1A
#define cmdDc2Current     0x3B1B
#define cmdRiso           0x3B1E
#define cmdGridPowerPeak  0x3B23
#define cmdTime           0x4600
#define cmdsetTime        0x4700
#define cmdEnergyDay      0x4E00
#define cmdEnergyTotal    0x4E05
//#define logMessageFlushPVIRecBuf     "*E4:FlushPVIRB x="
//#define logMessagePVIRecChkSumErr    "*E5:PVIChkSumErr"
//#define logMessagePVIRecTimeout      "*E6:PVIRecTimeout"


//#define cmdResetEnergyValue 0x0052	//reset energy total command for ArdaSol Energy Monitor


#define cmdSize 10    // 10 Bytes command Packet
static byte CmdBuf[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // 10 elements
#define rspSize 8    // 8 Bytes response Packet
static byte RspBuf[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // 8 elements

// Aurora Data definitions

unsigned int            invDateAndTime = 0; //absolute format of date and time in seconds from aurora

//unsigned long            GridVoltage = 0; //grid tension in Volts

//unsigned long            GridPower = 0; //power fed to grid in W
//unsigned long            TempBst = 0; //Inverter operating temperature
//unsigned long            TempInv = 0; //Inverter operating temperature
//unsigned long            InvRiso = 0; //Isolation resistance

// unsigned long            GridPowerPeakDay = 0; //peak value of power fed to grid in W

// unsigned long            EnergyDay = 0; // energy produced in day in Wh
//unsigned long            EnergyTotal = 0; // total energy produced in inverters lifetime in Wh
//unsigned long            EnergyTotalkWh = 0;
//unsigned long            EnergyAverageDay = 0; // average a day calculated from start of operation of the PVI

bool 			resetEnergyDayUsed = false;


unsigned int          receiveTimeoutStart;
bool                  prevTimeout;
unsigned short        crc16Checksum;
bool                  respTimeout;

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------


/* CRC16 checksum calculation

   Copyright (C) 2006-2012 Curtis J. Blank curt@curtronics.com
  16   12   5
  this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
  This is 0x1021 when x is 2, but the way the algorithm works
  we use 0x8408 (the reverse of the bit pattern).  The high
  bit is always assumed to be set, thus we only use 16 bits to
  represent the 17 bit value. */

#define POLY 0x8408   /* 1021H bit reversed */

unsigned short uiCrc16Cal (byte * buf, byte length)
{
  byte i;
  unsigned short data;
  unsigned short crc = 0xffff;

  if (length == 0)
    return (~crc);
  do
  {
    for (i = 0, data = (unsigned short)0xff & *buf++;
         i < 8;
         i++, data >>= 1)
    {
      if ((crc & 0x0001) ^ (data & 0x0001))
        crc = (crc >> 1) ^ POLY;
      else  crc >>= 1;
    }
  }
  while (--length);
  crc = ~crc;

  return (crc);
}

//-----------------------------------------------------------------------
// Sends the command get current power feeding to grid

void getGridVoltage()
{
  int i;
  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdGridVoltage);
  CmdBuf[2] = lowByte(cmdGridVoltage);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);
  char gGVvalbuff[16];

  prevTension = actTension;

  // retry max 2 times
  for (i = 0; i < 2; i++) {
    SendCommandPacketToPVI();
    if (ResponsePacketReceived("getGridVoltage")) {
      actTension = getReceivedValueAsFloat();
      //   ------------------------------ am ------------------------------------------------------------------------
      if  ((lastSecond == 55) && (actTension > 0) && (rxTimeflag == false) && (receiveMaxRS485Wait == shortRS485Wait) && (hour() < 12) ) {
        dtostrf( actTension, 3, 2, gGVvalbuff ); // gGVvalbuff max 16 char
        sprintf(temptxtbuff, "\r\n%s Probably inverter switching on moment - Gridtension: %sVolt", DateTime(now()), gGVvalbuff); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        if (!min_serial || (upTime == 0)) {
          textlog(" -> ", false);
        }
      }
      prevTimeout = false;
      return;
    }
    // wait if we have an error reading during daylight
    if ( receiveMaxRS485Wait == longRS485Wait) {
      delay(receiveMaxRS485Wait);
    }
  }
  // if we get no reponse  after two times, then suppose the PVI has stopped for today
  if ( respTimeout && (i == 2) ) {
    actTension = 0.0;
    if ( (receiveMaxRS485Wait == longRS485Wait) && !prevTimeout && (hour() > 15) ) {
      sprintf(temptxtbuff, "\r\n%s Timeout from PVI reading gridvoltage, probably inverter switching off moment", DateTime(now()));
      textlog(temptxtbuff, false);
      if (!min_serial || (upTime == 0)) {
        textlog(" -> ", false);
      }
      prevTimeout = true; //show only once
    }
  }
  else {
    dtostrf( actTension, 3, 2, gGVvalbuff ); // gGVvalbuff max 16 char
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Error reading PVI gridvoltage, actTension=%sV", hour(), minute(), second(), gGVvalbuff); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    textStringLog(" respTimeout=", false);
    respTimeout ? textStringLog("true", false) : textStringLog("false", false);
    textStringLog(" i=", false);
    itoa(i, gGVvalbuff, 10); // gGVvalbuff max 16 char
    textlog(gGVvalbuff, false);
    if (!min_serial || (upTime == 0)) {
      textlog(" -> ", false);
    }
  }
}
//-----------------------------------------------------------------------
// Sends the command get the grid power peak current day

void getGridPowerPeakDay()
{
  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdGridPowerPeak);
  CmdBuf[2] = lowByte(cmdGridPowerPeak);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();
  if (ResponsePacketReceived("getGridPowerPeakDay"))
  {
    float GPPD;
    GPPD = getReceivedValueAsFloat();
    if ((short(GPPD) >= 0) && (short(GPPD) < 4200))
    {
      pwrPeak = int(GPPD);
      if (short((GPPD - pwrPeak) * 10) >= 5) ++pwrPeak;
    }
  }
}

//-----------------------------------------------------------------------
// Sends the command get current power feeding to grid

void getGridPower()
{

  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdGridPower);
  CmdBuf[2] = lowByte(cmdGridPower);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();
  GridPower = 0;
  if (ResponsePacketReceived("getGridPower"))
  {
    float grdPpvi;
    grdPpvi = getReceivedValueAsFloat();
    unsigned short tempPpvi = (unsigned short)(grdPpvi);
    if ((tempPpvi >= 0) && (tempPpvi < 4200))
      // && (tempPpvi <= pwrPeak) )
    {
      GridPower = (unsigned short)(grdPpvi);
      if (int((tempPpvi - GridPower) * 10) >= 5) ++GridPower;  //round up
      //      lastGridPower=GridPower;
      //      sensors[1]->Actual = GridPower;
    }
  }
}

//-----------------------------------------------------------------------
// Sends the command get current booster temperature

void getTempBst()
{

  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdTempBst);
  CmdBuf[2] = lowByte(cmdTempBst);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();
  //  float BstTemp = 0.0;
  if (ResponsePacketReceived("getTempBst"))
  {
    actBstTemp = getReceivedValueAsFloat();
  }
}

//-----------------------------------------------------------------------
// Sends the command get actual inverter temperature

void getTempInv()
{

  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdTempInv);
  CmdBuf[2] = lowByte(cmdTempInv);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();
  if (ResponsePacketReceived("getTempInv"))
  {
    float InvTemp = getReceivedValueAsFloat();
    float tempTemp = InvTemp - actInvTemp;
    tempTemp = abs(tempTemp);
    if ( ((InvTemp < 80.0) && (tempTemp < 7.0)) || ((InvTemp > 5) && (InvTemp < 56)) )
    {
      actInvTemp = InvTemp;
    }
  }
  // check if we have a temperature within the expected wide range otherwaise we have a mis read. return old value
  // the max deviation is 30 degrees
  //  float tempHelper = InvTemp - actInvTemp;

}

//-----------------------------------------------------------------------
// Sends the command get actual DC voltage string 1

void getDc1Voltage()
{

  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdDc1Voltage);
  CmdBuf[2] = lowByte(cmdDc1Voltage);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();
  if (ResponsePacketReceived("getDc1Voltage"))
  {
    actDc1Voltage = getReceivedValueAsFloat();
  }
}

//-----------------------------------------------------------------------
// Sends the command get actual DC voltage string 1

void getDc1Current()
{

  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdDc1Current);
  CmdBuf[2] = lowByte(cmdDc1Current);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  if ( actTension == 0.0 ) {
    actDc1Current = 0.0;
  }
  SendCommandPacketToPVI();
  if (ResponsePacketReceived("getDc1Current"))
  {
    actDc1Current = getReceivedValueAsFloat();
  }
}


//-----------------------------------------------------------------------
// Sends the command get actual DC voltage string 1

void getDc2Voltage()
{

  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdDc2Voltage);
  CmdBuf[2] = lowByte(cmdDc2Voltage);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();

  if (ResponsePacketReceived("getDc2Voltage"))
  {
    actDc2Voltage = getReceivedValueAsFloat();
  }
}

//-----------------------------------------------------------------------
// Sends the command get actual DC voltage string 1

void getDc2Current()
{

  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdDc2Current);
  CmdBuf[2] = lowByte(cmdDc2Current);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();

  if (ResponsePacketReceived("getDc2Current"))
  {
    actDc2Current = getReceivedValueAsFloat();
  }
}


//-----------------------------------------------------------------------
// Sends the command get the measuresed value of Riso at wake-up

void getRiso()
{
  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdRiso);
  CmdBuf[2] = lowByte(cmdRiso);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);
  SendCommandPacketToPVI();
  if (ResponsePacketReceived("getRiso"))
  {
    Riso = getReceivedValueAsFloat();
  }
}

//-----------------------------------------------------------------------
// Sends the command get current date and time

void getTime()
{
  unsigned int at; // current time
  char gTvalbuff[16];
  CmdBuf[0] = pviadr;  //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdTime);
  CmdBuf[2] = lowByte(cmdTime);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();

  if (ResponsePacketReceived("getTime")) {
    invDateAndTime = getReceivedValueAsInt();
    at = invDateAndTime + 946684800;
    timeDiff = at - now();
    timeDiff = abs(timeDiff);
    if (timeDiff > 59)
    {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Time difference between Inverter and real time too large (>59 sec), but no action taken at this moment.", hour(), minute(), second());
      textlog(temptxtbuff, false);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Inverter time: %s, real time: %s", hour(), minute(), second(), DateTime(at), DateTime(now()));
      textlog(temptxtbuff, false);
      if (!sendEmail(4)) {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail Time difference between Inverter and real time too large (>59 sec), but no action taken at this moment.", hour(), minute(), second()); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
      }
    }
    else {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Inverter time difference within 1 minute: %d seconds.", hour(), minute(), second(), timeDiff);
      textlog(temptxtbuff, false);
    }
    SerInpRcdv = true; // set semaphore to start a new line in logging
  }
}

//-----------------------------------------------------------------------
// Sends the command get current date and time

void setinvTime()
{
  char siTvalbuff[16];
  CmdBuf[0] = pviadr;  //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdTime);
  CmdBuf[2] = lowByte(cmdTime);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  SendCommandPacketToPVI();

  if (ResponsePacketReceived("setinvTime")) {
    invDateAndTime = getReceivedValueAsInt();
    currentInvtime = invDateAndTime + 946684800;
    timeDiff = currentInvtime - now();
    timeDiff = abs(timeDiff);
    if (timeDiff > 59) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Time difference between Inverter and real time to large (>59 sec)", hour(), minute(), second() );
      textlog(temptxtbuff, false);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Inverter time: %s, real time: ", hour(), minute(), second(), DateTime(currentInvtime), DateTime(now()) );
      textlog(temptxtbuff, false);
      /* adjust time to Aurora's time base if skew >= 1 minute */
      invDateAndTime = now() - 946684800;
      /* adjust by 1 due to latency in setting the time */
      invDateAndTime++;
      CmdBuf[0] = pviadr;  //RS485 chain address of PVI
      CmdBuf[1] = highByte(cmdsetTime);
      CmdBuf[2] = (invDateAndTime >> 24) & 0xff;
      CmdBuf[3] = (invDateAndTime >> 16) & 0xff;
      CmdBuf[4] = (invDateAndTime >> 8) & 0xff;
      CmdBuf[5] = (invDateAndTime) & 0xff;
      CmdBuf[6] = 0;
      CmdBuf[7] = 0;
      crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
      CmdBuf[8] = lowByte(crc16Checksum);
      CmdBuf[9] = highByte(crc16Checksum);

      SendCommandPacketToPVI();
      // We have set the new time
      if (ResponsePacketReceived("setinvTime#1")) {
        invDateAndTime += 946684800;
        if (!sendEmail(5)) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail Time difference between Inverter and real time too large (>59 sec)", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }

      }
      else {
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Failed to set new inverter time", hour(), minute(), second());
        textlog(temptxtbuff, false);
        if (!sendEmail(7)) {
          sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail failed to set new inverter time", hour(), minute(), second()); // temptxtbuff is max 1024
          textlog(temptxtbuff, false);
        }
      }
    }
    else {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Inverter time difference within 1 minute: %d second(s). No action taken.", hour(), minute(), second(), timeDiff);
      textlog(temptxtbuff, false);
    }
  }
  else {
    textStringLog("Failed to read initial inverter time in setTime", true);
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Failed to read initial inverter time in setTime.", hour(), minute(), second());
    textlog(temptxtbuff, false);
    if (!sendEmail(8)) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Sending mail Failed to read initial inverter time in setTime.", hour(), minute(), second()); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  SerInpRcdv = true; // set semaphore to start a new line in logging
}

//-----------------------------------------------------------------------
// Sends the command get the cumulated energy current day

void getEnergyDay()
{
  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdEnergyDay);
  CmdBuf[2] = lowByte(cmdEnergyDay);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);

  //  unsigned short EnergyDay = 0;

  SendCommandPacketToPVI();

  if (ResponsePacketReceived("getEnergyDay"))
  {
    totalToday = getReceivedValueAsInt();
    //    if (Epvi < 30000) EnergyDay=Epvi;		//validate <30kWh in a day
  }
}

//-----------------------------------------------------------------------
// Sends the command get the total cumulated

void getEnergyTotal()
{
  CmdBuf[0] = pviadr;      //RS485 chain address of PVI
  CmdBuf[1] = highByte(cmdEnergyTotal);
  CmdBuf[2] = lowByte(cmdEnergyTotal);

  CmdBuf[6] = 0;
  CmdBuf[7] = 0;

  crc16Checksum = uiCrc16Cal (CmdBuf, 8) ;
  CmdBuf[8] = lowByte(crc16Checksum);
  CmdBuf[9] = highByte(crc16Checksum);


  SendCommandPacketToPVI();

  if (ResponsePacketReceived("getEnergyTotal"))
  {
    unsigned int EnergyTotal = getReceivedValueAsInt();
    EnergyTotal = EnergyTotal % 100000000;   // due to problem with PVI RAM failed
    cmltvPwr = EnergyTotal / 1000;
    if ((EnergyTotal % 1000) > 499)   ++cmltvPwr;  // round up
  }
}

//-----------------------------------------------------------------------

unsigned int getReceivedValueAsInt()
{
  unsigned int value;

  value = RspBuf[2];
  value = value << 8;
  value = value | RspBuf[3];
  value = value << 8;
  value = value | RspBuf[4];
  value = value << 8;
  value = value | RspBuf[5];

  return (value);
}
//-----------------------------------------------------------------------

float getReceivedValueAsFloat()
{
  byte cValue[4];
  float *value;
  float retvalue;

  cValue[0] = RspBuf[5];
  cValue[1] = RspBuf[4];
  cValue[2] = RspBuf[3];
  cValue[3] = RspBuf[2];
  value = (float *)cValue;
  retvalue = *value;
  /*
    Serial.print(F("in getReceivedValueAsFloat: "));
    Serial.print(*value);
    Serial.print(F(" converted for return: "));
    Serial.println(*value);
    delay(1);
  */
  return (retvalue);
}

//-----------------------------------------------------------------------
void clearRS485SerialInput()
{
  //  Serial2.setTimeout(receiveMaxRS485Wait);
  int charcnt = 0;
  while (Serial2.available() > 0)
  {
    Serial2.read();
    charcnt++;

  }
  if ((charcnt != 0) && (invlogging == true)) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d %d character(s) in read buffer from PVI cleared", hour(), minute(), second(), charcnt); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
    if (!min_serial || (upTime == 0)) {
      textlog(" -> ", false);
    }
    SerInpRcdv = true; // set semaphore to start a new line in logging
  }
}

//-----------------------------------------------------------------------
// sends a valid PVI Command to RS485
void SendCommandPacketToPVI()
{

  byte i;
  Serial2.flush();  // clear transmitbuffer
  for (int j = 0; j < rspSize; j++) {
    RspBuf[j] = 0;  // clear the receive buffer
  }
  clearRS485SerialInput();
  /*  Serial.println("\r\nSending to PVI: ");
     for (i = 0; i < cmdSize ; ++i )
    {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(CmdBuf[i], HEX);
    }
  */
  Serial2.write(CmdBuf, cmdSize);
  receiveTimeoutStart = millis();
}

//-----------------------------------------------------------------------
// waits for a valid PVI Response from RS485 (Aurora)
bool ResponsePacketReceived(const char* proc)
{
  bool respOk = false;
  unsigned short respCRC;
  unsigned short nrcr;
  unsigned short rxComplete;
  unsigned short rxTime;
  respTimeout = false;
  char RPRvalbuff[16];
  nrcr = Serial2.readBytes(RspBuf, rspSize);
  rxComplete = millis();
  if ( rspSize != nrcr ) {
    if (receiveMaxRS485Wait == longRS485Wait) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: number of bytes received should be 8: %d for function %s -> No response from PVI", hour(), minute(), second(), nrcr, proc);
      textlog(temptxtbuff, false);
      SerInpRcdv = true; // set semaphore to start a new line in logging
      if (!min_serial || (upTime == 0)) {
        textlog(" -> ", false);
      }
    }
    // prevent bogo readings
    for (int j = 0; j < rspSize; j++) {
      RspBuf[j] = 0;  // clear the receive buffer
    }

    respTimeout = true;
    if (receiveMaxRS485Wait == longRS485Wait) { // only count if the pvi is awake
      cntrespTimeout++;
    }
    return (false);
  }

  rxTime = rxComplete - receiveTimeoutStart;
  if (rxTime < minRxTime)
    minRxTime = rxTime;
  if (rxTime > maxRxTime)
    maxRxTime = rxTime;

  respCRC = RspBuf[6] + (RspBuf[7] << 8);
  if (respCRC == uiCrc16Cal(RspBuf, 6)) {
    respOk = true ;
  }
  else {
    ChkSumErrCnt++;
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: CRC in response from PVI: 0x%04x, calculated receive CRC: 0x%04x", hour(), minute(), second(), respCRC, uiCrc16Cal(RspBuf, 6));
    textlog(temptxtbuff, false);
    SerInpRcdv = true; // set semaphore to start a new line in logging
    if (!min_serial || (upTime == 0)) {
      textlog(" -> ", false);
    }
    return (false);
  }

  return (respOk);
}
