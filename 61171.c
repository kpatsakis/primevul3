ModuleExport void UnregisterJBIGImage(void)
{
  (void) UnregisterMagickInfo("BIE");
  (void) UnregisterMagickInfo("JBG");
  (void) UnregisterMagickInfo("JBIG");
}
