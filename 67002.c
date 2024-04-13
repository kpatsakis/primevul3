int espruino_snprintf( char * s, size_t n, const char * fmt, ... ) {
  espruino_snprintf_data d;
  d.outPtr = s;
  d.idx = 0;
  d.len = n;

  va_list argp;
  va_start(argp, fmt);
  vcbprintf(espruino_snprintf_cb,&d, fmt, argp);
  va_end(argp);

  if (d.idx < d.len) d.outPtr[d.idx] = 0;
  else d.outPtr[d.len-1] = 0;

  return (int)d.idx;
}
