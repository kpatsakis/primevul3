void jsvDumpFreeList() {
  JsVarRef ref = jsVarFirstEmpty;
  int n = 0;
  while (ref) {
    jsiConsolePrintf("%5d ", (int)ref);
    if (++n >= 16) {
      n = 0;
      jsiConsolePrintf("\n");
    }
    JsVar *v = jsvGetAddressOf(ref);
    ref = jsvGetNextSibling(v);
  }
  jsiConsolePrintf("\n");
}
