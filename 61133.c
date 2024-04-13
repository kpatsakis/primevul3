ModuleExport size_t RegisterEXRImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("EXR");
#if defined(MAGICKCORE_OPENEXR_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadEXRImage;
  entry->encoder=(EncodeImageHandler *) WriteEXRImage;
#endif
  entry->magick=(IsImageFormatHandler *) IsEXR;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("High Dynamic-range (HDR)");
  entry->blob_support=MagickFalse;
  entry->module=ConstantString("EXR");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
