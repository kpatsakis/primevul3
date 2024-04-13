ModuleExport size_t RegisterFAXImage(void)
{
  MagickInfo
    *entry;

  static const char
    *Note=
    {
      "FAX machines use non-square pixels which are 1.5 times wider than\n"
      "they are tall but computer displays use square pixels, therefore\n"
      "FAX images may appear to be narrow unless they are explicitly\n"
      "resized using a geometry of \"150x100%\".\n"
    };

  entry=SetMagickInfo("FAX");
  entry->decoder=(DecodeImageHandler *) ReadFAXImage;
  entry->encoder=(EncodeImageHandler *) WriteFAXImage;
  entry->magick=(IsImageFormatHandler *) IsFAX;
  entry->description=ConstantString("Group 3 FAX");
  entry->note=ConstantString(Note);
  entry->module=ConstantString("FAX");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("G3");
  entry->decoder=(DecodeImageHandler *) ReadFAXImage;
  entry->encoder=(EncodeImageHandler *) WriteFAXImage;
  entry->magick=(IsImageFormatHandler *) IsFAX;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Group 3 FAX");
  entry->module=ConstantString("FAX");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
