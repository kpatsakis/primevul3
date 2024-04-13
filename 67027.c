void vcbprintf(
    vcbprintf_callback user_callback, //!< Unknown
    void *user_data,                  //!< Unknown
    const char *fmt,                  //!< The format specified
    va_list argp                      //!< List of parameter values
  ) {
  char buf[32];
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      char fmtChar = *fmt++;
      switch (fmtChar) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        const char *pad = " ";
        if (fmtChar=='0') {
          pad = "0";
          fmtChar = *fmt++;
        }
        int digits = fmtChar - '0';
        int v = va_arg(argp, int);
        if (*fmt=='x') itostr_extra(v, buf, false, 16);
        else { assert('d' == *fmt); itostr(v, buf, 10); }
        fmt++; // skip over 'd'
        int len = (int)strlen(buf);
        while (len < digits) {
          user_callback(pad,user_data);
          len++;
        }
        user_callback(buf,user_data);
        break;
      }
      case 'd': itostr(va_arg(argp, int), buf, 10); user_callback(buf,user_data); break;
      case 'x': itostr_extra(va_arg(argp, int), buf, false, 16); user_callback(buf,user_data); break;
      case 'L': {
        unsigned int rad = 10;
        bool signedVal = true;
        if (*fmt=='x') { rad=16; fmt++; signedVal = false; }
        itostr_extra(va_arg(argp, JsVarInt), buf, signedVal, rad); user_callback(buf,user_data);
      } break;
      case 'f': ftoa_bounded(va_arg(argp, JsVarFloat), buf, sizeof(buf)); user_callback(buf,user_data);  break;
      case 's': user_callback(va_arg(argp, char *), user_data); break;
      case 'c': buf[0]=(char)va_arg(argp, int/*char*/);buf[1]=0; user_callback(buf, user_data); break;
      case 'q':
      case 'v': {
        bool quoted = fmtChar=='q';
        if (quoted) user_callback("\"",user_data);
        JsVar *v = jsvAsString(va_arg(argp, JsVar*), false/*no unlock*/);
        buf[1] = 0;
        if (jsvIsString(v)) {
          JsvStringIterator it;
          jsvStringIteratorNew(&it, v, 0);
          while (jsvStringIteratorHasChar(&it)) {
            buf[0] = jsvStringIteratorGetChar(&it);
            if (quoted) {
              user_callback(escapeCharacter(buf[0]), user_data);
            } else {
              user_callback(buf,user_data);
            }
            jsvStringIteratorNext(&it);
          }
          jsvStringIteratorFree(&it);
          jsvUnLock(v);
        }
        if (quoted) user_callback("\"",user_data);
      } break;
      case 'j': {
        JsVar *v = va_arg(argp, JsVar*);
        jsfGetJSONWithCallback(v, JSON_SOME_NEWLINES | JSON_PRETTY | JSON_SHOW_DEVICES, 0, user_callback, user_data);
        break;
      }
      case 't': {
        JsVar *v = va_arg(argp, JsVar*);
        const char *n = jsvIsNull(v)?"null":jswGetBasicObjectName(v);
        if (!n) n = jsvGetTypeOf(v);
        user_callback(n, user_data);
        break;
      }
      case 'p': jshGetPinString(buf, (Pin)va_arg(argp, int/*Pin*/)); user_callback(buf, user_data); break;
      default: assert(0); return; // eep
      }
    } else {
      buf[0] = *(fmt++);
      buf[1] = 0;
      user_callback(&buf[0], user_data);
    }
  }
}
