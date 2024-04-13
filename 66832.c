JsVarInt jsvArrayPush(JsVar *arr, JsVar *value) {
  assert(jsvIsArray(arr));
  JsVarInt index = jsvGetArrayLength(arr);
  JsVar *idx = jsvMakeIntoVariableName(jsvNewFromInteger(index), value);
  if (!idx) return 0; // out of memory - error flag will have been set already
  jsvAddName(arr, idx);
  jsvUnLock(idx);
  return jsvGetArrayLength(arr);
}
