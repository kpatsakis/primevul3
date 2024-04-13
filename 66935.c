bool jsvIsNumeric(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)>=_JSV_NUMERIC_START && (v->flags&JSV_VARTYPEMASK)<=_JSV_NUMERIC_END; }
