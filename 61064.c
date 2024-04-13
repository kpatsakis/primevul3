static MagickBooleanType WriteARTImage(const ImageInfo *image_info,Image *image)
{
  MagickBooleanType
    status;

  QuantumInfo
    *quantum_info;

  register const PixelPacket
    *p;

  size_t
    length;

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
  if ((image->columns > 65535UL) || (image->rows > 65535UL))
    ThrowWriterException(ImageError,"WidthOrHeightExceedsLimit");
  image->endian=MSBEndian;
  image->depth=1;
  (void) WriteBlobLSBShort(image,0);
  (void) WriteBlobLSBShort(image,(unsigned short) image->columns);
  (void) WriteBlobLSBShort(image,0);
  (void) WriteBlobLSBShort(image,(unsigned short) image->rows);
  (void) TransformImageColorspace(image,sRGBColorspace);
  length=(image->columns+7)/8;
  pixels=(unsigned char *) AcquireQuantumMemory(length,sizeof(*pixels));
  if (pixels == (unsigned char *) NULL)
    ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
  /*
    Convert image to a bi-level image.
  */
  (void) SetImageType(image,BilevelType);
  quantum_info=AcquireQuantumInfo(image_info,image);
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    (void) ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
      GrayQuantum,pixels,&image->exception);
    count=WriteBlob(image,length,pixels);
    if (count != (ssize_t) length)
      ThrowWriterException(CorruptImageError,"UnableToWriteImageData");
    count=WriteBlob(image,(size_t) (-(ssize_t) length) & 0x01,pixels);
    status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }
  quantum_info=DestroyQuantumInfo(quantum_info);
  pixels=(unsigned char *) RelinquishMagickMemory(pixels);
  (void) CloseBlob(image);
  return(MagickTrue);
}
