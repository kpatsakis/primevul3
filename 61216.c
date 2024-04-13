ModuleExport size_t RegisterPDBImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("PDB");
  entry->decoder=(DecodeImageHandler *) ReadPDBImage;
  entry->encoder=(EncodeImageHandler *) WritePDBImage;
  entry->magick=(IsImageFormatHandler *) IsPDB;
  entry->description=ConstantString("Palm Database ImageViewer Format");
  entry->module=ConstantString("PDB");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
