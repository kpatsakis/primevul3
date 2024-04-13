JsVar *jsvFindChildFromVar(JsVar *parent, JsVar *childName, bool addIfNotFound) {
  JsVar *child;
  JsVarRef childref = jsvGetFirstChild(parent);

  while (childref) {
    child = jsvLock(childref);
    if (jsvIsBasicVarEqual(child, childName)) {
      return child;
    }
    childref = jsvGetNextSibling(child);
    jsvUnLock(child);
  }

  child = 0;
  if (addIfNotFound && childName) {
    child = jsvAsName(childName);
    jsvAddName(parent, child);
  }
  return child;
}
