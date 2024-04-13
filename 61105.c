ModuleExport size_t RegisterDJVUImage(void)
{
  char
    version[MaxTextExtent];

  MagickInfo
    *entry;

  static const char
    *DJVUNote =
    {
      "See http://www.djvuzone.org/ for details about the DJVU format.  The\n"
      "DJVU 1.2 specification is available there and at\n"
      "ftp://swrinde.nde.swri.edu/pub/djvu/documents/."
    };

  *version='\0';
#if defined(DJVU_LIBDJVU_VER_STRING)
  (void) ConcatenateMagickString(version,"libdjvu ",MaxTextExtent);
  (void) ConcatenateMagickString(version,DJVU_LIBDJVU_VER_STRING,MaxTextExtent);
#endif
  entry=SetMagickInfo("DJVU");
#if defined(MAGICKCORE_DJVU_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadDJVUImage;
#endif
  entry->raw=MagickTrue;
  entry->magick=(IsImageFormatHandler *) IsDJVU;
  entry->adjoin=MagickFalse;
  entry->thread_support=MagickTrue;
  entry->description=AcquireString("Dj vu");
  entry->module=AcquireString("DJVU");
  if (*version != '\0')
    entry->version=AcquireString(version);
  entry->note=AcquireString(DJVUNote);
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
