ModuleExport size_t RegisterRLAImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("RLA");
  entry->decoder=(DecodeImageHandler *) ReadRLAImage;
  entry->adjoin=MagickFalse;
  entry->seekable_stream=MagickTrue;
  entry->description=ConstantString("Alias/Wavefront image");
  entry->module=ConstantString("RLA");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
