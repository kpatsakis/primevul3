void jsvKill() {
#ifdef RESIZABLE_JSVARS
  unsigned int i;
  for (i=0;i<jsVarsSize>>JSVAR_BLOCK_SHIFT;i++)
    free(jsVarBlocks[i]);
  free(jsVarBlocks);
  jsVarBlocks = 0;
  jsVarsSize = 0;
#endif
}
