JsVar *jsvGetPathTo(JsVar *root, JsVar *element, int maxDepth, JsVar *ignoreParent) {
  if (maxDepth<=0) return 0;
  JsvIterator it;
  jsvIteratorNew(&it, root, JSIF_DEFINED_ARRAY_ElEMENTS);
  while (jsvIteratorHasElement(&it)) {
    JsVar *el = jsvIteratorGetValue(&it);
    if (el == element && root != ignoreParent) {
      JsVar *name = jsvAsString(jsvIteratorGetKey(&it), true);
      jsvIteratorFree(&it);
      return name;
    } else if (jsvIsObject(el) || jsvIsArray(el) || jsvIsFunction(el)) {
      JsVar *n = jsvGetPathTo(el, element, maxDepth-1, ignoreParent);
      if (n) {
        JsVar *keyName = jsvIteratorGetKey(&it);
        JsVar *name = jsvVarPrintf(jsvIsObject(el) ? "%v.%v" : "%v[%q]",keyName,n);
        jsvUnLock2(keyName, n);
        jsvIteratorFree(&it);
        return name;
      }
    }
    jsvIteratorNext(&it);
  }
  jsvIteratorFree(&it);
  return 0;
}
