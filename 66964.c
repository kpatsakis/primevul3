JsVar *jsvNewArrayBufferWithPtr(unsigned int length, char **ptr) {
  assert(ptr);
  *ptr=0;
  JsVar *backingString = jsvNewFlatStringOfLength(length);
  if (!backingString) return 0;
  JsVar *arr = jsvNewArrayBufferFromString(backingString, length);
  if (!arr) {
    jsvUnLock(backingString);
    return 0;
  }
  *ptr = jsvGetFlatStringPointer(backingString);
  jsvUnLock(backingString);
  return arr;
}
