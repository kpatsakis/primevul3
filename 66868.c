JsVar *jsvGetCommonCharacters(JsVar *va, JsVar *vb) {
  JsVar *v = jsvNewFromEmptyString();
  if (!v) return 0;
  JsvStringIterator ita, itb;
  jsvStringIteratorNew(&ita, va, 0);
  jsvStringIteratorNew(&itb, vb, 0);
  int ca = jsvStringIteratorGetCharOrMinusOne(&ita);
  int cb = jsvStringIteratorGetCharOrMinusOne(&itb);
  while (ca>0 && cb>0 && ca == cb) {
    jsvAppendCharacter(v, (char)ca);
    jsvStringIteratorNext(&ita);
    jsvStringIteratorNext(&itb);
    ca = jsvStringIteratorGetCharOrMinusOne(&ita);
    cb = jsvStringIteratorGetCharOrMinusOne(&itb);
  }
  jsvStringIteratorFree(&ita);
  jsvStringIteratorFree(&itb);
  return v;
}
