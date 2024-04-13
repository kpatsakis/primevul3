ModuleExport size_t RegisterWBMPImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("WBMP");
  entry->decoder=(DecodeImageHandler *) ReadWBMPImage;
  entry->encoder=(EncodeImageHandler *) WriteWBMPImage;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Wireless Bitmap (level 0) image");
  entry->module=ConstantString("WBMP");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
