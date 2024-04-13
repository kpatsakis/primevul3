static MagickBooleanType WriteJBIGImage(const ImageInfo *image_info,
  Image *image)
{
  double
    version;

  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  MemoryInfo
    *pixel_info;

  register const PixelPacket
    *p;

  register ssize_t
    x;

  register unsigned char
    *q;

  size_t
    number_packets;

  ssize_t
    y;

  struct jbg_enc_state
    jbig_info;

  unsigned char
    bit,
    byte,
    *pixels;

  /*
    Open image file.
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
  version=StringToDouble(JBG_VERSION,(char **) NULL);
  scene=0;
  do
  {
    /*
      Allocate pixel data.
    */
    (void) TransformImageColorspace(image,sRGBColorspace);
    number_packets=(image->columns+7)/8;
    pixel_info=AcquireVirtualMemory(number_packets,image->rows*sizeof(*pixels));
    if (pixel_info == (MemoryInfo *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    pixels=(unsigned char *) GetVirtualMemoryBlob(pixel_info);
    /*
      Convert pixels to a bitmap.
    */
    (void) SetImageType(image,BilevelType);
    q=pixels;
    for (y=0; y < (ssize_t) image->rows; y++)
    {
      p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
      if (p == (const PixelPacket *) NULL)
        break;
      bit=0;
      byte=0;
      for (x=0; x < (ssize_t) image->columns; x++)
      {
        byte<<=1;
        if (GetPixelLuma(image,p) < (QuantumRange/2.0))
          byte|=0x01;
        bit++;
        if (bit == 8)
          {
            *q++=byte;
            bit=0;
            byte=0;
          }
        p++;
      }
      if (bit != 0)
        *q++=byte << (8-bit);
      if (image->previous == (Image *) NULL)
        {
          status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
            image->rows);
          if (status == MagickFalse)
            break;
        }
    }
    /*
      Initialize JBIG info structure.
    */
    jbg_enc_init(&jbig_info,(unsigned long) image->columns,(unsigned long)
      image->rows,1,&pixels,(void (*)(unsigned char *,size_t,void *))
      JBIGEncode,image);
    if (image_info->scene != 0)
      jbg_enc_layers(&jbig_info,(int) image_info->scene);
    else
      {
        size_t
          x_resolution,
          y_resolution;

        x_resolution=640;
        y_resolution=480;
        if (image_info->density != (char *) NULL)
          {
            GeometryInfo
              geometry_info;

            MagickStatusType
              flags;

            flags=ParseGeometry(image_info->density,&geometry_info);
            x_resolution=geometry_info.rho;
            y_resolution=geometry_info.sigma;
            if ((flags & SigmaValue) == 0)
              y_resolution=x_resolution;
          }
        if (image->units == PixelsPerCentimeterResolution)
          {
            x_resolution=(size_t) (100.0*2.54*x_resolution+0.5)/100.0;
            y_resolution=(size_t) (100.0*2.54*y_resolution+0.5)/100.0;
          }
        (void) jbg_enc_lrlmax(&jbig_info,(unsigned long) x_resolution,
          (unsigned long) y_resolution);
      }
    (void) jbg_enc_lrange(&jbig_info,-1,-1);
    jbg_enc_options(&jbig_info,JBG_ILEAVE | JBG_SMID,JBG_TPDON | JBG_TPBON |
      JBG_DPON,version < 1.6 ? -1 : 0,-1,-1);
    /*
      Write JBIG image.
    */
    jbg_enc_out(&jbig_info);
    jbg_enc_free(&jbig_info);
    pixel_info=RelinquishVirtualMemory(pixel_info);
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
