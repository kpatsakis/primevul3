NO_INLINE void jsError_flash(const char *fmt, ...) {
  size_t len = flash_strlen(fmt);
  char buff[len+1];
  flash_strncpy(buff, fmt, len+1);

  jsiConsoleRemoveInputLine();
  jsiConsolePrint("ERROR: ");
  va_list argp;
  va_start(argp, fmt);
  vcbprintf((vcbprintf_callback)jsiConsolePrintString,0, buff, argp);
  va_end(argp);
  jsiConsolePrint("\n");
}
