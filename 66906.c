void jsvInit() {
#ifdef RESIZABLE_JSVARS
  jsVarsSize = JSVAR_BLOCK_SIZE;
  jsVarBlocks = malloc(sizeof(JsVar*)); // just 1
  jsVarBlocks[0] = malloc(sizeof(JsVar) * JSVAR_BLOCK_SIZE);
#endif

  jsVarFirstEmpty = jsvInitJsVars(1/*first*/, jsVarsSize);
  jsvSoftInit();
}
