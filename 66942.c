bool jsvIsStringEqual(JsVar *var, const char *str) {
  return jsvIsStringEqualOrStartsWith(var, str, false);
}
