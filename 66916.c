bool jsvIsFlatString(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)==JSV_FLAT_STRING; }
