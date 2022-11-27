/*
  This sketch employs mallinfo() to retrieve memory allocation
  statistics before and after allocating and freeing blocks of
  memory. The statistics are displayed on Serial.
  Creation 14 Feb 2018
  by Frederic Pillon
  This example code is in the public domain.
  Based on example from:
  http://man7.org/linux/man-pages/man3/mallinfo.3.html
*/
#include <malloc.h>

extern "C" char *sbrk(int i);


//#define NUM_BLOCKS 100
//#define BLOCK_SIZE 4

void display_mallinfo(EthernetClient client)
{
#ifdef WOLFSSL_DEBUG_MEMORY
  Serial.println("WOLFSSL_DEBUG_MEMORY is enabled");
  client.print(F("<p align=\"left\">========Function currently disabled due to WOLFSSL_DEBUG_MEMORY is enabled====<br>\r\n"));
  return;
#endif

#ifdef WOLFSSL_TRACK_MEMORY
  Serial.println("WOLFSSL_TRACK_MEMORY is enabled");
  client.print(F("<p align=\"left\">========Function currently disabled due to WOLFSSL_TRACK_MEMORY is enabled====<br>\r\n"));
  return;
#endif
  /* Use linker definition */
  extern char _end;
  extern char _sdata;
  extern char _estack;
  extern char _Min_Stack_Size;

  static char *ramstart = &_sdata;
  static char *ramend = &_estack;
  static char *minSP = (char*)(ramend - &_Min_Stack_Size);


  char *heapend = (char*)sbrk(0);
  char * stack_ptr = (char*)__get_MSP();
  struct mallinfo mi = mallinfo();
  client << (F("<table border=\"1\" cellspacing=\"0\">\r\n<caption>STM32 actual memory status</caption>\r\n"));
  client << ("<tr><td align=\"left\">Total non-mmapped bytes (arena)</td><td align=\"right\">") << mi.arena << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\"># of free chunks (ordblks)</td><td align=\"right\">") << mi.ordblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\"># of free fastbin blocks (smblks)</td><td align=\"right\">") << mi.smblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\"># of mapped regions (hblks)</td><td align=\"right\">") << mi.hblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Bytes in mapped regions (hblkhd)</td><td align=\"right\">") << mi.hblkhd << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Max. total allocated space (usmblks</td>)<td align=\"right\">") << mi.usmblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Free bytes held in fastbins (fsmblks)</td><td align=\"right\">") << mi.fsmblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Total allocated space (uordblks)</td><td align=\"right\">") << mi.uordblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Total free space (fordblks)</td><td align=\"right\">") << mi.fordblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Topmost releasable block (keepcost)</td><td align=\"right\">") << mi.keepcost << (F("</td></tr>\r\n"));

  client << ("<tr><td align=\"left\">RAM Start at</td><td align=\"right\">0x");
  client.print((unsigned long)ramstart, HEX);
  client << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Data/Bss end at</td><td align=\"right\">0x");
  client.print((unsigned long)&_end, HEX);
  client << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Heap end at</td><td align=\"right\">0x");
  client.print((unsigned long)heapend, HEX);
  client << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Stack Ptr end at</td><td align=\"right\">0x");
  client.print((unsigned long)stack_ptr, HEX);
  client << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">RAM End at</td><td align=\"right\">0x");
  client.print((unsigned long)ramend, HEX);
  client << (F("</td></tr>\r\n"));

  client << ("<tr><td align=\"left\">\r\nHeap RAM Used</td><td align=\"right\">") << mi.uordblks << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Program RAM Used</td><td align=\"right\">") << (&_end - ramstart) << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Stack RAM Used</td><td align=\"right\">") << (ramend - stack_ptr) << (F("</td></tr>\r\n"));
  client << ("<tr><td align=\"left\">Estimated Free RAM</td><td align=\"right\">") << (((stack_ptr < minSP) ? stack_ptr : minSP) - heapend + mi.fordblks) << (F("</td></tr>\r\n"));
  client << (F("</table>\r\n</body>\r\n</html>\r\n"));
}

int STM32freeRAM() {
#ifdef WOLFSSL_DEBUG_MEMORY
  Serial.println("INFORMATION: Function currently disabled due to WOLFSSL_DEBUG_MEMORY is enabled");
  return (-255);
#endif

#ifdef WOLFSSL_TRACK_MEMORY
  Serial.println("INFORMATION: Function currently disabled due to WOLFSSL_TRACK_MEMORY is enabled");
  return (-255);
#endif

  extern char _end;
  extern char _sdata;
  extern char _estack;
  extern char _Min_Stack_Size;

  static char *ramstart = &_sdata;
  static char *ramend = &_estack;
  static char *minSP = (char*)(ramend - &_Min_Stack_Size);

  char *heapend = (char*)sbrk(0);
  char * stack_ptr = (char*)__get_MSP();
  struct mallinfo mi = mallinfo();
  //  Serial.print("\r\nHeap end at:        0x");
  //  Serial.println((unsigned long)heapend, HEX);
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Heap end at:        0x%08lX", hour(), minute(), second(), (unsigned long)heapend); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  //  Serial.print("Stack Ptr end at:   0x");
  //  Serial.println((unsigned long)stack_ptr, HEX);
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Stack Ptr end at:   0x%08lX", hour(), minute(), second(), (unsigned long)stack_ptr); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);
  //  Serial.print("Estimated Free RAM: ");
  //  Serial.print(((stack_ptr < minSP) ? stack_ptr : minSP) - heapend + mi.fordblks);
  sprintf(temptxtbuff, "\r\n%02d:%02d:%02d Estimated Free RAM: %d bytes", hour(), minute(), second(), ((stack_ptr < minSP) ? stack_ptr : minSP) - heapend + mi.fordblks); // temptxtbuff is max 1024
  textlog(temptxtbuff, false);

  return (((stack_ptr < minSP) ? stack_ptr : minSP) - heapend + mi.fordblks);
}
/*
  void showMemory(EthernetClient client) {

  Serial.println("============== Before allocating blocks ==============");
  delay(1000);
  display_mallinfo(EthernetClient client);

  for (n = 0; n < NUM_BLOCKS; n++) {
    alloc[n] = (char*)malloc(BLOCK_SIZE);
    if (alloc[n] == NULL) {
      Serial.print("Failed to allocate blocks ");
      Serial.println(n);
      while (1);
    }
  }

  Serial.println("============== After allocating blocks ==============");
  display_mallinfo();

  for (n = 0; n < NUM_BLOCKS; n++)
    free(alloc[n]);

  Serial.println("============== After freeing blocks ==============");
  display_mallinfo();
  while (1);
  }
*/
