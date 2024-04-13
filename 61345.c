ModuleExport size_t RegisterVIPSImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("VIPS");
  entry->decoder=(DecodeImageHandler *) ReadVIPSImage;
  entry->encoder=(EncodeImageHandler *) WriteVIPSImage;
  entry->magick=(IsImageFormatHandler *) IsVIPS;
  entry->description=ConstantString("VIPS image");
  entry->endian_support=MagickTrue;
  entry->module=ConstantString("VIPS");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
