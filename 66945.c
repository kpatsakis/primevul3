bool jsvIsStringExt(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)>=JSV_STRING_EXT_0 && (v->flags&JSV_VARTYPEMASK)<=JSV_STRING_EXT_MAX; } ///< The extra bits dumped onto the end of a string to store more data
