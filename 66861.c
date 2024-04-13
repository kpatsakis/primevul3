JsVarInt jsvGetArrayLength(const JsVar *arr) {
  if (!arr) return 0;
  assert(jsvIsArray(arr));
  return arr->varData.integer;
}
