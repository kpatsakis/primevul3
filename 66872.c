JsVarRefSigned jsvGetFirstChildSigned(const JsVar *v) {
  JsVarRefSigned r = (JsVarRefSigned)jsvGetFirstChild(v);
  if (r & (1<<(JSVARREF_PACKED_BITS+7)))
    r -= 1<<(JSVARREF_PACKED_BITS+8);
  return r;
}
