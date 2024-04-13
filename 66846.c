JsVar *jsvCreateNewChild(JsVar *parent, JsVar *index, JsVar *child) {
  JsVar *newChild = jsvAsName(index);
  if (!newChild) return 0;
  assert(!jsvGetFirstChild(newChild));
  if (child) jsvSetValueOfName(newChild, child);
  assert(!jsvGetNextSibling(newChild) && !jsvGetPrevSibling(newChild));
  JsVarRef r = jsvGetRef(jsvRef(jsvRef(parent)));
  jsvSetNextSibling(newChild, r);
  jsvSetPrevSibling(newChild, r);

  return newChild;
}
