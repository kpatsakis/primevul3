static MagickBooleanType WriteUYVYImage(const ImageInfo *image_info,
  Image *image)
{
  MagickPixelPacket
    pixel;

  Image
    *uyvy_image;

  MagickBooleanType
    full,
    status;

  register const PixelPacket
    *p;

  register ssize_t
    x;

  ssize_t
    y;

  /*
    Open output image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  if ((image->columns % 2) != 0)
    image->columns++;
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  /*
    Accumulate two pixels, then output.
  */
  uyvy_image=CloneImage(image,0,0,MagickTrue,&image->exception);
  if (uyvy_image == (Image *) NULL)
    ThrowWriterException(ResourceLimitError,image->exception.reason);
  (void) TransformImageColorspace(uyvy_image,YCbCrColorspace);
  full=MagickFalse;
  (void) ResetMagickMemory(&pixel,0,sizeof(MagickPixelPacket));
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    p=GetVirtualPixels(uyvy_image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      if (full != MagickFalse)
        {
          pixel.green=(pixel.green+GetPixelGreen(p))/2;
          pixel.blue=(pixel.blue+GetPixelBlue(p))/2;
          (void) WriteBlobByte(image,ScaleQuantumToChar((Quantum) pixel.green));
          (void) WriteBlobByte(image,ScaleQuantumToChar((Quantum) pixel.red));
          (void) WriteBlobByte(image,ScaleQuantumToChar((Quantum) pixel.blue));
          (void) WriteBlobByte(image,ScaleQuantumToChar(
             GetPixelRed(p)));
        }
      pixel.red=(double) GetPixelRed(p);
      pixel.green=(double) GetPixelGreen(p);
      pixel.blue=(double) GetPixelBlue(p);
      full=full == MagickFalse ? MagickTrue : MagickFalse;
      p++;
    }
    status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }
  uyvy_image=DestroyImage(uyvy_image);
  (void) CloseBlob(image);
  return(MagickTrue);
}
