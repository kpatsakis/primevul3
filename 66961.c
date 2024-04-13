JsVar *jsvNewArray(JsVar **elements, int elementCount) {
  JsVar *arr = jsvNewEmptyArray();
  if (!arr) return 0;
  int i;
  for (i=0;i<elementCount;i++)
    jsvArrayPush(arr, elements[i]);
  return arr;
}
