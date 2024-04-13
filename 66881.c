JsVar *jsvGetIndexOf(JsVar *arr, JsVar *value, bool matchExact) {
  return jsvGetIndexOfFull(arr, value, matchExact, false, 0);
}
