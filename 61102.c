static MagickBooleanType IsDJVU(const unsigned char *magick,const size_t length)
{
  if (length < 8)
    return(MagickFalse);
  if (memcmp(magick,"AT&TFORM",8) == 0)
    return(MagickTrue);
  return(MagickFalse);
}
