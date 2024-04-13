ModuleExport size_t RegisterRGFImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("RGF");
  entry->decoder=(DecodeImageHandler *) ReadRGFImage;
  entry->encoder=(EncodeImageHandler *) WriteRGFImage;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString(
    "LEGO Mindstorms EV3 Robot Graphic Format (black and white)");
  entry->module=ConstantString("RGF");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
