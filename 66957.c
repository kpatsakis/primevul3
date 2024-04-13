JsVar *jsvMathsOpSkipNames(JsVar *a, JsVar *b, int op) {
  JsVar *pa = jsvSkipName(a);
  JsVar *pb = jsvSkipName(b);
  JsVar *oa = jsvGetValueOf(pa);
  JsVar *ob = jsvGetValueOf(pb);
  jsvUnLock2(pa, pb);
  JsVar *res = jsvMathsOp(oa,ob,op);
  jsvUnLock2(oa, ob);
  return res;
}
