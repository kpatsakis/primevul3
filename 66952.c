ALWAYS_INLINE JsVar *jsvLockAgainSafe(JsVar *var) {
  return var ? jsvLockAgain(var) : 0;
}
