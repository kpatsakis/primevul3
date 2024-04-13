JsVar *jsvGetFlatStringFromPointer(char *v) {
  JsVar *secondVar = (JsVar*)v;
  JsVar *flatStr = secondVar-1;
  assert(jsvIsFlatString(flatStr));
  return flatStr;
}
