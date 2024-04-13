bool jsvIsBoolean(const JsVar *v) { return v && ((v->flags&JSV_VARTYPEMASK)==JSV_BOOLEAN || (v->flags&JSV_VARTYPEMASK)==JSV_NAME_INT_BOOL); }
