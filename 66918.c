bool jsvIsFunction(const JsVar *v) { return v && ((v->flags&JSV_VARTYPEMASK)==JSV_FUNCTION || (v->flags&JSV_VARTYPEMASK)==JSV_FUNCTION_RETURN); }
