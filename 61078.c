static MagickBooleanType WriteBMPImage(const ImageInfo *image_info,Image *image)
{
  BMPInfo
    bmp_info;

  const char
    *value;

  const StringInfo
    *profile;

  MagickBooleanType
    have_color_info,
    status;

  MemoryInfo
    *pixel_info;

  MagickOffsetType
    scene;

  register const IndexPacket
    *indexes;

  register const PixelPacket
    *p;

  register ssize_t
    i,
    x;

  register unsigned char
    *q;

  size_t
    bytes_per_line,
    type;

  ssize_t
    y;

  unsigned char
    *bmp_data,
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
  type=4;
  if (LocaleCompare(image_info->magick,"BMP2") == 0)
    type=2;
  else
    if (LocaleCompare(image_info->magick,"BMP3") == 0)
      type=3;

  value=GetImageOption(image_info,"bmp:format");

  if (value != (char *) NULL)
    {
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "  Format=%s",value);

      if (LocaleCompare(value,"bmp2") == 0)
        type=2;
      if (LocaleCompare(value,"bmp3") == 0)
        type=3;
      if (LocaleCompare(value,"bmp4") == 0)
        type=4;
    }

  scene=0;
  do
  {
    /*
      Initialize BMP raster file header.
    */
    (void) TransformImageColorspace(image,sRGBColorspace);
    (void) ResetMagickMemory(&bmp_info,0,sizeof(bmp_info));
    bmp_info.file_size=14+12;
    if (type > 2)
      bmp_info.file_size+=28;
    bmp_info.offset_bits=bmp_info.file_size;
    bmp_info.compression=BI_RGB;
    if ((image->storage_class == PseudoClass) && (image->colors > 256))
      (void) SetImageStorageClass(image,DirectClass);
    if (image->storage_class != DirectClass)
      {
        /*
          Colormapped BMP raster.
        */
        bmp_info.bits_per_pixel=8;
        if (image->colors <= 2)
          bmp_info.bits_per_pixel=1;
        else
          if (image->colors <= 16)
            bmp_info.bits_per_pixel=4;
          else
            if (image->colors <= 256)
              bmp_info.bits_per_pixel=8;
        if (image_info->compression == RLECompression)
          bmp_info.bits_per_pixel=8;
        bmp_info.number_colors=1U << bmp_info.bits_per_pixel;
        if (image->matte != MagickFalse)
          (void) SetImageStorageClass(image,DirectClass);
        else
          if ((size_t) bmp_info.number_colors < image->colors)
            (void) SetImageStorageClass(image,DirectClass);
          else
            {
              bmp_info.file_size+=3*(1UL << bmp_info.bits_per_pixel);
              bmp_info.offset_bits+=3*(1UL << bmp_info.bits_per_pixel);
              if (type > 2)
                {
                  bmp_info.file_size+=(1UL << bmp_info.bits_per_pixel);
                  bmp_info.offset_bits+=(1UL << bmp_info.bits_per_pixel);
                }
            }
      }
    if (image->storage_class == DirectClass)
      {
        /*
          Full color BMP raster.
        */
        bmp_info.number_colors=0;
        bmp_info.bits_per_pixel=(unsigned short)
          ((type > 3) && (image->matte != MagickFalse) ? 32 : 24);
        bmp_info.compression=(unsigned int) ((type > 3) &&
          (image->matte != MagickFalse) ?  BI_BITFIELDS : BI_RGB);
      }
    bytes_per_line=4*((image->columns*bmp_info.bits_per_pixel+31)/32);
    bmp_info.ba_offset=0;
    profile=GetImageProfile(image,"icc");
    have_color_info=(image->rendering_intent != UndefinedIntent) ||
      (profile != (StringInfo *) NULL) || (image->gamma != 0.0) ?  MagickTrue :
      MagickFalse;
    if (type == 2)
      bmp_info.size=12;
    else
      if ((type == 3) || ((image->matte == MagickFalse) &&
          (have_color_info == MagickFalse)))
        {
          type=3;
          bmp_info.size=40;
        }
      else
        {
          int
            extra_size;

          bmp_info.size=108;
          extra_size=68;
          if ((image->rendering_intent != UndefinedIntent) ||
              (profile != (StringInfo *) NULL))
            {
              bmp_info.size=124;
              extra_size+=16;
            }
          bmp_info.file_size+=extra_size;
          bmp_info.offset_bits+=extra_size;
        }
    bmp_info.width=(ssize_t) image->columns;
    bmp_info.height=(ssize_t) image->rows;
    bmp_info.planes=1;
    bmp_info.image_size=(unsigned int) (bytes_per_line*image->rows);
    bmp_info.file_size+=bmp_info.image_size;
    bmp_info.x_pixels=75*39;
    bmp_info.y_pixels=75*39;
    switch (image->units)
    {
      case UndefinedResolution:
      case PixelsPerInchResolution:
      {
        bmp_info.x_pixels=(unsigned int) (100.0*image->x_resolution/2.54);
        bmp_info.y_pixels=(unsigned int) (100.0*image->y_resolution/2.54);
        break;
      }
      case PixelsPerCentimeterResolution:
      {
        bmp_info.x_pixels=(unsigned int) (100.0*image->x_resolution);
        bmp_info.y_pixels=(unsigned int) (100.0*image->y_resolution);
        break;
      }
    }
    bmp_info.colors_important=bmp_info.number_colors;
    /*
      Convert MIFF to BMP raster pixels.
    */
    pixel_info=AcquireVirtualMemory((size_t) bmp_info.image_size,
      sizeof(*pixels));
    if (pixel_info == (MemoryInfo *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    pixels=(unsigned char *) GetVirtualMemoryBlob(pixel_info);
    (void) ResetMagickMemory(pixels,0,(size_t) bmp_info.image_size);
    switch (bmp_info.bits_per_pixel)
    {
      case 1:
      {
        size_t
          bit,
          byte;

        /*
          Convert PseudoClass image to a BMP monochrome image.
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          indexes=GetVirtualIndexQueue(image);
          q=pixels+(image->rows-y-1)*bytes_per_line;
          bit=0;
          byte=0;
          for (x=0; x < (ssize_t) image->columns; x++)
          {
            byte<<=1;
            byte|=GetPixelIndex(indexes+x) != 0 ? 0x01 : 0x00;
            bit++;
            if (bit == 8)
              {
                *q++=(unsigned char) byte;
                bit=0;
                byte=0;
              }
           }
           if (bit != 0)
             {
               *q++=(unsigned char) (byte << (8-bit));
               x++;
             }
          for (x=(ssize_t) (image->columns+7)/8; x < (ssize_t) bytes_per_line; x++)
            *q++=0x00;
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
        break;
      }
      case 4:
      {
        size_t
          nibble,
          byte;

        /*
          Convert PseudoClass image to a BMP monochrome image.
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          indexes=GetVirtualIndexQueue(image);
          q=pixels+(image->rows-y-1)*bytes_per_line;
          nibble=0;
          byte=0;
          for (x=0; x < (ssize_t) image->columns; x++)
          {
            byte<<=4;
            byte|=((size_t) GetPixelIndex(indexes+x) & 0x0f);
            nibble++;
            if (nibble == 2)
              {
                *q++=(unsigned char) byte;
                nibble=0;
                byte=0;
              }
           }
         if (nibble != 0)
           {
             *q++=(unsigned char) (byte << 4);
             x++;
           }
          for (x=(ssize_t) (image->columns+1)/2; x < (ssize_t) bytes_per_line; x++)
            *q++=0x00;
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
        break;
      }
      case 8:
      {
        /*
          Convert PseudoClass packet to BMP pixel.
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          indexes=GetVirtualIndexQueue(image);
          q=pixels+(image->rows-y-1)*bytes_per_line;
          for (x=0; x < (ssize_t) image->columns; x++)
            *q++=(unsigned char) GetPixelIndex(indexes+x);
          for ( ; x < (ssize_t) bytes_per_line; x++)
            *q++=0x00;
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
        break;
      }
      case 24:
      {
        /*
          Convert DirectClass packet to BMP BGR888.
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          q=pixels+(image->rows-y-1)*bytes_per_line;
          for (x=0; x < (ssize_t) image->columns; x++)
          {
            *q++=ScaleQuantumToChar(GetPixelBlue(p));
            *q++=ScaleQuantumToChar(GetPixelGreen(p));
            *q++=ScaleQuantumToChar(GetPixelRed(p));
            p++;
          }
          for (x=3L*(ssize_t) image->columns; x < (ssize_t) bytes_per_line; x++)
            *q++=0x00;
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
        break;
      }
      case 32:
      {
        /*
          Convert DirectClass packet to ARGB8888 pixel.
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          q=pixels+(image->rows-y-1)*bytes_per_line;
          for (x=0; x < (ssize_t) image->columns; x++)
          {
            *q++=ScaleQuantumToChar(GetPixelBlue(p));
            *q++=ScaleQuantumToChar(GetPixelGreen(p));
            *q++=ScaleQuantumToChar(GetPixelRed(p));
            *q++=ScaleQuantumToChar(GetPixelAlpha(p));
            p++;
          }
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
        break;
      }
    }
    if ((type > 2) && (bmp_info.bits_per_pixel == 8))
      if (image_info->compression != NoCompression)
        {
          MemoryInfo
            *rle_info;

          /*
            Convert run-length encoded raster pixels.
          */
          rle_info=AcquireVirtualMemory((size_t) (2*(bytes_per_line+2)+2),
            (image->rows+2)*sizeof(*pixels));
          if (rle_info == (MemoryInfo *) NULL)
            {
              pixel_info=RelinquishVirtualMemory(pixel_info);
              ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
            }
          bmp_data=(unsigned char *) GetVirtualMemoryBlob(rle_info);
          bmp_info.file_size-=bmp_info.image_size;
          bmp_info.image_size=(unsigned int) EncodeImage(image,bytes_per_line,
            pixels,bmp_data);
          bmp_info.file_size+=bmp_info.image_size;
          pixel_info=RelinquishVirtualMemory(pixel_info);
          pixel_info=rle_info;
          pixels=bmp_data;
          bmp_info.compression=BI_RLE8;
        }
    /*
      Write BMP for Windows, all versions, 14-byte header.
    */
    if (image->debug != MagickFalse)
      {
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "   Writing BMP version %.20g datastream",(double) type);
        if (image->storage_class == DirectClass)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "   Storage class=DirectClass");
        else
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "   Storage class=PseudoClass");
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "   Image depth=%.20g",(double) image->depth);
        if (image->matte != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "   Matte=True");
        else
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "   Matte=MagickFalse");
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "   BMP bits_per_pixel=%.20g",(double) bmp_info.bits_per_pixel);
        switch ((int) bmp_info.compression)
        {
           case BI_RGB:
           {
             (void) LogMagickEvent(CoderEvent,GetMagickModule(),
               "   Compression=BI_RGB");
             break;
           }
           case BI_RLE8:
           {
             (void) LogMagickEvent(CoderEvent,GetMagickModule(),
               "   Compression=BI_RLE8");
             break;
           }
           case BI_BITFIELDS:
           {
             (void) LogMagickEvent(CoderEvent,GetMagickModule(),
               "   Compression=BI_BITFIELDS");
             break;
           }
           default:
           {
             (void) LogMagickEvent(CoderEvent,GetMagickModule(),
               "   Compression=UNKNOWN (%u)",bmp_info.compression);
             break;
           }
        }
        if (bmp_info.number_colors == 0)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "   Number_colors=unspecified");
        else
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "   Number_colors=%u",bmp_info.number_colors);
      }
    (void) WriteBlob(image,2,(unsigned char *) "BM");
    (void) WriteBlobLSBLong(image,bmp_info.file_size);
    (void) WriteBlobLSBLong(image,bmp_info.ba_offset);  /* always 0 */
    (void) WriteBlobLSBLong(image,bmp_info.offset_bits);
    if (type == 2)
      {
        /*
          Write 12-byte version 2 bitmap header.
        */
        (void) WriteBlobLSBLong(image,bmp_info.size);
        (void) WriteBlobLSBShort(image,(unsigned short) bmp_info.width);
        (void) WriteBlobLSBShort(image,(unsigned short) bmp_info.height);
        (void) WriteBlobLSBShort(image,bmp_info.planes);
        (void) WriteBlobLSBShort(image,bmp_info.bits_per_pixel);
      }
    else
      {
        /*
          Write 40-byte version 3+ bitmap header.
        */
        (void) WriteBlobLSBLong(image,bmp_info.size);
        (void) WriteBlobLSBLong(image,(unsigned int) bmp_info.width);
        (void) WriteBlobLSBLong(image,(unsigned int) bmp_info.height);
        (void) WriteBlobLSBShort(image,bmp_info.planes);
        (void) WriteBlobLSBShort(image,bmp_info.bits_per_pixel);
        (void) WriteBlobLSBLong(image,bmp_info.compression);
        (void) WriteBlobLSBLong(image,bmp_info.image_size);
        (void) WriteBlobLSBLong(image,bmp_info.x_pixels);
        (void) WriteBlobLSBLong(image,bmp_info.y_pixels);
        (void) WriteBlobLSBLong(image,bmp_info.number_colors);
        (void) WriteBlobLSBLong(image,bmp_info.colors_important);
      }
    if ((type > 3) && ((image->matte != MagickFalse) ||
        (have_color_info != MagickFalse)))
      {
        /*
          Write the rest of the 108-byte BMP Version 4 header.
        */
        (void) WriteBlobLSBLong(image,0x00ff0000U);  /* Red mask */
        (void) WriteBlobLSBLong(image,0x0000ff00U);  /* Green mask */
        (void) WriteBlobLSBLong(image,0x000000ffU);  /* Blue mask */
        (void) WriteBlobLSBLong(image,0xff000000U);  /* Alpha mask */
        (void) WriteBlobLSBLong(image,0x73524742U);  /* sRGB */
        (void) WriteBlobLSBLong(image,(unsigned int)
          (image->chromaticity.red_primary.x*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (image->chromaticity.red_primary.y*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          ((1.000f-(image->chromaticity.red_primary.x+
          image->chromaticity.red_primary.y))*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (image->chromaticity.green_primary.x*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (image->chromaticity.green_primary.y*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          ((1.000f-(image->chromaticity.green_primary.x+
          image->chromaticity.green_primary.y))*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (image->chromaticity.blue_primary.x*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (image->chromaticity.blue_primary.y*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          ((1.000f-(image->chromaticity.blue_primary.x+
          image->chromaticity.blue_primary.y))*0x40000000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (bmp_info.gamma_scale.x*0x10000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (bmp_info.gamma_scale.y*0x10000));
        (void) WriteBlobLSBLong(image,(unsigned int)
          (bmp_info.gamma_scale.z*0x10000));
        if ((image->rendering_intent != UndefinedIntent) ||
            (profile != (StringInfo *) NULL))
          {
            ssize_t
              intent;

            switch ((int) image->rendering_intent)
            {
              case SaturationIntent:
              {
                intent=LCS_GM_BUSINESS;
                break;
              }
              case RelativeIntent:
              {
                intent=LCS_GM_GRAPHICS;
                break;
              }
              case PerceptualIntent:
              {
                intent=LCS_GM_IMAGES;
                break;
              }
              case AbsoluteIntent:
              {
                intent=LCS_GM_ABS_COLORIMETRIC;
                break;
              }
              default:
              {
                intent=0;
                break;
              }
            }
            (void) WriteBlobLSBLong(image,(unsigned int) intent);
            (void) WriteBlobLSBLong(image,0x00);  /* dummy profile data */
            (void) WriteBlobLSBLong(image,0x00);  /* dummy profile length */
            (void) WriteBlobLSBLong(image,0x00);  /* reserved */
          }
      }
    if (image->storage_class == PseudoClass)
      {
        unsigned char
          *bmp_colormap;

        /*
          Dump colormap to file.
        */
        if (image->debug != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "  Colormap: %.20g entries",(double) image->colors);
        bmp_colormap=(unsigned char *) AcquireQuantumMemory((size_t) (1UL <<
          bmp_info.bits_per_pixel),4*sizeof(*bmp_colormap));
        if (bmp_colormap == (unsigned char *) NULL)
          ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
        q=bmp_colormap;
        for (i=0; i < (ssize_t) MagickMin((ssize_t) image->colors,(ssize_t) bmp_info.number_colors); i++)
        {
          *q++=ScaleQuantumToChar(image->colormap[i].blue);
          *q++=ScaleQuantumToChar(image->colormap[i].green);
          *q++=ScaleQuantumToChar(image->colormap[i].red);
          if (type > 2)
            *q++=(unsigned char) 0x0;
        }
        for ( ; i < (ssize_t) (1UL << bmp_info.bits_per_pixel); i++)
        {
          *q++=(unsigned char) 0x00;
          *q++=(unsigned char) 0x00;
          *q++=(unsigned char) 0x00;
          if (type > 2)
            *q++=(unsigned char) 0x00;
        }
        if (type <= 2)
          (void) WriteBlob(image,(size_t) (3*(1L << bmp_info.bits_per_pixel)),
            bmp_colormap);
        else
          (void) WriteBlob(image,(size_t) (4*(1L << bmp_info.bits_per_pixel)),
            bmp_colormap);
        bmp_colormap=(unsigned char *) RelinquishMagickMemory(bmp_colormap);
      }
    if (image->debug != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
        "  Pixels:  %u bytes",bmp_info.image_size);
    (void) WriteBlob(image,(size_t) bmp_info.image_size,pixels);
    pixel_info=RelinquishVirtualMemory(pixel_info);
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,
      GetImageListLength(image));
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
       "  File_size:  %u bytes",bmp_info.file_size);
  (void) CloseBlob(image);
  return(MagickTrue);
}
