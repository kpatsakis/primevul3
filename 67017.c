NO_INLINE void jsAssertFail(const char *file, int line, const char *expr) {
  static bool inAssertFail = false;
  bool wasInAssertFail = inAssertFail;
  inAssertFail = true;
  jsiConsoleRemoveInputLine();
  if (expr) {
#ifndef USE_FLASH_MEMORY
    jsiConsolePrintf("ASSERT(%s) FAILED AT ", expr);
#else
    jsiConsolePrintString("ASSERT(");
    size_t len = flash_strlen(expr);
    char buff[len+1];
    flash_strncpy(buff, expr, len+1);
    jsiConsolePrintString(buff);
    jsiConsolePrintString(") FAILED AT ");
#endif
  } else {
    jsiConsolePrint("ASSERT FAILED AT ");
  }
  jsiConsolePrintf("%s:%d\n",file,line);
  if (!wasInAssertFail) {
    jsvTrace(jsvFindOrCreateRoot(), 2);
  }
#if defined(ARM)
  jsiConsolePrint("REBOOTING.\n");
  jshTransmitFlush();
  NVIC_SystemReset();
#elif defined(ESP8266)
  int os_printf_plus(const char *format, ...)  __attribute__((format(printf, 1, 2)));
  os_printf_plus("ASSERT FAILED AT %s:%d\n", file,line);
  jsiConsolePrint("---console end---\n");
  int c, console = jsiGetConsoleDevice();
  while ((c=jshGetCharToTransmit(console)) >= 0)
    os_printf_plus("%c", c);
  os_printf_plus("CRASHING.\n");
  *(int*)0xdead = 0xbeef;
  extern void jswrap_ESP8266_reboot(void);
  jswrap_ESP8266_reboot();
  while(1) ;
#elif defined(LINUX)
  jsiConsolePrint("EXITING.\n");
  exit(1);
#else
  jsiConsolePrint("HALTING.\n");
  while (1);
#endif
  inAssertFail = false;
}
