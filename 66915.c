bool jsvIsEqual(JsVar *a, JsVar *b) {
  if (jsvIsBasic(a) && jsvIsBasic(b))
    return jsvIsBasicVarEqual(a,b);
  return jsvGetRef(a)==jsvGetRef(b);
}
