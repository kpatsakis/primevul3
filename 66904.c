bool jsvHasSingleChild(const JsVar *v) {
  return jsvIsArrayBuffer(v) ||
      (jsvIsName(v) && !jsvIsNameWithValue(v));
}
