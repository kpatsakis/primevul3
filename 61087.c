ModuleExport void UnregisterCMYKImage(void)
{
  (void) UnregisterMagickInfo("CMYK");
  (void) UnregisterMagickInfo("CMYKA");
}
