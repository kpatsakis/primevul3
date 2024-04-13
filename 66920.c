bool jsvIsFunctionReturn(const JsVar *v) { return v && ((v->flags&JSV_VARTYPEMASK)==JSV_FUNCTION_RETURN); } ///< Is this a function with an implicit 'return' at the start?
