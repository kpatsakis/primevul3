void jsvArrayBufferSet(JsVar *arrayBuffer, size_t idx, JsVar *value) {
  JsvArrayBufferIterator it;
  jsvArrayBufferIteratorNew(&it, arrayBuffer, idx);
  jsvArrayBufferIteratorSetValue(&it, value);
  jsvArrayBufferIteratorFree(&it);
}
