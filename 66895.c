JsVarRef jsvGetPrevSibling(const JsVar *v) { return (JsVarRef)(v->varData.ref.prevSibling | (((v->varData.ref.pack >> (JSVARREF_PACKED_BITS*3))&JSVARREF_PACKED_BIT_MASK))<<8); }
