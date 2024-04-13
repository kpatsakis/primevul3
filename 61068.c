ModuleExport size_t RegisterBGRImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("BGR");
  entry->decoder=(DecodeImageHandler *) ReadBGRImage;
  entry->encoder=(EncodeImageHandler *) WriteBGRImage;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("Raw blue, green, and red samples");
  entry->module=ConstantString("BGR");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("BGRA");
  entry->decoder=(DecodeImageHandler *) ReadBGRImage;
  entry->encoder=(EncodeImageHandler *) WriteBGRImage;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("Raw blue, green, red, and alpha samples");
  entry->module=ConstantString("BGR");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
