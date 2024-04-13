void cbprintf(vcbprintf_callback user_callback, void *user_data, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vcbprintf(user_callback,user_data, fmt, argp);
  va_end(argp);
}
