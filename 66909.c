bool jsvIsBasic(const JsVar *v) { return jsvIsNumeric(v) || jsvIsString(v);} ///< Is this *not* an array/object/etc
