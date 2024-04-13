const char *jsvGetConstString(const JsVar *v) {
  if (jsvIsUndefined(v)) {
    return "undefined";
  } else if (jsvIsNull(v)) {
    return "null";
  } else if (jsvIsBoolean(v)) {
    return jsvGetBool(v) ? "true" : "false";
  }
  return 0;
}
