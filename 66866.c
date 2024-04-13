size_t jsvGetCharsOnLine(JsVar *v, size_t line) {
  size_t currentLine = 1;
  size_t chars = 0;
  JsvStringIterator it;
  jsvStringIteratorNew(&it, v, 0);
  while (jsvStringIteratorHasChar(&it)) {
    if (jsvStringIteratorGetChar(&it)=='\n') {
      currentLine++;
      if (currentLine > line) break;
    } else if (currentLine==line) chars++;
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  return chars;
}
