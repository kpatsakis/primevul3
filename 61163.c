ModuleExport size_t RegisterHRZImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("HRZ");
  entry->decoder=(DecodeImageHandler *) ReadHRZImage;
  entry->encoder=(EncodeImageHandler *) WriteHRZImage;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Slow Scan TeleVision");
  entry->module=ConstantString("HRZ");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
