void jsvGetArrayItems(JsVar *arr, unsigned int itemCount, JsVar **itemPtr) {
  JsvObjectIterator it;
  jsvObjectIteratorNew(&it, arr);
  unsigned int i = 0;
  while (jsvObjectIteratorHasValue(&it)) {
    if (i<itemCount)
      itemPtr[i++] = jsvObjectIteratorGetValue(&it);
    jsvObjectIteratorNext(&it);
  }
  jsvObjectIteratorFree(&it);
  while (i<itemCount)
    itemPtr[i++] = 0; // just ensure we don't end up with bad data
}
