ModuleExport size_t RegisterCLIPBOARDImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("CLIPBOARD");
#if defined(MAGICKCORE_WINGDI32_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadCLIPBOARDImage;
  entry->encoder=(EncodeImageHandler *) WriteCLIPBOARDImage;
#endif
  entry->adjoin=MagickFalse;
  entry->format_type=ImplicitFormatType;
  entry->description=ConstantString("The system clipboard");
  entry->module=ConstantString("CLIPBOARD");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
