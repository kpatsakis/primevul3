static MagickBooleanType IsTTF(const unsigned char *magick,const size_t length)
{
  if (length < 5)
    return(MagickFalse);
  if (((int) magick[0] == 0x00) && ((int) magick[1] == 0x01) &&
      ((int) magick[2] == 0x00) && ((int) magick[3] == 0x00) &&
      ((int) magick[4] == 0x00))
    return(MagickTrue);
  return(MagickFalse);
}
