ModuleExport size_t RegisterHALDImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("HALD");
  entry->decoder=(DecodeImageHandler *) ReadHALDImage;
  entry->adjoin=MagickFalse;
  entry->format_type=ImplicitFormatType;
  entry->raw=MagickTrue;
  entry->endian_support=MagickTrue;
  entry->description=ConstantString("Identity Hald color lookup table image");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
