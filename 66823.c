void jsvAppendPrintf(JsVar *var, const char *fmt, ...) {
  JsvStringIterator it;
  jsvStringIteratorNew(&it, var, 0);
  jsvStringIteratorGotoEnd(&it);

  va_list argp;
  va_start(argp, fmt);
  vcbprintf((vcbprintf_callback)jsvStringIteratorPrintfCallback,&it, fmt, argp);
  va_end(argp);

  jsvStringIteratorFree(&it);
}
