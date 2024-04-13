static MagickBooleanType IsSCT(const unsigned char *magick,const size_t length)
{
  if (length < 2)
    return(MagickFalse);
  if (LocaleNCompare((const char *) magick,"CT",2) == 0)
    return(MagickTrue);
  return(MagickFalse);
}
