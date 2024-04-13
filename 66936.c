bool jsvIsObject(const JsVar *v) { return v && (((v->flags&JSV_VARTYPEMASK)==JSV_OBJECT) || ((v->flags&JSV_VARTYPEMASK)==JSV_ROOT)); }
