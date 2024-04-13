MagickExport MagickBooleanType DiscardBlobBytes(Image *image,
  const MagickSizeType length)
{
  register MagickOffsetType
    i;

  size_t
    quantum;

  ssize_t
    count;

  unsigned char
    buffer[16384];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (length != (MagickSizeType) ((MagickOffsetType) length))
    return(MagickFalse);
  count=0;
  for (i=0; i < (MagickOffsetType) length; i+=count)
  {
    quantum=(size_t) MagickMin(length-i,sizeof(buffer));
    (void) ReadBlobStream(image,quantum,buffer,&count);
    if (count <= 0)
      {
        count=0;
        if (errno != EINTR)
          break;
      }
  }
  return(i < (MagickOffsetType) length ? MagickFalse : MagickTrue);
}
