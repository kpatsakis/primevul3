JsVar *jsvAsArrayIndexAndUnLock(JsVar *a) {
  JsVar *b = jsvAsArrayIndex(a);
  jsvUnLock(a);
  return b;
}
