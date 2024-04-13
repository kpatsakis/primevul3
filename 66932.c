bool jsvIsNativeFunction(const JsVar *v) { return v && (v->flags&(JSV_NATIVE|JSV_VARTYPEMASK))==(JSV_NATIVE|JSV_FUNCTION); }
