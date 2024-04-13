JsVarFloat stringToFloat(const char *s) {
  return stringToFloatWithRadix(s, 0, NULL); // don't force the radix to anything in particular
}
