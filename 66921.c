bool jsvIsInstanceOf(JsVar *var, const char *constructorName) {
  bool isInst = false;
  if (!jsvHasChildren(var)) return false;
  JsVar *proto = jsvObjectGetChild(var, JSPARSE_INHERITS_VAR, 0);
  if (jsvIsObject(proto)) {
    JsVar *constr = jsvObjectGetChild(proto, JSPARSE_CONSTRUCTOR_VAR, 0);
    if (constr)
      isInst = jspIsConstructor(constr, constructorName);
    jsvUnLock(constr);
  }
  jsvUnLock(proto);
  return isInst;
}
