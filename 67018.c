NO_INLINE void jsError(const char *fmt, ...) {
  jsiConsoleRemoveInputLine();
  jsiConsolePrint("ERROR: ");
  va_list argp;
  va_start(argp, fmt);
  vcbprintf((vcbprintf_callback)jsiConsolePrintString,0, fmt, argp);
  va_end(argp);
  jsiConsolePrint("\n");
}
