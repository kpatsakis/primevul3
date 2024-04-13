void ftoa_bounded(JsVarFloat val,char *str, size_t len) {
  ftoa_bounded_extra(val, str, len, 10, -1);
}
