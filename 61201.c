static MagickBooleanType WriteOTBImage(const ImageInfo *image_info,Image *image)
{
#define SetBit(a,i,set) \
  a=(unsigned char) ((set) ? (a) | (1L << (i)) : (a) & ~(1L << (i)))

  MagickBooleanType
    status;

  register const PixelPacket
    *p;

  register ssize_t
    x;

  ssize_t
    y;

  unsigned char
    bit,
    byte,
    info;

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
  (void) TransformImageColorspace(image,sRGBColorspace);
  /*
    Convert image to a bi-level image.
  */
  (void) SetImageType(image,BilevelType);
  info=0;
  if ((image->columns >= 256) || (image->rows >= 256))
    SetBit(info,4,1);
  (void) WriteBlobByte(image,info);
  if ((image->columns >= 256) || (image->rows >= 256))
    {
      (void) WriteBlobMSBShort(image,(unsigned short) image->columns);
      (void) WriteBlobMSBShort(image,(unsigned short) image->rows);
    }
  else
    {
      (void) WriteBlobByte(image,(unsigned char) image->columns);
      (void) WriteBlobByte(image,(unsigned char) image->rows);
    }
  (void) WriteBlobByte(image,1);  /* depth */
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    bit=0;
    byte=0;
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      if (GetPixelLuma(image,p) < (QuantumRange/2.0))
        byte|=0x1 << (7-bit);
      bit++;
      if (bit == 8)
        {
          (void) WriteBlobByte(image,byte);
          bit=0;
          byte=0;
        }
      p++;
    }
    if (bit != 0)
      (void) WriteBlobByte(image,byte);
    if (image->previous == (Image *) NULL)
      {
        status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
          image->rows);
        if (status == MagickFalse)
          break;
      }
  }
  (void) CloseBlob(image);
  return(MagickTrue);
}
