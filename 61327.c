static MagickBooleanType IsPFA(const unsigned char *magick,const size_t length)
{
  if (length < 14)
    return(MagickFalse);
  if (LocaleNCompare((char *) magick,"%!PS-AdobeFont",14) == 0)
    return(MagickTrue);
  return(MagickFalse);
}
