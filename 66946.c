bool jsvIsStringIEqualAndUnLock(JsVar *var, const char *str) {
  bool b = jsvIsStringEqualOrStartsWithOffset(var, str, false, 0, true);
  jsvUnLock(var);
  return b;
}
