size_t jsvGetLinesInString(JsVar *v) {
  size_t lines = 1;
  JsvStringIterator it;
  jsvStringIteratorNew(&it, v, 0);
  while (jsvStringIteratorHasChar(&it)) {
    if (jsvStringIteratorGetChar(&it)=='\n') lines++;
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  return lines;
}
