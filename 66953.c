void jsvMakeFunctionParameter(JsVar *v) {
  assert(jsvIsString(v));
  if (!jsvIsName(v)) jsvMakeIntoVariableName(v,0);
  v->flags = (JsVarFlags)(v->flags | JSV_NATIVE);
}
