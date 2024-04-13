JsVar *jsvNewArrayBufferWithData(JsVarInt length, unsigned char *data) {
  assert(data);
  JsVar *dst = 0;
  JsVar *arr = jsvNewArrayBufferWithPtr((unsigned int)length, (char**)&dst);
  if (!dst) {
    jsvUnLock(arr);
    return 0;
  }
  memcpy(dst, data, (size_t)length);
  return arr;
}
