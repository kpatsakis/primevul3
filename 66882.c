JsVarInt jsvGetInteger(const JsVar *v) {
  if (!v) return 0; // undefined
  /* strtol understands about hex and octal */
  if (jsvIsNull(v)) return 0;
  if (jsvIsUndefined(v)) return 0;
  if (jsvIsIntegerish(v) || jsvIsArrayBufferName(v)) return v->varData.integer;
  if (jsvIsArray(v) || jsvIsArrayBuffer(v)) {
    JsVarInt l = jsvGetLength((JsVar *)v);
    if (l==0) return 0; // 0 length, return 0
    if (l==1) {
      if (jsvIsArrayBuffer(v))
        return jsvGetIntegerAndUnLock(jsvArrayBufferGet((JsVar*)v,0));
      return jsvGetIntegerAndUnLock(jsvSkipNameAndUnLock(jsvGetArrayItem(v,0)));
    }
  }
  if (jsvIsFloat(v)) {
    if (isfinite(v->varData.floating))
      return (JsVarInt)(long long)v->varData.floating;
    return 0;
  }
  if (jsvIsString(v) && jsvIsStringNumericInt(v, true/* allow decimal point*/)) {
    char buf[32];
    if (jsvGetString(v, buf, sizeof(buf))==sizeof(buf))
      jsExceptionHere(JSET_ERROR, "String too big to convert to integer\n");
    else
      return (JsVarInt)stringToInt(buf);
  }
  return 0;
}
