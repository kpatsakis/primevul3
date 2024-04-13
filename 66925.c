bool jsvIsIterable(const JsVar *v) {
  return jsvIsArray(v) || jsvIsObject(v) || jsvIsFunction(v) ||
         jsvIsString(v) || jsvIsArrayBuffer(v);
}
