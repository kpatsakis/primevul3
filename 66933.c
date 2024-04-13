bool jsvIsNativeString(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)==JSV_NATIVE_STRING; }
