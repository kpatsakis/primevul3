size_t jsvGetFlatStringBlocks(const JsVar *v) {
  assert(jsvIsFlatString(v));
  return ((size_t)v->varData.integer+sizeof(JsVar)-1) / sizeof(JsVar);
}
