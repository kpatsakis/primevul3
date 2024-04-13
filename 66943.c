bool jsvIsStringEqualOrStartsWith(JsVar *var, const char *str, bool isStartsWith) {
  return jsvIsStringEqualOrStartsWithOffset(var, str, isStartsWith, 0, false);
}
