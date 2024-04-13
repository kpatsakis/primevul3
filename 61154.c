ModuleExport size_t RegisterGRAYImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("GRAY");
  entry->decoder=(DecodeImageHandler *) ReadGRAYImage;
  entry->encoder=(EncodeImageHandler *) WriteGRAYImage;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("Raw gray samples");
  entry->module=ConstantString("GRAY");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
