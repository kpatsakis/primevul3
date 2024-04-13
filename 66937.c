bool jsvIsPin(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)==JSV_PIN; }
