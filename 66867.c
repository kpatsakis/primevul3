int jsvGetChildren(const JsVar *v) {
  int children = 0;
  JsVarRef childref = jsvGetFirstChild(v);
  while (childref) {
    JsVar *child = jsvLock(childref);
    children++;
    childref = jsvGetNextSibling(child);
    jsvUnLock(child);
  }
  return children;
}
