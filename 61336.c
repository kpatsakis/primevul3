static MagickBooleanType IsVICAR(const unsigned char *magick,
  const size_t length)
{
  if (length < 14)
    return(MagickFalse);
  if (LocaleNCompare((const char *) magick,"LBLSIZE",7) == 0)
    return(MagickTrue);
  if (LocaleNCompare((const char *) magick,"NJPL1I",6) == 0)
    return(MagickTrue);
  if (LocaleNCompare((const char *) magick,"PDS_VERSION_ID",14) == 0)
    return(MagickTrue);
  return(MagickFalse);
}
