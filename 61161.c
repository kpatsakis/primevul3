ModuleExport size_t RegisterHDRImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("HDR");
  entry->decoder=(DecodeImageHandler *) ReadHDRImage;
  entry->encoder=(EncodeImageHandler *) WriteHDRImage;
  entry->description=ConstantString("Radiance RGBE image format");
  entry->module=ConstantString("HDR");
  entry->magick=(IsImageFormatHandler *) IsHDR;
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
