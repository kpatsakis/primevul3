static MagickBooleanType WriteAAIImage(const ImageInfo *image_info,Image *image)
{
  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  register const PixelPacket
    *restrict p;

  register ssize_t
    x;

  register unsigned char
    *restrict q;

  ssize_t
    count,
    y;

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
  scene=0;
  do
  {
    /*
      Write AAI header.
    */
    (void) TransformImageColorspace(image,sRGBColorspace);
    (void) WriteBlobLSBLong(image,(unsigned int) image->columns);
    (void) WriteBlobLSBLong(image,(unsigned int) image->rows);
    /*
      Allocate memory for pixels.
    */
    pixels=(unsigned char *) AcquireQuantumMemory((size_t) image->columns,
      4*sizeof(*pixels));
    if (pixels == (unsigned char *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    /*
      Convert MIFF to AAI raster pixels.
    */
    for (y=0; y < (ssize_t) image->rows; y++)
    {
      p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
      if (p == (PixelPacket *) NULL)
        break;
      q=pixels;
      for (x=0; x < (ssize_t) image->columns; x++)
      {
        *q++=ScaleQuantumToChar(GetPixelBlue(p));
        *q++=ScaleQuantumToChar(GetPixelGreen(p));
        *q++=ScaleQuantumToChar(GetPixelRed(p));
        *q=ScaleQuantumToChar((Quantum) (QuantumRange-(image->matte !=
          MagickFalse ? GetPixelOpacity(p) : OpaqueOpacity)));
        if (*q == 255)
          *q=254;
        p++;
        q++;
      }
      count=WriteBlob(image,(size_t) (q-pixels),pixels);
      if (count != (ssize_t) (q-pixels))
        break;
      if (image->previous == (Image *) NULL)
        {
          status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
            image->rows);
          if (status == MagickFalse)
            break;
        }
    }
    pixels=(unsigned char *) RelinquishMagickMemory(pixels);
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,
      GetImageListLength(image));
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  (void) CloseBlob(image);
  return(MagickTrue);
}
