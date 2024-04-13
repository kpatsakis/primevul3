JsVarFloat wrapAround(JsVarFloat val, JsVarFloat size) {
  val = val / size;
  val = val - (int)val;
  return val * size;
}
