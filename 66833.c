void jsvArrayPushAll(JsVar *target, JsVar *source, bool checkDuplicates) {
  assert(jsvIsArray(target));
  assert(jsvIsArray(source));
  JsvObjectIterator it;
  jsvObjectIteratorNew(&it, source);
  while (jsvObjectIteratorHasValue(&it)) {
    JsVar *v = jsvObjectIteratorGetValue(&it);
    bool add = true;
    if (checkDuplicates) {
      JsVar *idx = jsvGetIndexOf(target, v, false);
      if (idx) {
        add = false;
        jsvUnLock(idx);
      }
    }
    if (add) jsvArrayPush(target, v);
    jsvUnLock(v);
    jsvObjectIteratorNext(&it);
  }
  jsvObjectIteratorFree(&it);
}
