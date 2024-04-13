bool jsvMoreFreeVariablesThan(unsigned int vars) {
  if (!vars) return false;
  JsVarRef r = jsVarFirstEmpty;
  while (r) {
    if (!vars--) return true;
    r = jsvGetNextSibling(jsvGetAddressOf(r));
  }
  return false;
}
