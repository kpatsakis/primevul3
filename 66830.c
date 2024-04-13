JsVar *jsvArrayJoin(JsVar *arr, JsVar *filler) {
  JsVar *str = jsvNewFromEmptyString();
  if (!str) return 0; // out of memory

  JsvIterator it;
  jsvIteratorNew(&it, arr, JSIF_EVERY_ARRAY_ELEMENT);
  bool first = true;
  while (!jspIsInterrupted() && jsvIteratorHasElement(&it)) {
    JsVar *key = jsvIteratorGetKey(&it);
    if (jsvIsInt(key)) {
      if (filler && !first)
        jsvAppendStringVarComplete(str, filler);
      first = false;
      JsVar *value = jsvIteratorGetValue(&it);
      if (value && !jsvIsNull(value)) {
        JsVar *valueStr = jsvAsString(value, false);
        if (valueStr) { // could be out of memory
          jsvAppendStringVarComplete(str, valueStr);
          jsvUnLock(valueStr);
        }
      }
      jsvUnLock(value);
    }
    jsvUnLock(key);
    jsvIteratorNext(&it);
  }
  jsvIteratorFree(&it);
  return str;
}
