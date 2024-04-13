ModuleExport void UnregisterTTFImage(void)
{
  (void) UnregisterMagickInfo("TTF");
  (void) UnregisterMagickInfo("TTC");
  (void) UnregisterMagickInfo("OTF");
  (void) UnregisterMagickInfo("PFA");
  (void) UnregisterMagickInfo("PFB");
  (void) UnregisterMagickInfo("PFA");
  (void) UnregisterMagickInfo("DFONT");
}
