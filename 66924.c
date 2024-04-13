bool jsvIsInternalFunctionKey(JsVar *v) {
  return (jsvIsString(v) && (
      v->varData.str[0]==JS_HIDDEN_CHAR)
  ) ||
  jsvIsFunctionParameter(v);
}
