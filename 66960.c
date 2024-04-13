JsVar *jsvNegateAndUnLock(JsVar *v) {
  JsVar *zero = jsvNewFromInteger(0);
  JsVar *res = jsvMathsOpSkipNames(zero, v, '-');
  jsvUnLock2(zero, v);
  return res;
}
