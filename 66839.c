JsVar *jsvAsString(JsVar *v, bool unlockVar) {
  JsVar *str = 0;
  if (jsvHasCharacterData(v) && jsvIsName(v)) {
    str = jsvNewFromStringVar(v,0,JSVAPPENDSTRINGVAR_MAXLENGTH);
  } else if (jsvIsString(v)) { // If it is a string - just return a reference
    str = jsvLockAgain(v);
  } else if (jsvIsObject(v)) { // If it is an object and we can call toString on it
    JsVar *toStringFn = jspGetNamedField(v, "toString", false);
    if (toStringFn && toStringFn->varData.native.ptr != (void (*)(void))jswrap_object_toString) {
      JsVar *result = jspExecuteFunction(toStringFn,v,0,0);
      jsvUnLock(toStringFn);
      str = jsvAsString(result, true);
    } else {
      jsvUnLock(toStringFn);
      str = jsvNewFromString("[object Object]");
    }
  } else {
    const char *constChar = jsvGetConstString(v);
    assert(JS_NUMBER_BUFFER_SIZE>=10);
    char buf[JS_NUMBER_BUFFER_SIZE];
    if (constChar) {
      str = jsvNewFromString(constChar);
    } else if (jsvIsPin(v)) {
      jshGetPinString(buf, (Pin)v->varData.integer);
      str = jsvNewFromString(buf);
    } else if (jsvIsInt(v)) {
      itostr(v->varData.integer, buf, 10);
      str = jsvNewFromString(buf);
    } else if (jsvIsFloat(v)) {
      ftoa_bounded(v->varData.floating, buf, sizeof(buf));
      str = jsvNewFromString(buf);
    } else if (jsvIsArray(v) || jsvIsArrayBuffer(v)) {
      JsVar *filler = jsvNewFromString(",");
      str = jsvArrayJoin(v, filler);
      jsvUnLock(filler);
    } else if (jsvIsFunction(v)) {
      str = jsvNewFromEmptyString();
      if (str) jsfGetJSON(v, str, JSON_NONE);
    } else {
      jsExceptionHere(JSET_INTERNALERROR, "Variable type cannot be converted to string");
      str = 0;
    }
  }

  if (unlockVar) jsvUnLock(v);
  return str;
}
