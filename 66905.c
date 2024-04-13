bool jsvHasStringExt(const JsVar *v) {
  return jsvIsString(v) || jsvIsStringExt(v);
}
