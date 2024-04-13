ModuleExport void UnregisterBGRImage(void)
{
  (void) UnregisterMagickInfo("BGRA");
  (void) UnregisterMagickInfo("BGR");
}
