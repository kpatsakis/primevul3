ModuleExport size_t RegisterVICARImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("VICAR");
  entry->decoder=(DecodeImageHandler *) ReadVICARImage;
  entry->encoder=(EncodeImageHandler *) WriteVICARImage;
  entry->magick=(IsImageFormatHandler *) IsVICAR;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("VICAR rasterfile format");
  entry->module=ConstantString("VICAR");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
