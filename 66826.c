void jsvAppendStringVarComplete(JsVar *var, const JsVar *str) {
  jsvAppendStringVar(var, str, 0, JSVAPPENDSTRINGVAR_MAXLENGTH);
}
