JsVar *jsvAsNumber(JsVar *var) {
  if (jsvIsInt(var) || jsvIsFloat(var)) return jsvLockAgain(var);
  if (jsvIsBoolean(var) ||
      jsvIsPin(var) ||
      jsvIsNull(var) ||
      jsvIsBoolean(var) ||
      jsvIsArrayBufferName(var))
    return jsvNewFromInteger(jsvGetInteger(var));
  if (jsvIsString(var) && (jsvIsEmptyString(var) || jsvIsStringNumericInt(var, false/* no decimal pt - handle that with GetFloat */))) {
    char buf[64];
    if (jsvGetString(var, buf, sizeof(buf))==sizeof(buf)) {
      jsExceptionHere(JSET_ERROR, "String too big to convert to integer\n");
      return jsvNewFromFloat(NAN);
    } else
      return jsvNewFromLongInteger(stringToInt(buf));
  }
  return jsvNewFromFloat(jsvGetFloat(var));
}
