bool jsvIsString(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)>=_JSV_STRING_START && (v->flags&JSV_VARTYPEMASK)<=_JSV_STRING_END; } ///< String, or a NAME too
