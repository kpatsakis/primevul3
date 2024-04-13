JsVar *jsvGetArrayBufferBackingString(JsVar *arrayBuffer) {
  jsvLockAgain(arrayBuffer);
  while (jsvIsArrayBuffer(arrayBuffer)) {
    JsVar *s = jsvLock(jsvGetFirstChild(arrayBuffer));
    jsvUnLock(arrayBuffer);
    arrayBuffer = s;
  }
  assert(jsvIsString(arrayBuffer));
  return arrayBuffer;
}
