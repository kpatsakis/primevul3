static MagickBooleanType WriteMONOImage(const ImageInfo *image_info,
  Image *image)
{
  MagickBooleanType
    status;

  register const PixelPacket
    *p;

  register ssize_t
    x;

  size_t
    bit,
    byte;

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
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  (void) TransformImageColorspace(image,sRGBColorspace);
  /*
    Convert image to a bi-level image.
  */
  (void) SetImageType(image,BilevelType);
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    bit=0;
    byte=0;
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      byte>>=1;
      if (image->endian == LSBEndian)
        {
          if (GetPixelLuma(image,p) < (QuantumRange/2.0))
            byte|=0x80;
        }
      else
        if (GetPixelLuma(image,p) >= (QuantumRange/2.0))
          byte|=0x80;
      bit++;
      if (bit == 8)
        {
          (void) WriteBlobByte(image,(unsigned char) byte);
          bit=0;
          byte=0;
        }
      p++;
    }
    if (bit != 0)
      (void) WriteBlobByte(image,(unsigned char) (byte >> (8-bit)));
    status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }
  (void) CloseBlob(image);
  return(MagickTrue);
}
