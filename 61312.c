static MagickBooleanType ReadProfile(Image *image,const char *name,
  unsigned char *datum,ssize_t length)
{
  MagickBooleanType
    status;

  register ssize_t
    i;

  StringInfo
    *profile;

  if (length < 4)
    return(MagickFalse);
  i=0;
  if ((LocaleCompare(name,"icc") != 0) && (LocaleCompare(name,"xmp") != 0))
    {
      for (i=0; i < (length-4); i+=2)
        if (LocaleNCompare((char *) (datum+i),"8BIM",4) == 0)
          break;
      if (i == length)
        length-=i;
      else
        i=0;
      if (length < 4)
        return(MagickFalse);
    }
  profile=BlobToStringInfo(datum+i,(size_t) length);
  if (profile == (StringInfo *) NULL)
    ThrowBinaryException(ResourceLimitError,"MemoryAllocationFailed",
      image->filename);
  status=SetImageProfile(image,name,profile);
  profile=DestroyStringInfo(profile);
  if (status == MagickFalse)
    ThrowBinaryException(ResourceLimitError,"MemoryAllocationFailed",
      image->filename);
  return(MagickTrue);
}
