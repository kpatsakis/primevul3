int jsvCompareString(JsVar *va, JsVar *vb, size_t starta, size_t startb, bool equalAtEndOfString) {
  JsvStringIterator ita, itb;
  jsvStringIteratorNew(&ita, va, starta);
  jsvStringIteratorNew(&itb, vb, startb);
  while (true) {
    int ca = jsvStringIteratorGetCharOrMinusOne(&ita);
    int cb = jsvStringIteratorGetCharOrMinusOne(&itb);

    if (ca != cb) {
      jsvStringIteratorFree(&ita);
      jsvStringIteratorFree(&itb);
      if ((ca<0 || cb<0) && equalAtEndOfString) return 0;
      return ca - cb;
    }
    if (ca < 0) { // both equal, but end of string
      jsvStringIteratorFree(&ita);
      jsvStringIteratorFree(&itb);
      return 0;
    }
    jsvStringIteratorNext(&ita);
    jsvStringIteratorNext(&itb);
  }
  return true;
}
