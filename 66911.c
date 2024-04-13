bool jsvIsBasicVarEqual(JsVar *a, JsVar *b) {
  if (a==b) return true;
  if (!a || !b) return false; // one of them is undefined
  assert(jsvIsBasic(a) && jsvIsBasic(b));
  if (jsvIsNumeric(a) && jsvIsNumeric(b)) {
    if (jsvIsIntegerish(a)) {
      if (jsvIsIntegerish(b)) {
        return a->varData.integer == b->varData.integer;
      } else {
        assert(jsvIsFloat(b));
        return a->varData.integer == b->varData.floating;
      }
    } else {
      assert(jsvIsFloat(a));
      if (jsvIsIntegerish(b)) {
        return a->varData.floating == b->varData.integer;
      } else {
        assert(jsvIsFloat(b));
        return a->varData.floating == b->varData.floating;
      }
    }
  } else if (jsvIsString(a) && jsvIsString(b)) {
    JsvStringIterator ita, itb;
    jsvStringIteratorNew(&ita, a, 0);
    jsvStringIteratorNew(&itb, b, 0);
    while (true) {
      char a = jsvStringIteratorGetChar(&ita);
      char b = jsvStringIteratorGetChar(&itb);
      if (a != b) {
        jsvStringIteratorFree(&ita);
        jsvStringIteratorFree(&itb);
        return false;
      }
      if (!a) { // equal, but end of string
        jsvStringIteratorFree(&ita);
        jsvStringIteratorFree(&itb);
        return true;
      }
      jsvStringIteratorNext(&ita);
      jsvStringIteratorNext(&itb);
    }
    return false; // make compiler happy
  } else {
    return false;
  }
}
