long long jsvGetLongInteger(const JsVar *v) {
  if (jsvIsInt(v)) return jsvGetInteger(v);
  return (long long)jsvGetFloat(v);
}
