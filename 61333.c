ModuleExport size_t RegisterUYVYImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("PAL");
  entry->decoder=(DecodeImageHandler *) ReadUYVYImage;
  entry->encoder=(EncodeImageHandler *) WriteUYVYImage;
  entry->adjoin=MagickFalse;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("16bit/pixel interleaved YUV");
  entry->module=ConstantString("UYVY");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("UYVY");
  entry->decoder=(DecodeImageHandler *) ReadUYVYImage;
  entry->encoder=(EncodeImageHandler *) WriteUYVYImage;
  entry->adjoin=MagickFalse;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("16bit/pixel interleaved YUV");
  entry->module=ConstantString("UYVY");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
