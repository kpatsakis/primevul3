bool jsvHasCharacterData(const JsVar *v) {
  return jsvIsString(v) || jsvIsStringExt(v);
}
