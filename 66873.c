JsVar *jsvGetFirstName(JsVar *v) {
  assert(jsvHasChildren(v));
  if (!jsvGetFirstChild(v)) return 0;
  return jsvLock(jsvGetFirstChild(v));
}
