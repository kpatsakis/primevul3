ModuleExport size_t RegisterCMYKImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("CMYK");
  entry->decoder=(DecodeImageHandler *) ReadCMYKImage;
  entry->encoder=(EncodeImageHandler *) WriteCMYKImage;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("Raw cyan, magenta, yellow, and black "
    "samples");
  entry->module=ConstantString("CMYK");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("CMYKA");
  entry->decoder=(DecodeImageHandler *) ReadCMYKImage;
  entry->encoder=(EncodeImageHandler *) WriteCMYKImage;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("Raw cyan, magenta, yellow, black, and "
    "alpha samples");
  entry->module=ConstantString("CMYK");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
