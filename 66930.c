bool jsvIsNameWithValue(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)>=_JSV_NAME_WITH_VALUE_START && (v->flags&JSV_VARTYPEMASK)<=_JSV_NAME_WITH_VALUE_END; }
