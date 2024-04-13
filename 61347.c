static MagickBooleanType WriteVIPSImage(const ImageInfo *image_info,
  Image *image)
{
  const char
    *metadata;

  MagickBooleanType
    status;

  register const IndexPacket
    *indexes;

  register const PixelPacket
    *p;

  register ssize_t
    x;

  ssize_t
    y;

  unsigned int
    channels;

  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);

  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  if (image->endian == LSBEndian)
    (void) WriteBlobLSBLong(image,VIPS_MAGIC_LSB);
  else
    (void) WriteBlobLSBLong(image,VIPS_MAGIC_MSB);
  (void) WriteBlobLong(image,(unsigned int) image->columns);
  (void) WriteBlobLong(image,(unsigned int) image->rows);
  (void) SetImageStorageClass(image,DirectClass);
  channels=image->matte ? 4 : 3;
  if (IsGrayImage(image,&image->exception) != MagickFalse)
    channels=image->matte ? 2 : 1;
  else if (image->colorspace == CMYKColorspace)
    channels=image->matte ? 5 : 4;
  (void) WriteBlobLong(image,channels);
  (void) WriteBlobLong(image,0);
  if (image->depth == 16)
    (void) WriteBlobLong(image,(unsigned int) VIPSBandFormatUSHORT);
  else
    {
      image->depth=8;
      (void) WriteBlobLong(image,(unsigned int) VIPSBandFormatUCHAR);
    }
  (void) WriteBlobLong(image,VIPSCodingNONE);
  switch(image->colorspace)
  {
    case CMYKColorspace:
      (void) WriteBlobLong(image,VIPSTypeCMYK);
      break;
    case GRAYColorspace:
      if (image->depth == 16)
        (void) WriteBlobLong(image, VIPSTypeGREY16);
      else
        (void) WriteBlobLong(image, VIPSTypeB_W);
      break;
    case RGBColorspace:
      if (image->depth == 16)
        (void) WriteBlobLong(image, VIPSTypeRGB16);
      else
        (void) WriteBlobLong(image, VIPSTypeRGB);
      break;
    default:
    case sRGBColorspace:
      (void) SetImageColorspace(image,sRGBColorspace);
      (void) WriteBlobLong(image,VIPSTypesRGB);
      break;
  }
  if (image->units == PixelsPerCentimeterResolution)
    {
      (void) WriteBlobFloat(image,(image->x_resolution / 10));
      (void) WriteBlobFloat(image,(image->y_resolution / 10));
    }
  else if (image->units == PixelsPerInchResolution)
    {
      (void) WriteBlobFloat(image,(image->x_resolution / 25.4));
      (void) WriteBlobFloat(image,(image->y_resolution / 25.4));
    }
  else
    {
      (void) WriteBlobLong(image,0);
      (void) WriteBlobLong(image,0);
    }
  /*
    Legacy, Offsets, Future
  */
  for (y=0; y < 24; y++)
    (void) WriteBlobByte(image,0);
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    indexes=GetVirtualIndexQueue(image);
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      WriteVIPSPixel(image,GetPixelRed(p));
      if (channels == 2)
        WriteVIPSPixel(image,GetPixelAlpha(p));
      else
        {
          WriteVIPSPixel(image,GetPixelGreen(p));
          WriteVIPSPixel(image,GetPixelBlue(p));
          if (channels >= 4)
            {
              if (image->colorspace == CMYKColorspace)
                WriteVIPSPixel(image,GetPixelIndex(indexes+x));
              else
                WriteVIPSPixel(image,GetPixelAlpha(p));
            }
          else if (channels == 5)
            {
               WriteVIPSPixel(image,GetPixelIndex(indexes+x));
               WriteVIPSPixel(image,GetPixelAlpha(p));
            }
        }
      p++;
    }
  }
  metadata=GetImageProperty(image,"vips:metadata");
  if (metadata != (const char*) NULL)
    WriteBlobString(image,metadata);
  (void) CloseBlob(image);
  return(status);
}
