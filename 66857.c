size_t jsvGetArrayBufferLength(const JsVar *arrayBuffer) {
  assert(jsvIsArrayBuffer(arrayBuffer));
  return arrayBuffer->varData.arraybuffer.length;
}
