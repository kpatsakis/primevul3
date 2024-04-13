ModuleExport size_t RegisterSCTImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("SCT");
  entry->decoder=(DecodeImageHandler *) ReadSCTImage;
  entry->magick=(IsImageFormatHandler *) IsSCT;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Scitex HandShake");
  entry->module=ConstantString("SCT");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
