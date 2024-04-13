bool jsvIsSimpleInt(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)==JSV_INTEGER; } // is just a very basic integer value
