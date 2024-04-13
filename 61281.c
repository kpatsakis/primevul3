static MagickBooleanType IsSVG(const unsigned char *magick,const size_t length)
{
  if (length < 4)
    return(MagickFalse);
  if (LocaleNCompare((const char *) magick,"?xml",4) == 0)
    return(MagickTrue);
  return(MagickFalse);
}
