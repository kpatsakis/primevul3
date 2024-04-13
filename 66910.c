bool jsvIsBasicString(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)>=JSV_STRING_0 && (v->flags&JSV_VARTYPEMASK)<=JSV_STRING_MAX; } ///< Just a string (NOT a name)
