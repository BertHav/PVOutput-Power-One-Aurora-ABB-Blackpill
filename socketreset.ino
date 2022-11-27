#include "userdefs.h"

bool CheckSecondsDelay(unsigned long tijd1, unsigned long tijd2) {
  if ( (millis() - tijd1) > tijd2 )
    return (true);
  else
    return (false);
}

int CheckSocketConnections() {
  // Check the current status of the sockets
  // check first if we have FIN_Wait sockets
  CloseSocket18Connection();
  int nrfreesockets = 0;
  for (uint8_t l_sock = 0; l_sock < MAX_SOCK_NUM; l_sock++) {
    uint8_t l_status = Ethernet.socketStatus(l_sock); //status
    if ( l_status == 0 ) {
      nrfreesockets++;
    }
    if ((l_status == 0x22) || (l_status == 0x14) || (l_status == 0x00)) { //only listen and closed are allowed for long time - Force timer reset
      G_SocketConnectionTimers[l_sock] = 0;
    }
    else if (G_SocketConnectionTimers[l_sock] == 0) {
      //If the socket timer is not active but it is currently connected
      //then we start the socket timer and record the current (start) time.
      G_SocketConnectionTimers[l_sock] = millis(); // of met now()
    }
  }
  if ( !min_serial) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d CheckSocketConnections: Number of free sockets: %02d", hour(), minute(), second(), nrfreesockets); // temptxtbuff is max 1024
    textlog(temptxtbuff, false);
  }
  //Now check to see if any connected sockets have been running for two minutes
  bool l_reset_reqd = false;
  unsigned long l_timer;
  for (uint8_t l_sock = 0; l_sock < MAX_SOCK_NUM; l_sock++) {
    l_timer = G_SocketConnectionTimers[l_sock];
    if ((G_SocketConnectionTimers[l_sock] != 0) && (CheckSecondsDelay(l_timer, (unsigned long)DISCONNECT_TIMER * 1000))) { //l_timer reset to zero at timeout
      Ethernet.socketDisconnect(l_sock);
      Sock_DisCon[l_sock]++;
      G_SocketConnectionTimers[l_sock] = 0; //already reset by CheckSecondsDelay
      sprintf(lastSock_DisCon[l_sock], "%s",  DateTime(now()));
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: Forced socket disconnection after %d seconds. Socket nr: %d", hour(), minute(), second(), DISCONNECT_TIMER, l_sock); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
    }
  }
  nrfreesockets = 0;
  for (uint8_t l_sock = 0; l_sock < MAX_SOCK_NUM; l_sock++) {
    uint8_t l_status = Ethernet.socketStatus(l_sock); //status
    if ( l_status == 0 ) {
      nrfreesockets++;
    }
  }
  return nrfreesockets;
} //CheckSocketConnections

int DisconnectSocketConnections() {
  // check first if we have FIN_Wait sockets
  CloseSocket18Connection();
  int nrfreesockets = 0;
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    if ( Ethernet.socketStatus(i) == 0 ) {
      nrfreesockets++;
    }
  }
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d DisconnectSocketConnections: Number of free sockets: %d", hour(), minute(), second(), nrfreesockets); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  // in case all sockets are occupied, force disconnect all sockets, always 1 socket is reserved for listen
  if ( (MAX_SOCK_NUM - nrfreesockets) >=  (MAX_SOCK_NUM - 1)) {
    sprintf(temptxtbuff, "\r\n%02d:%02d:%02d ERROR: all sockets are occupied, force disconnect all sockets", hour(), minute(), second());
    textlog(temptxtbuff, false);
    for (int i = 0; i < MAX_SOCK_NUM; i++) {
      if (( Ethernet.socketStatus(i) != 0 ) && ( Ethernet.socketStatus(i) != 0x14 ) && ( Ethernet.socketStatus(i) != 0x22 )) {
        Ethernet.socketDisconnect(i);
        Sock_DisCon[i]++;
        sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Force disconnect socket: %d", hour(), minute(), second(), i); // temptxtbuff is max 1024
        textlog(temptxtbuff, false);
        serprintsockstat(i, true);
        sprintf(lastSock_DisCon[i], "%s",  DateTime(now()));
      }
    }
  }
  nrfreesockets = 0;
  for (uint8_t l_sock = 0; l_sock < MAX_SOCK_NUM; l_sock++) {
    uint8_t l_status = Ethernet.socketStatus(l_sock); //status
    if ( l_status == 0 ) {
      nrfreesockets++;
    }
  }
  return nrfreesockets;
}

bool hasUDPsocket() {
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    if (Ethernet.socketStatus(i) == 0x22 ) {
      return true;
    }
  }
  return false;
}

void CloseSocket18Connection() {
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    if ( Ethernet.socketStatus(i) == 0x18 ) {
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Force close socket: %d, due to FIN_WAIT status", hour(), minute(), second(), i); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      Ethernet.socketClose(i);
      delay(500);
      sprintf(temptxtbuff, "\r\n%02d:%02d:%02d New status 0x%02x for socket %d", hour(), minute(), second(), Ethernet.socketStatus(i), i); // temptxtbuff is max 1024
      textlog(temptxtbuff, false);
      lastsocket18close = i;
      sprintf(lastsocket18closetime, "%s",  DateTime(now()));
      socket0x18close++;
    }
  }
}
/*
  void ListSocketStatus() {
  /*
    static const uint8_t CLOSED      = 0x00;
    static const uint8_t INIT        = 0x13;
    static const uint8_t LISTEN      = 0x14;
    static const uint8_t SYNSENT     = 0x15;
    static const uint8_t SYNRECV     = 0x16;
    static const uint8_t ESTABLISHED = 0x17;
    static const uint8_t FIN_WAIT    = 0x18;
    static const uint8_t CLOSING     = 0x1A;
    static const uint8_t TIME_WAIT   = 0x1B;
    static const uint8_t CLOSE_WAIT  = 0x1C;
    static const uint8_t LAST_ACK    = 0x1D;
    static const uint8_t UDP         = 0x22;
    static const uint8_t IPRAW       = 0x32;
    static const uint8_t MACRAW      = 0x42;
    static const uint8_t PPPOE       = 0x5F;
*/
/*
  //byte socketStat[MAX_SOCK_NUM];
  ActivityWrite(EPSR(E_ETHERNET_SOCKET_LIST_27));
  ActivityWrite(EPSR(E_Socket_Heading_28));

  //ActivityWrite("0=avail,14=waiting,17=connected,22=UDP");
  //ActivityWrite("1C=close wait");
  String l_line = "";
  l_line.reserve(64);
  char l_buffer[10] = "";
  for (uint8_t l_socket = 0; l_socket < MAX_SOCK_NUM; l_socket++) {
    l_line = " " + String(l_socket);
    uint8_t l_status = W5100.readSnSR(l_socket); //status
    //socketStat[l_socket] = l_status;
    l_line += " 0x";
    //l_buffer = "";
    sprintf(l_buffer, "%x", l_status);
    if (l_status != 23) { //HEX 17 Connected
      //The connection status has cleared since the last five minute check
      G_SocketConnectionTimers[l_socket] = 0;
      G_SocketConnectionTimes[l_socket] = 0;
    }
    l_line += l_buffer;
    l_line += " ";
    l_line += String(W5100.readSnPORT(l_socket)); //port
    l_line += " D:";
    uint8_t dip[4];
    W5100.readSnDIPR(l_socket, dip); //IP Address
    for (int j = 0; j < 4; j++) {
      l_line += int(dip[j]);
      if (j < 3) l_line += ".";
    }
    l_line += " (";
    l_line += String(W5100.readSnDPORT(l_socket)); //port on destination
    l_line += ") ";
    if (G_SocketConnectionTimes[l_socket] != 0)
      l_line += TimeToHHMM(G_SocketConnectionTimes[l_socket]);
    //Serial.println(l_line);

    ActivityWrite(l_line);
  }
  } // ListSocketStatus
*/
