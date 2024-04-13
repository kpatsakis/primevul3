char *jsvGetDataPointer(JsVar *v, size_t *len) {
  assert(len);
  if (jsvIsArrayBuffer(v)) {
    /* Arraybuffers generally use some kind of string to store their data.
     * Find it, then call ourselves again to figure out if we can get a
     * raw pointer to it.  */
    JsVar *d = jsvGetArrayBufferBackingString(v);
    char *r = jsvGetDataPointer(d, len);
    jsvUnLock(d);
    if (r) {
      r += v->varData.arraybuffer.byteOffset;
      *len = v->varData.arraybuffer.length;
    }
    return r;
  }
  if (jsvIsNativeString(v)) {
    *len = v->varData.nativeStr.len;
    return (char*)v->varData.nativeStr.ptr;
  }
  if (jsvIsFlatString(v)) {
    *len = jsvGetStringLength(v);
    return jsvGetFlatStringPointer(v);
  }
  return 0;
}
