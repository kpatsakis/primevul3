bool jsvIsChild(JsVar *parent, JsVar *child) {
  assert(jsvIsArray(parent) || jsvIsObject(parent));
  assert(jsvIsName(child));
  JsVarRef childref = jsvGetRef(child);
  JsVarRef indexref;
  indexref = jsvGetFirstChild(parent);
  while (indexref) {
    if (indexref == childref) return true;
    JsVar *indexVar = jsvLock(indexref);
    indexref = jsvGetNextSibling(indexVar);
    jsvUnLock(indexVar);
  }
  return false; // not found undefined
}
