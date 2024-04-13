static Image *ReadSIXELImage(const ImageInfo *image_info,ExceptionInfo *exception)
{
  char
    *sixel_buffer;

  Image
    *image;

  MagickBooleanType
    status;

  register char
    *p;

  register IndexPacket
    *indexes;

  register ssize_t
    x;

  register PixelPacket
    *r;

  size_t
    length;

  ssize_t
    i,
    j,
    y;

  unsigned char
    *sixel_pixels,
    *sixel_palette;

  /*
    Open image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  if (image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickSignature);
  image=AcquireImage(image_info);
  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == MagickFalse)
    {
      image=DestroyImageList(image);
      return((Image *) NULL);
    }
  /*
    Read SIXEL file.
  */
  length=MaxTextExtent;
  sixel_buffer=(char *) AcquireQuantumMemory((size_t) length,sizeof(*sixel_buffer));
  p=sixel_buffer;
  if (sixel_buffer != (char *) NULL)
    while (ReadBlobString(image,p) != (char *) NULL)
    {
      if ((*p == '#') && ((p == sixel_buffer) || (*(p-1) == '\n')))
        continue;
      if ((*p == '}') && (*(p+1) == ';'))
        break;
      p+=strlen(p);
      if ((size_t) (p-sixel_buffer+MaxTextExtent) < length)
        continue;
      length<<=1;
      sixel_buffer=(char *) ResizeQuantumMemory(sixel_buffer,length+MaxTextExtent,
        sizeof(*sixel_buffer));
      if (sixel_buffer == (char *) NULL)
        break;
      p=sixel_buffer+strlen(sixel_buffer);
    }
