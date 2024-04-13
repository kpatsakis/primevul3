JsVarFloat jsvGetFloat(const JsVar *v) {
  if (!v) return NAN; // undefined
  if (jsvIsFloat(v)) return v->varData.floating;
  if (jsvIsIntegerish(v)) return (JsVarFloat)v->varData.integer;
  if (jsvIsArray(v) || jsvIsArrayBuffer(v)) {
    JsVarInt l = jsvGetLength(v);
    if (l==0) return 0; // zero element array==0 (not undefined)
    if (l==1) {
      if (jsvIsArrayBuffer(v))
        return jsvGetFloatAndUnLock(jsvArrayBufferGet((JsVar*)v,0));
      return jsvGetFloatAndUnLock(jsvSkipNameAndUnLock(jsvGetArrayItem(v,0)));
    }
  }
  if (jsvIsString(v)) {
    char buf[64];
    if (jsvGetString(v, buf, sizeof(buf))==sizeof(buf)) {
      jsExceptionHere(JSET_ERROR, "String too big to convert to float\n");
    } else {
      if (buf[0]==0) return 0; // empty string -> 0
      if (!strcmp(buf,"Infinity")) return INFINITY;
      if (!strcmp(buf,"-Infinity")) return -INFINITY;
      return stringToFloat(buf);
    }
  }
  return NAN;
}
