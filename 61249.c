ModuleExport size_t RegisterSCRImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("SCR");
  entry->decoder=(DecodeImageHandler *) ReadSCRImage;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("ZX-Spectrum SCREEN$");
  entry->module=ConstantString("SCR");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
