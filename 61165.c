static MagickBooleanType WriteHRZImage(const ImageInfo *image_info,Image *image)
{
  Image
    *hrz_image;

  MagickBooleanType
    status;

  register const PixelPacket
    *p;

  register ssize_t
    x,
    y;

  register unsigned char
    *q;

  ssize_t
    count;

  unsigned char
    *pixels;

  /*
    Open output image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  hrz_image=ResizeImage(image,256,240,image->filter,image->blur,
    &image->exception);
  if (hrz_image == (Image *) NULL)
    return(MagickFalse);
  (void) TransformImageColorspace(hrz_image,sRGBColorspace);
  /*
    Allocate memory for pixels.
  */
  pixels=(unsigned char *) AcquireQuantumMemory((size_t) hrz_image->columns,
    3*sizeof(*pixels));
  if (pixels == (unsigned char *) NULL)
    {
      hrz_image=DestroyImage(hrz_image);
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    }
  /*
    Convert MIFF to HRZ raster pixels.
  */
  for (y=0; y < (ssize_t) hrz_image->rows; y++)
  {
    p=GetVirtualPixels(hrz_image,0,y,hrz_image->columns,1,&image->exception);
    if (p == (PixelPacket *) NULL)
      break;
    q=pixels;
    for (x=0; x < (ssize_t) hrz_image->columns; x++)
    {
      *q++=ScaleQuantumToChar(GetPixelRed(p)/4);
      *q++=ScaleQuantumToChar(GetPixelGreen(p)/4);
      *q++=ScaleQuantumToChar(GetPixelBlue(p)/4);
      p++;
    }
    count=WriteBlob(image,(size_t) (q-pixels),pixels);
    if (count != (ssize_t) (q-pixels))
      break;
    status=SetImageProgress(image,SaveImageTag,y,hrz_image->rows);
    if (status == MagickFalse)
      break;
  }
  pixels=(unsigned char *) RelinquishMagickMemory(pixels);
  hrz_image=DestroyImage(hrz_image);
  (void) CloseBlob(image);
  return(MagickTrue);
}
