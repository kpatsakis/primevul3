ModuleExport void UnregisterJPEGImage(void)
{
  (void) UnregisterMagickInfo("PJPG");
  (void) UnregisterMagickInfo("JPEG");
  (void) UnregisterMagickInfo("JPG");
  (void) UnregisterMagickInfo("JPE");
}
