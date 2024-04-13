JsVar *jsvAsName(JsVar *var) {
  if (!var) return 0;
  if (jsvGetRefs(var) == 0) {
    if (!jsvIsName(var))
      var = jsvMakeIntoVariableName(var, 0);
    return jsvLockAgain(var);
  } else { // it was reffed, we must add a new one
    return jsvMakeIntoVariableName(jsvCopy(var, false), 0);
  }
}
