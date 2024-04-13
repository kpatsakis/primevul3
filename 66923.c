bool jsvIsIntegerish(const JsVar *v) { return jsvIsInt(v) || jsvIsPin(v) || jsvIsBoolean(v) || jsvIsNull(v); }
