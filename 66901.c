JsVar *jsvGetValueOf(JsVar *v) {
  if (!jsvIsObject(v)) return jsvLockAgainSafe(v);
  JsVar *valueOf = jspGetNamedField(v, "valueOf", false);
  if (!jsvIsFunction(valueOf)) {
    jsvUnLock(valueOf);
    return jsvLockAgain(v);
  }
  v = jspeFunctionCall(valueOf, 0, v, false, 0, 0);
  jsvUnLock(valueOf);
  return v;
}
