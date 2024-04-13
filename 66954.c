void *jsvMalloc(size_t size) {
  /** Allocate flat string, return pointer to its first element.
   * As we drop the pointer here, it's left locked. jsvGetFlatStringPointer
   * is also safe if 0 is passed in.  */
  JsVar *flatStr = jsvNewFlatStringOfLength((unsigned int)size);
  if (!flatStr) {
    jsErrorFlags |= JSERR_LOW_MEMORY;
    while (jsiFreeMoreMemory());
    jsvGarbageCollect();
    flatStr = jsvNewFlatStringOfLength((unsigned int)size);
  }
  void *p = (void*)jsvGetFlatStringPointer(flatStr);
  if (p) {
    memset(p,0,size);
  }
  return p;
}
