JsVar *jsvGetArrayItem(const JsVar *arr, JsVarInt index) {
  return jsvSkipNameAndUnLock(jsvGetArrayIndex(arr,index));
}
