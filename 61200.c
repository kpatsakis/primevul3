ModuleExport size_t RegisterOTBImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("OTB");
  entry->decoder=(DecodeImageHandler *) ReadOTBImage;
  entry->encoder=(EncodeImageHandler *) WriteOTBImage;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("On-the-air bitmap");
  entry->module=ConstantString("OTB");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
