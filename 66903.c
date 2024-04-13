bool jsvHasChildren(const JsVar *v) {
  return jsvIsFunction(v) || jsvIsObject(v) || jsvIsArray(v) || jsvIsRoot(v);
}
