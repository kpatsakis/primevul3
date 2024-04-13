size_t jsvCountJsVarsUsed(JsVar *v) {
  size_t c = _jsvCountJsVarsUsedRecursive(v, false);
  _jsvCountJsVarsUsedRecursive(v, true);
  return c;
}
