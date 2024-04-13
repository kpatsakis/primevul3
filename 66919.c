bool jsvIsFunctionParameter(const JsVar *v) { return v && (v->flags&JSV_NATIVE) && jsvIsString(v); }
