char *jsvGetFlatStringPointer(JsVar *v) {
  assert(jsvIsFlatString(v));
  if (!jsvIsFlatString(v)) return 0;
  return (char*)(v+1); // pointer to the next JsVar
}
