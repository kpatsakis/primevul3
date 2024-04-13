JsVar *jsvNewArrayBufferFromString(JsVar *str, unsigned int lengthOrZero) {
  JsVar *arr = jsvNewWithFlags(JSV_ARRAYBUFFER);
  if (!arr) return 0;
  jsvSetFirstChild(arr, jsvGetRef(jsvRef(str)));
  arr->varData.arraybuffer.type = ARRAYBUFFERVIEW_ARRAYBUFFER;
  assert(arr->varData.arraybuffer.byteOffset == 0);
  if (lengthOrZero==0) lengthOrZero = (unsigned int)jsvGetStringLength(str);
  arr->varData.arraybuffer.length = (unsigned short)lengthOrZero;
  return arr;
}
