JsVarRef jsvGetNextSibling(const JsVar *v) { return (JsVarRef)(v->varData.ref.nextSibling | (((v->varData.ref.pack >> (JSVARREF_PACKED_BITS*2))&JSVARREF_PACKED_BIT_MASK))<<8); }
