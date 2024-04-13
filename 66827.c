void jsvArrayAddUnique(JsVar *arr, JsVar *v) {
  JsVar *idx = jsvGetIndexOf(arr, v, false); // did it already exist?
  if (!idx) {
    jsvArrayPush(arr, v); // if 0, it failed
  } else {
    jsvUnLock(idx);
  }
}
