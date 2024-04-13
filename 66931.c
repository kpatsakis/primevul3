bool jsvIsNative(const JsVar *v) { return v && (v->flags&JSV_NATIVE)!=0; }
