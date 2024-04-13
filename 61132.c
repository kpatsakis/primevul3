static MagickBooleanType IsEXR(const unsigned char *magick,const size_t length)
{
  if (length < 4)
    return(MagickFalse);
  if (memcmp(magick,"\166\057\061\001",4) == 0)
    return(MagickTrue);
  return(MagickFalse);
}
