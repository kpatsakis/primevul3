JsVar *jsvAddNamedChild(JsVar *parent, JsVar *child, const char *name) {
  JsVar *namedChild = jsvMakeIntoVariableName(jsvNewFromString(name), child);
  if (!namedChild) return 0; // Out of memory
  jsvAddName(parent, namedChild);
  return namedChild;
}
