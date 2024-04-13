JsVarInt jsvArrayPushAndUnLock(JsVar *arr, JsVar *value) {
  JsVarInt l = jsvArrayPush(arr, value);
  jsvUnLock(value);
  return l;
}
