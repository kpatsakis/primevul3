JsVarRef jsvGetLastChild(const JsVar *v) {
  return (JsVarRef)(v->varData.ref.lastChild | (((v->flags >> JSV_LASTCHILD_BIT_SHIFT)&JSVARREF_PACKED_BIT_MASK))<<8);
}
