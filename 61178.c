ModuleExport size_t RegisterMACImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("MAC");
  entry->decoder=(DecodeImageHandler *) ReadMACImage;
  entry->description=ConstantString("MAC Paint");
  entry->module=ConstantString("MAC");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
