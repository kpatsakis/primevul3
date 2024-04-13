NO_INLINE void jsExceptionHere_flash(JsExceptionType type, const char *ffmt, ...) {
  size_t len = flash_strlen(ffmt);
  char fmt[len+1];
  flash_strncpy(fmt, ffmt, len+1);

  if (jspHasError()) return;

  jsiConsoleRemoveInputLine();

  JsVar *var = jsvNewFromEmptyString();
  if (!var) {
    jspSetError(false);
    return; // out of memory
  }

  JsvStringIterator it;
  jsvStringIteratorNew(&it, var, 0);
  jsvStringIteratorGotoEnd(&it);

  vcbprintf_callback cb = (vcbprintf_callback)jsvStringIteratorPrintfCallback;

  va_list argp;
  va_start(argp, ffmt);
  vcbprintf(cb,&it, fmt, argp);
  va_end(argp);

  jsvStringIteratorFree(&it);

  if (type != JSET_STRING) {
    JsVar *obj = 0;
    if (type == JSET_ERROR) obj = jswrap_error_constructor(var);
    else if (type == JSET_SYNTAXERROR) obj = jswrap_syntaxerror_constructor(var);
    else if (type == JSET_TYPEERROR) obj = jswrap_typeerror_constructor(var);
    else if (type == JSET_INTERNALERROR) obj = jswrap_internalerror_constructor(var);
    else if (type == JSET_REFERENCEERROR) obj = jswrap_referenceerror_constructor(var);
    jsvUnLock(var);
    var = obj;
  }

  jspSetException(var);
  jsvUnLock(var);
}
