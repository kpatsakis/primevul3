char jsvGetCharInString(JsVar *v, size_t idx) {
  if (!jsvIsString(v)) return 0;

  JsvStringIterator it;
  jsvStringIteratorNew(&it, v, idx);
  char ch = jsvStringIteratorGetChar(&it);
  jsvStringIteratorFree(&it);
  return ch;
}
