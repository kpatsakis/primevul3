JsVarRef jsvGetFirstChild(const JsVar *v) { return (JsVarRef)(v->varData.ref.firstChild | (((v->varData.ref.pack)&JSVARREF_PACKED_BIT_MASK))<<8); }
