static MagickBooleanType WriteMIFFImage(const ImageInfo *image_info,
  Image *image)
{
#if defined(MAGICKCORE_BZLIB_DELEGATE)
  bz_stream
    bzip_info;
#endif

  char
    buffer[MaxTextExtent];

  CompressionType
    compression;

  const char
    *property,
    *value;

  IndexPacket
    index;

#if defined(MAGICKCORE_LZMA_DELEGATE)
  lzma_allocator
    allocator;

  lzma_stream
    initialize_lzma = LZMA_STREAM_INIT,
    lzma_info;
#endif

  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  PixelPacket
    pixel;

  QuantumInfo
    *quantum_info;

  QuantumType
    quantum_type;

  register ssize_t
    i;

  size_t
    length,
    packet_size;

  ssize_t
    y;

  unsigned char
    *compress_pixels,
    *pixels,
    *q;

#if defined(MAGICKCORE_ZLIB_DELEGATE)
  z_stream
    zip_info;
#endif

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
      Allocate image pixels.
    */
    if ((image->storage_class == PseudoClass) &&
        (image->colors > (size_t) (GetQuantumRange(image->depth)+1)))
      (void) SetImageStorageClass(image,DirectClass);
    image->depth=image->depth <= 8 ? 8UL : image->depth <= 16 ? 16UL :
      image->depth <= 32 ? 32UL : 64UL;
    if (IsGrayImage(image,&image->exception) == MagickFalse)
      {
        /*
          sRGB masquerading as a grayscale image?
        */
        if (IsGrayColorspace(image->colorspace) != MagickFalse)
          (void) SetImageColorspace(image,sRGBColorspace);
      }
    else
      if (IsGrayColorspace(image->colorspace) == MagickFalse)
        (void) SetImageColorspace(image,GRAYColorspace);
    quantum_info=AcquireQuantumInfo(image_info,image);
    if (quantum_info == (QuantumInfo *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    if ((image->storage_class != PseudoClass) && (image->depth >= 32) &&
        (quantum_info->format == UndefinedQuantumFormat) &&
        (IsHighDynamicRangeImage(image,&image->exception) != MagickFalse))
      {
        status=SetQuantumFormat(image,quantum_info,FloatingPointQuantumFormat);
        if (status == MagickFalse)
          ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
      }
    compression=UndefinedCompression;
    if (image_info->compression != UndefinedCompression)
      compression=image_info->compression;
    switch (compression)
    {
#if !defined(MAGICKCORE_LZMA_DELEGATE)
      case LZMACompression: compression=NoCompression; break;
#endif
#if !defined(MAGICKCORE_ZLIB_DELEGATE)
      case LZWCompression:
      case ZipCompression: compression=NoCompression; break;
#endif
#if !defined(MAGICKCORE_BZLIB_DELEGATE)
      case BZipCompression: compression=NoCompression; break;
#endif
      case RLECompression:
      {
        if (quantum_info->format == FloatingPointQuantumFormat)
          compression=NoCompression;
        break;
      }
      default:
        break;
    }
    packet_size=(size_t) (quantum_info->depth/8);
    if (image->storage_class == DirectClass)
      packet_size=(size_t) (3*quantum_info->depth/8);
    if (IsGrayColorspace(image->colorspace) != MagickFalse)
      packet_size=(size_t) (quantum_info->depth/8);
    if (image->matte != MagickFalse)
      packet_size+=quantum_info->depth/8;
    if (image->colorspace == CMYKColorspace)
      packet_size+=quantum_info->depth/8;
    if (compression == RLECompression)
      packet_size++;
    length=image->columns;
    length=MagickMax(BZipMaxExtent(packet_size*image->columns),ZipMaxExtent(
      packet_size*image->columns));
    if ((compression == BZipCompression) || (compression == ZipCompression))
      if (length != (size_t) ((unsigned int) length))
        compression=NoCompression;
    compress_pixels=(unsigned char *) AcquireQuantumMemory(length,
      sizeof(*compress_pixels));
    if (compress_pixels == (unsigned char *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    /*
      Write MIFF header.
    */
    (void) WriteBlobString(image,"id=ImageMagick  version=1.0\n");
    (void) FormatLocaleString(buffer,MaxTextExtent,
      "class=%s  colors=%.20g  matte=%s\n",CommandOptionToMnemonic(
      MagickClassOptions,image->storage_class),(double) image->colors,
      CommandOptionToMnemonic(MagickBooleanOptions,(ssize_t) image->matte));
    (void) WriteBlobString(image,buffer);
    (void) FormatLocaleString(buffer,MaxTextExtent,"columns=%.20g  rows=%.20g  "
      "depth=%.20g\n",(double) image->columns,(double) image->rows,(double)
      image->depth);
    (void) WriteBlobString(image,buffer);
    if (image->type != UndefinedType)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"type=%s\n",
          CommandOptionToMnemonic(MagickTypeOptions,image->type));
        (void) WriteBlobString(image,buffer);
      }
    if (image->colorspace != UndefinedColorspace)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"colorspace=%s\n",
          CommandOptionToMnemonic(MagickColorspaceOptions,image->colorspace));
        (void) WriteBlobString(image,buffer);
      }
    if (image->intensity != UndefinedPixelIntensityMethod)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"pixel-intensity=%s\n",
          CommandOptionToMnemonic(MagickPixelIntensityOptions,
          image->intensity));
        (void) WriteBlobString(image,buffer);
      }
    if (image->endian != UndefinedEndian)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"endian=%s\n",
          CommandOptionToMnemonic(MagickEndianOptions,image->endian));
        (void) WriteBlobString(image,buffer);
      }
    if (compression != UndefinedCompression)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"compression=%s  "
          "quality=%.20g\n",CommandOptionToMnemonic(MagickCompressOptions,
          compression),(double) image->quality);
        (void) WriteBlobString(image,buffer);
      }
    if (image->units != UndefinedResolution)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"units=%s\n",
          CommandOptionToMnemonic(MagickResolutionOptions,image->units));
        (void) WriteBlobString(image,buffer);
      }
    if ((image->x_resolution != 0) || (image->y_resolution != 0))
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,
          "resolution=%gx%g\n",image->x_resolution,image->y_resolution);
        (void) WriteBlobString(image,buffer);
      }
    if ((image->page.width != 0) || (image->page.height != 0))
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,
          "page=%.20gx%.20g%+.20g%+.20g\n",(double) image->page.width,(double)
          image->page.height,(double) image->page.x,(double) image->page.y);
        (void) WriteBlobString(image,buffer);
      }
    else
      if ((image->page.x != 0) || (image->page.y != 0))
        {
          (void) FormatLocaleString(buffer,MaxTextExtent,"page=%+ld%+ld\n",
            (long) image->page.x,(long) image->page.y);
          (void) WriteBlobString(image,buffer);
        }
    if ((image->tile_offset.x != 0) || (image->tile_offset.y != 0))
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"tile-offset=%+ld%+ld\n",
          (long) image->tile_offset.x,(long) image->tile_offset.y);
        (void) WriteBlobString(image,buffer);
      }
    if ((GetNextImageInList(image) != (Image *) NULL) ||
        (GetPreviousImageInList(image) != (Image *) NULL))
      {
        if (image->scene == 0)
          (void) FormatLocaleString(buffer,MaxTextExtent,"iterations=%.20g  "
            "delay=%.20g  ticks-per-second=%.20g\n",(double) image->iterations,
            (double) image->delay,(double) image->ticks_per_second);
        else
          (void) FormatLocaleString(buffer,MaxTextExtent,"scene=%.20g  "
            "iterations=%.20g  delay=%.20g  ticks-per-second=%.20g\n",(double)
            image->scene,(double) image->iterations,(double) image->delay,
            (double) image->ticks_per_second);
        (void) WriteBlobString(image,buffer);
      }
    else
      {
        if (image->scene != 0)
          {
            (void) FormatLocaleString(buffer,MaxTextExtent,"scene=%.20g\n",
              (double) image->scene);
            (void) WriteBlobString(image,buffer);
          }
        if (image->iterations != 0)
          {
            (void) FormatLocaleString(buffer,MaxTextExtent,"iterations=%.20g\n",
              (double) image->iterations);
            (void) WriteBlobString(image,buffer);
          }
        if (image->delay != 0)
          {
            (void) FormatLocaleString(buffer,MaxTextExtent,"delay=%.20g\n",
              (double) image->delay);
            (void) WriteBlobString(image,buffer);
          }
        if (image->ticks_per_second != UndefinedTicksPerSecond)
          {
            (void) FormatLocaleString(buffer,MaxTextExtent,
              "ticks-per-second=%.20g\n",(double) image->ticks_per_second);
            (void) WriteBlobString(image,buffer);
          }
      }
    if (image->gravity != UndefinedGravity)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"gravity=%s\n",
          CommandOptionToMnemonic(MagickGravityOptions,image->gravity));
        (void) WriteBlobString(image,buffer);
      }
    if (image->dispose != UndefinedDispose)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"dispose=%s\n",
          CommandOptionToMnemonic(MagickDisposeOptions,image->dispose));
        (void) WriteBlobString(image,buffer);
      }
    if (image->rendering_intent != UndefinedIntent)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,
          "rendering-intent=%s\n",
           CommandOptionToMnemonic(MagickIntentOptions,image->rendering_intent));
        (void) WriteBlobString(image,buffer);
      }
    if (image->gamma != 0.0)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"gamma=%g\n",
          image->gamma);
        (void) WriteBlobString(image,buffer);
      }
    if (image->chromaticity.white_point.x != 0.0)
      {
        /*
          Note chomaticity points.
        */
        (void) FormatLocaleString(buffer,MaxTextExtent,"red-primary=%g,"
          "%g  green-primary=%g,%g  blue-primary=%g,%g\n",
          image->chromaticity.red_primary.x,image->chromaticity.red_primary.y,
          image->chromaticity.green_primary.x,
          image->chromaticity.green_primary.y,
          image->chromaticity.blue_primary.x,
          image->chromaticity.blue_primary.y);
        (void) WriteBlobString(image,buffer);
        (void) FormatLocaleString(buffer,MaxTextExtent,
          "white-point=%g,%g\n",image->chromaticity.white_point.x,
          image->chromaticity.white_point.y);
        (void) WriteBlobString(image,buffer);
      }
    if (image->orientation != UndefinedOrientation)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"orientation=%s\n",
          CommandOptionToMnemonic(MagickOrientationOptions,image->orientation));
        (void) WriteBlobString(image,buffer);
      }
    if (image->profiles != (void *) NULL)
      {
        const char
          *name;

        const StringInfo
          *profile;

        /*
          Write image profiles.
        */
        ResetImageProfileIterator(image);
        name=GetNextImageProfile(image);
        while (name != (const char *) NULL)
        {
          profile=GetImageProfile(image,name);
          if (profile != (StringInfo *) NULL)
            {
              (void) FormatLocaleString(buffer,MaxTextExtent,
                "profile:%s=%.20g\n",name,(double)
                GetStringInfoLength(profile));
              (void) WriteBlobString(image,buffer);
            }
          name=GetNextImageProfile(image);
        }
      }
    if (image->montage != (char *) NULL)
      {
        (void) FormatLocaleString(buffer,MaxTextExtent,"montage=%s\n",
          image->montage);
        (void) WriteBlobString(image,buffer);
      }
    if (quantum_info->format == FloatingPointQuantumFormat)
      (void) SetImageProperty(image,"quantum:format","floating-point");
    ResetImagePropertyIterator(image);
    property=GetNextImageProperty(image);
    while (property != (const char *) NULL)
    {
      (void) FormatLocaleString(buffer,MaxTextExtent,"%s=",property);
      (void) WriteBlobString(image,buffer);
      value=GetImageProperty(image,property);
      if (value != (const char *) NULL)
        {
          size_t
            length;

          length=strlen(value);
          for (i=0; i < (ssize_t) length; i++)
            if (isspace((int) ((unsigned char) value[i])) != 0)
              break;
          if ((i == (ssize_t) length) && (i != 0))
            (void) WriteBlob(image,length,(const unsigned char *) value);
          else
            {
              (void) WriteBlobByte(image,'{');
              if (strchr(value,'}') == (char *) NULL)
                (void) WriteBlob(image,length,(const unsigned char *) value);
              else
                for (i=0; i < (ssize_t) length; i++)
                {
                  if (value[i] == (int) '}')
                    (void) WriteBlobByte(image,'\\');
                  (void) WriteBlobByte(image,value[i]);
                }
              (void) WriteBlobByte(image,'}');
            }
        }
      (void) WriteBlobByte(image,'\n');
      property=GetNextImageProperty(image);
    }
