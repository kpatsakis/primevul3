static JsVarRef jsvInitJsVars(JsVarRef start, unsigned int count) {
  JsVarRef i;
  for (i=start;i<start+count;i++) {
    JsVar *v = jsvGetAddressOf(i);
    v->flags = JSV_UNUSED;
    jsvSetNextSibling(v, (JsVarRef)(i+1)); // link to next
  }
  jsvSetNextSibling(jsvGetAddressOf((JsVarRef)(start+count-1)), (JsVarRef)0); // set the final one to 0
  return start;
}
