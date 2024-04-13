ModuleExport size_t RegisterNULLImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("NULL");
  entry->decoder=(DecodeImageHandler *) ReadNULLImage;
  entry->encoder=(EncodeImageHandler *) WriteNULLImage;
  entry->adjoin=MagickFalse;
  entry->format_type=ImplicitFormatType;
  entry->description=ConstantString("Constant image of uniform color");
  entry->module=ConstantString("NULL");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
