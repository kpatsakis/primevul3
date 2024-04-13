ModuleExport size_t RegisterTILEImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("TILE");
  entry->decoder=(DecodeImageHandler *) ReadTILEImage;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->format_type=ImplicitFormatType;
  entry->description=ConstantString("Tile image with a texture");
  entry->module=ConstantString("TILE");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
