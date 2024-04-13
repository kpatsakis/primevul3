bool jsvGetBool(const JsVar *v) {
  if (jsvIsString(v))
    return jsvGetStringLength((JsVar*)v)!=0;
  if (jsvIsFunction(v) || jsvIsArray(v) || jsvIsObject(v) || jsvIsArrayBuffer(v))
    return true;
  if (jsvIsFloat(v)) {
    JsVarFloat f = jsvGetFloat(v);
    return !isnan(f) && f!=0.0;
  }
  return jsvGetInteger(v)!=0;
}
