static MagickBooleanType WriteFPXImage(const ImageInfo *image_info,Image *image)
{
  FPXBackground
    background_color;

  FPXColorspace
    colorspace =
    {
      TRUE, 4,
      {
        { NIFRGB_R, DATA_TYPE_UNSIGNED_BYTE },
        { NIFRGB_G, DATA_TYPE_UNSIGNED_BYTE },
        { NIFRGB_B, DATA_TYPE_UNSIGNED_BYTE },
        { ALPHA, DATA_TYPE_UNSIGNED_BYTE }
      }
    };

  const char
    *comment,
    *label;

  FPXCompressionOption
    compression;

  FPXImageDesc
    fpx_info;

  FPXImageHandle
    *flashpix;

  FPXStatus
    fpx_status;

  FPXSummaryInformation
    summary_info;

  MagickBooleanType
    status;

  QuantumInfo
    *quantum_info;

  QuantumType
    quantum_type;

  register const PixelPacket
    *p;

  register ssize_t
    i;

  size_t
    memory_limit;

  ssize_t
    y;

  unsigned char
    *pixels;

  unsigned int
    tile_height,
    tile_width;

  /*
    Open input file.
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
  (void) CloseBlob(image);
  /*
    Initialize FPX toolkit.
  */
  image->depth=8;
  memory_limit=20000000;
  fpx_status=FPX_SetToolkitMemoryLimit(&memory_limit);
  if (fpx_status != FPX_OK)
    ThrowWriterException(DelegateError,"UnableToInitializeFPXLibrary");
  tile_width=64;
  tile_height=64;
  colorspace.numberOfComponents=3;
  if (image->matte != MagickFalse)
    colorspace.numberOfComponents=4;
  if ((image_info->type != TrueColorType) &&
      IsGrayImage(image,&image->exception))
    {
      colorspace.numberOfComponents=1;
      colorspace.theComponents[0].myColor=MONOCHROME;
    }
  background_color.color1_value=0;
  background_color.color2_value=0;
  background_color.color3_value=0;
  background_color.color4_value=0;
  compression=NONE;
  if (image->compression == JPEGCompression)
    compression=JPEG_UNSPECIFIED;
  if (image_info->compression == JPEGCompression)
    compression=JPEG_UNSPECIFIED;
  {
#if defined(macintosh)
    FSSpec
      fsspec;

    FilenameToFSSpec(filename,&fsspec);
    fpx_status=FPX_CreateImageByFilename((const FSSpec &) fsspec,image->columns,
#else
    fpx_status=FPX_CreateImageByFilename(image->filename,image->columns,
#endif
      image->rows,tile_width,tile_height,colorspace,background_color,
      compression,&flashpix);
  }
  if (fpx_status != FPX_OK)
    return(status);
  if (compression == JPEG_UNSPECIFIED)
    {
      /*
        Initialize the compression by quality for the entire image.
      */
      fpx_status=FPX_SetJPEGCompression(flashpix,(unsigned short)
        image->quality == UndefinedCompressionQuality ? 75 : image->quality);
      if (fpx_status != FPX_OK)
        ThrowWriterException(DelegateError,"UnableToSetJPEGLevel");
    }
  /*
    Set image summary info.
  */
  summary_info.title_valid=MagickFalse;
  summary_info.subject_valid=MagickFalse;
  summary_info.author_valid=MagickFalse;
  summary_info.comments_valid=MagickFalse;
  summary_info.keywords_valid=MagickFalse;
  summary_info.OLEtemplate_valid=MagickFalse;
  summary_info.last_author_valid=MagickFalse;
  summary_info.rev_number_valid=MagickFalse;
  summary_info.edit_time_valid=MagickFalse;
  summary_info.last_printed_valid=MagickFalse;
  summary_info.create_dtm_valid=MagickFalse;
  summary_info.last_save_dtm_valid=MagickFalse;
  summary_info.page_count_valid=MagickFalse;
  summary_info.word_count_valid=MagickFalse;
  summary_info.char_count_valid=MagickFalse;
  summary_info.thumbnail_valid=MagickFalse;
  summary_info.appname_valid=MagickFalse;
  summary_info.security_valid=MagickFalse;
  summary_info.title.ptr=(unsigned char *) NULL;
  label=GetImageProperty(image,"label");
  if (label != (const char *) NULL)
    {
      size_t
        length;

      /*
        Note image label.
      */
      summary_info.title_valid=MagickTrue;
      length=strlen(label);
      summary_info.title.length=length;
      if (~length >= (MaxTextExtent-1))
        summary_info.title.ptr=(unsigned char *) AcquireQuantumMemory(
          length+MaxTextExtent,sizeof(*summary_info.title.ptr));
      if (summary_info.title.ptr == (unsigned char *) NULL)
        ThrowWriterException(DelegateError,"UnableToSetImageTitle");
      (void) CopyMagickString((char *) summary_info.title.ptr,label,
        MaxTextExtent);
    }
  comment=GetImageProperty(image,"comment");
  if (comment != (const char *) NULL)
    {
      /*
        Note image comment.
      */
      summary_info.comments_valid=MagickTrue;
      summary_info.comments.ptr=(unsigned char *) AcquireString(comment);
      summary_info.comments.length=strlen(comment);
    }
  fpx_status=FPX_SetSummaryInformation(flashpix,&summary_info);
  if (fpx_status != FPX_OK)
    ThrowWriterException(DelegateError,"UnableToSetSummaryInfo");
  /*
    Initialize FlashPix image description.
  */
  quantum_info=AcquireQuantumInfo(image_info,image);
  if (quantum_info == (QuantumInfo *) NULL)
    ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
  pixels=GetQuantumPixels(quantum_info);
  fpx_info.numberOfComponents=colorspace.numberOfComponents;
  for (i=0; i < (ssize_t) fpx_info.numberOfComponents; i++)
  {
    fpx_info.components[i].myColorType.myDataType=DATA_TYPE_UNSIGNED_BYTE;
    fpx_info.components[i].horzSubSampFactor=1;
    fpx_info.components[i].vertSubSampFactor=1;
    fpx_info.components[i].columnStride=fpx_info.numberOfComponents;
    fpx_info.components[i].lineStride=
      image->columns*fpx_info.components[i].columnStride;
    fpx_info.components[i].theData=pixels+i;
  }
  fpx_info.components[0].myColorType.myColor=fpx_info.numberOfComponents != 1
    ? NIFRGB_R : MONOCHROME;
  fpx_info.components[1].myColorType.myColor=NIFRGB_G;
  fpx_info.components[2].myColorType.myColor=NIFRGB_B;
  fpx_info.components[3].myColorType.myColor=ALPHA;
  /*
    Write image pixelss.
  */
  quantum_type=RGBQuantum;
  if (image->matte != MagickFalse)
    quantum_type=RGBAQuantum;
  if (fpx_info.numberOfComponents == 1)
    quantum_type=GrayQuantum;
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    size_t
      length;

    p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    length=ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
      quantum_type,pixels,&image->exception);
    (void) length;
    fpx_status=FPX_WriteImageLine(flashpix,&fpx_info);
    if (fpx_status != FPX_OK)
      break;
    status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }
  quantum_info=DestroyQuantumInfo(quantum_info);
  if (image_info->view != (char *) NULL)
    {
      FPXAffineMatrix
        affine;

      FPXColorTwistMatrix
        color_twist;

      FPXContrastAdjustment
        contrast;

      FPXFilteringValue
        sharpen;

      FPXResultAspectRatio
        aspect_ratio;

      FPXROI
        view_rect;

      MagickBooleanType
        affine_valid,
        aspect_ratio_valid,
        color_twist_valid,
        contrast_valid,
        sharpen_valid,
        view_rect_valid;

      /*
        Initialize default viewing parameters.
      */
      contrast=1.0;
      contrast_valid=MagickTrue;
      color_twist.byy=1.0;
      color_twist.byc1=0.0;
      color_twist.byc2=0.0;
      color_twist.dummy1_zero=0.0;
      color_twist.bc1y=0.0;
      color_twist.bc1c1=1.0;
      color_twist.bc1c2=0.0;
      color_twist.dummy2_zero=0.0;
      color_twist.bc2y=0.0;
      color_twist.bc2c1=0.0;
      color_twist.bc2c2=1.0;
      color_twist.dummy3_zero=0.0;
      color_twist.dummy4_zero=0.0;
      color_twist.dummy5_zero=0.0;
      color_twist.dummy6_zero=0.0;
      color_twist.dummy7_one=1.0;
      color_twist_valid=MagickTrue;
      sharpen=0.0;
      sharpen_valid=MagickTrue;
      aspect_ratio=(double) image->columns/image->rows;
      aspect_ratio_valid=MagickTrue;
      view_rect.left=(float) 0.1;
      view_rect.width=aspect_ratio-0.2;
      view_rect.top=(float) 0.1;
      view_rect.height=(float) 0.8; /* 1.0-0.2 */
      view_rect_valid=MagickTrue;
      affine.a11=1.0;
      affine.a12=0.0;
      affine.a13=0.0;
      affine.a14=0.0;
      affine.a21=0.0;
      affine.a22=1.0;
      affine.a23=0.0;
      affine.a24=0.0;
      affine.a31=0.0;
      affine.a32=0.0;
      affine.a33=1.0;
      affine.a34=0.0;
      affine.a41=0.0;
      affine.a42=0.0;
      affine.a43=0.0;
      affine.a44=1.0;
      affine_valid=MagickTrue;
      if (0)
        {
          /*
            Color color twist.
          */
          SetBrightness(0.5,&color_twist);
          SetSaturation(0.5,&color_twist);
          SetColorBalance(0.5,1.0,1.0,&color_twist);
          color_twist_valid=MagickTrue;
        }
      if (affine_valid != MagickFalse)
        {
          fpx_status=FPX_SetImageAffineMatrix(flashpix,&affine);
          if (fpx_status != FPX_OK)
            ThrowWriterException(DelegateError,"UnableToSetAffineMatrix");
        }
      if (aspect_ratio_valid != MagickFalse)
        {
          fpx_status=FPX_SetImageResultAspectRatio(flashpix,&aspect_ratio);
          if (fpx_status != FPX_OK)
            ThrowWriterException(DelegateError,"UnableToSetAspectRatio");
        }
      if (color_twist_valid != MagickFalse)
        {
          fpx_status=FPX_SetImageColorTwistMatrix(flashpix,&color_twist);
          if (fpx_status != FPX_OK)
            ThrowWriterException(DelegateError,"UnableToSetColorTwist");
        }
      if (contrast_valid != MagickFalse)
        {
          fpx_status=FPX_SetImageContrastAdjustment(flashpix,&contrast);
          if (fpx_status != FPX_OK)
            ThrowWriterException(DelegateError,"UnableToSetContrast");
        }
      if (sharpen_valid != MagickFalse)
        {
          fpx_status=FPX_SetImageFilteringValue(flashpix,&sharpen);
          if (fpx_status != FPX_OK)
            ThrowWriterException(DelegateError,"UnableToSetFilteringValue");
        }
      if (view_rect_valid != MagickFalse)
        {
          fpx_status=FPX_SetImageROI(flashpix,&view_rect);
          if (fpx_status != FPX_OK)
            ThrowWriterException(DelegateError,"UnableToSetRegionOfInterest");
        }
    }
  (void) FPX_CloseImage(flashpix);
  FPX_ClearSystem();
  return(MagickTrue);
}
