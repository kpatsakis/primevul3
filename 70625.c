WandExport MagickBooleanType MogrifyImage(ImageInfo *image_info,const int argc,
  const char **argv,Image **image,ExceptionInfo *exception)
{
  ChannelType
    channel;

  const char
    *format,
    *option;

  DrawInfo
    *draw_info;

  GeometryInfo
    geometry_info;

  Image
    *region_image;

  ImageInfo
    *mogrify_info;

  MagickStatusType
    status;

  MagickPixelPacket
    fill;

  MagickStatusType
    flags;

  QuantizeInfo
    *quantize_info;

  RectangleInfo
    geometry,
    region_geometry;

  register ssize_t
    i;

  /*
    Initialize method variables.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  assert(image != (Image **) NULL);
  assert((*image)->signature == MagickCoreSignature);
  if ((*image)->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",(*image)->filename);
  if (argc < 0)
    return(MagickTrue);
  mogrify_info=CloneImageInfo(image_info);
  draw_info=CloneDrawInfo(mogrify_info,(DrawInfo *) NULL);
  quantize_info=AcquireQuantizeInfo(mogrify_info);
  SetGeometryInfo(&geometry_info);
  GetMagickPixelPacket(*image,&fill);
  SetMagickPixelPacket(*image,&(*image)->background_color,(IndexPacket *) NULL,
    &fill);
  channel=mogrify_info->channel;
  format=GetImageOption(mogrify_info,"format");
  SetGeometry(*image,&region_geometry);
  region_image=NewImageList();
  /*
    Transmogrify the image.
  */
  for (i=0; i < (ssize_t) argc; i++)
  {
    Image
      *mogrify_image;

    ssize_t
      count;

    option=argv[i];
    if (IsCommandOption(option) == MagickFalse)
      continue;
    count=MagickMax(ParseCommandOption(MagickCommandOptions,MagickFalse,option),
      0L);
    if ((i+count) >= (ssize_t) argc)
      break;
    status=MogrifyImageInfo(mogrify_info,(int) count+1,argv+i,exception);
    mogrify_image=(Image *) NULL;
    switch (*(option+1))
    {
      case 'a':
      {
        if (LocaleCompare("adaptive-blur",option+1) == 0)
          {
            /*
              Adaptive blur image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=AdaptiveBlurImageChannel(*image,channel,
              geometry_info.rho,geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("adaptive-resize",option+1) == 0)
          {
            /*
              Adaptive resize image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=AdaptiveResizeImage(*image,geometry.width,
              geometry.height,exception);
            break;
          }
        if (LocaleCompare("adaptive-sharpen",option+1) == 0)
          {
            /*
              Adaptive sharpen image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=AdaptiveSharpenImageChannel(*image,channel,
              geometry_info.rho,geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("affine",option+1) == 0)
          {
            /*
              Affine matrix.
            */
            if (*option == '+')
              {
                GetAffineMatrix(&draw_info->affine);
                break;
              }
            (void) ParseAffineGeometry(argv[i+1],&draw_info->affine,exception);
            break;
          }
        if (LocaleCompare("alpha",option+1) == 0)
          {
            AlphaChannelType
              alpha_type;

            (void) SyncImageSettings(mogrify_info,*image);
            alpha_type=(AlphaChannelType) ParseCommandOption(MagickAlphaOptions,
              MagickFalse,argv[i+1]);
            (void) SetImageAlphaChannel(*image,alpha_type);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("annotate",option+1) == 0)
          {
            char
              *text,
              geometry[MaxTextExtent];

            /*
              Annotate image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            SetGeometryInfo(&geometry_info);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=geometry_info.rho;
            text=InterpretImageProperties(mogrify_info,*image,argv[i+2]);
            InheritException(exception,&(*image)->exception);
            if (text == (char *) NULL)
              break;
            (void) CloneString(&draw_info->text,text);
            text=DestroyString(text);
            (void) FormatLocaleString(geometry,MaxTextExtent,"%+f%+f",
              geometry_info.xi,geometry_info.psi);
            (void) CloneString(&draw_info->geometry,geometry);
            draw_info->affine.sx=cos(DegreesToRadians(
              fmod(geometry_info.rho,360.0)));
            draw_info->affine.rx=sin(DegreesToRadians(
              fmod(geometry_info.rho,360.0)));
            draw_info->affine.ry=(-sin(DegreesToRadians(
              fmod(geometry_info.sigma,360.0))));
            draw_info->affine.sy=cos(DegreesToRadians(
              fmod(geometry_info.sigma,360.0)));
            (void) AnnotateImage(*image,draw_info);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("antialias",option+1) == 0)
          {
            draw_info->stroke_antialias=(*option == '-') ? MagickTrue :
              MagickFalse;
            draw_info->text_antialias=(*option == '-') ? MagickTrue :
              MagickFalse;
            break;
          }
        if (LocaleCompare("auto-gamma",option+1) == 0)
          {
            /*
              Auto Adjust Gamma of image based on its mean
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) AutoGammaImageChannel(*image,channel);
            break;
          }
        if (LocaleCompare("auto-level",option+1) == 0)
          {
            /*
              Perfectly Normalize (max/min stretch) the image
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) AutoLevelImageChannel(*image,channel);
            break;
          }
        if (LocaleCompare("auto-orient",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=AutoOrientImage(*image,(*image)->orientation,
              exception);
            break;
          }
        break;
      }
      case 'b':
      {
        if (LocaleCompare("black-threshold",option+1) == 0)
          {
            /*
              Black threshold image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) BlackThresholdImageChannel(*image,channel,argv[i+1],
              exception);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("blue-shift",option+1) == 0)
          {
            /*
              Blue shift image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            geometry_info.rho=1.5;
            if (*option == '-')
              flags=ParseGeometry(argv[i+1],&geometry_info);
            mogrify_image=BlueShiftImage(*image,geometry_info.rho,exception);
            break;
          }
        if (LocaleCompare("blur",option+1) == 0)
          {
            /*
              Gaussian blur image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=BlurImageChannel(*image,channel,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("border",option+1) == 0)
          {
            /*
              Surround image with a border of solid color.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParsePageGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=BorderImage(*image,&geometry,exception);
            break;
          }
        if (LocaleCompare("bordercolor",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) QueryColorDatabase(MogrifyBorderColor,
                  &draw_info->border_color,exception);
                break;
              }
            (void) QueryColorDatabase(argv[i+1],&draw_info->border_color,
              exception);
            break;
          }
        if (LocaleCompare("box",option+1) == 0)
          {
            (void) QueryColorDatabase(argv[i+1],&draw_info->undercolor,
              exception);
            break;
          }
        if (LocaleCompare("brightness-contrast",option+1) == 0)
          {
            double
              brightness,
              contrast;

            GeometryInfo
              geometry_info;

            MagickStatusType
              flags;

            /*
              Brightness / contrast image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            brightness=geometry_info.rho;
            contrast=0.0;
            if ((flags & SigmaValue) != 0)
              contrast=geometry_info.sigma;
            (void) BrightnessContrastImageChannel(*image,channel,brightness,
              contrast);
            InheritException(exception,&(*image)->exception);
            break;
          }
        break;
      }
      case 'c':
      {
        if (LocaleCompare("canny",option+1) == 0)
          {
            /*
              Detect edges in the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            if ((flags & XiValue) == 0)
              geometry_info.xi=0.10;
            if ((flags & PsiValue) == 0)
              geometry_info.psi=0.30;
            if ((flags & PercentValue) != 0)
              {
                geometry_info.xi/=100.0;
                geometry_info.psi/=100.0;
              }
            mogrify_image=CannyEdgeImage(*image,geometry_info.rho,
              geometry_info.sigma,geometry_info.xi,geometry_info.psi,exception);
            break;
          }
        if (LocaleCompare("cdl",option+1) == 0)
          {
            char
              *color_correction_collection;

            /*
              Color correct with a color decision list.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            color_correction_collection=FileToString(argv[i+1],~0UL,exception);
            if (color_correction_collection == (char *) NULL)
              break;
            (void) ColorDecisionListImage(*image,color_correction_collection);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("channel",option+1) == 0)
          {
            if (*option == '+')
              channel=DefaultChannels;
            else
              channel=(ChannelType) ParseChannelOption(argv[i+1]);
            break;
          }
        if (LocaleCompare("charcoal",option+1) == 0)
          {
            /*
              Charcoal image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=CharcoalImage(*image,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("chop",option+1) == 0)
          {
            /*
              Chop the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGravityGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=ChopImage(*image,&geometry,exception);
            break;
          }
        if (LocaleCompare("clamp",option+1) == 0)
          {
            /*
              Clamp image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ClampImageChannel(*image,channel);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("clip",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              {
                (void) SetImageClipMask(*image,(Image *) NULL);
                InheritException(exception,&(*image)->exception);
                break;
              }
            (void) ClipImage(*image);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("clip-mask",option+1) == 0)
          {
            CacheView
              *mask_view;

            Image
              *mask_image;

            register PixelPacket
              *magick_restrict q;

            register ssize_t
              x;

            ssize_t
              y;

            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              {
                /*
                  Remove a mask.
                */
                (void) SetImageMask(*image,(Image *) NULL);
                InheritException(exception,&(*image)->exception);
                break;
              }
            /*
              Set the image mask.
              FUTURE: This Should Be a SetImageAlphaChannel() call, Or two.
            */
            mask_image=GetImageCache(mogrify_info,argv[i+1],exception);
            if (mask_image == (Image *) NULL)
              break;
            if (SetImageStorageClass(mask_image,DirectClass) == MagickFalse)
              return(MagickFalse);
            mask_view=AcquireAuthenticCacheView(mask_image,exception);
            for (y=0; y < (ssize_t) mask_image->rows; y++)
            {
              q=GetCacheViewAuthenticPixels(mask_view,0,y,mask_image->columns,1,
                exception);
              if (q == (PixelPacket *) NULL)
                break;
              for (x=0; x < (ssize_t) mask_image->columns; x++)
              {
                if (mask_image->matte == MagickFalse)
                  SetPixelOpacity(q,ClampToQuantum(GetPixelIntensity(mask_image,
                    q)));
                SetPixelRed(q,GetPixelOpacity(q));
                SetPixelGreen(q,GetPixelOpacity(q));
                SetPixelBlue(q,GetPixelOpacity(q));
                q++;
              }
              if (SyncCacheViewAuthenticPixels(mask_view,exception) == MagickFalse)
                break;
            }
            mask_view=DestroyCacheView(mask_view);
            mask_image->matte=MagickTrue;
            (void) SetImageClipMask(*image,mask_image);
            mask_image=DestroyImage(mask_image);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("clip-path",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ClipImagePath(*image,argv[i+1],*option == '-' ? MagickTrue :
              MagickFalse);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("colorize",option+1) == 0)
          {
            /*
              Colorize the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=ColorizeImage(*image,argv[i+1],draw_info->fill,
              exception);
            break;
          }
        if (LocaleCompare("color-matrix",option+1) == 0)
          {
            KernelInfo
              *kernel;

            (void) SyncImageSettings(mogrify_info,*image);
            kernel=AcquireKernelInfo(argv[i+1]);
            if (kernel == (KernelInfo *) NULL)
              break;
            mogrify_image=ColorMatrixImage(*image,kernel,exception);
            kernel=DestroyKernelInfo(kernel);
            break;
          }
        if (LocaleCompare("colors",option+1) == 0)
          {
            /*
              Reduce the number of colors in the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            quantize_info->number_colors=StringToUnsignedLong(argv[i+1]);
            if (quantize_info->number_colors == 0)
              break;
            if (((*image)->storage_class == DirectClass) ||
                (*image)->colors > quantize_info->number_colors)
              (void) QuantizeImage(quantize_info,*image);
            else
              (void) CompressImageColormap(*image);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("colorspace",option+1) == 0)
          {
            ColorspaceType
              colorspace;

            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              {
                (void) TransformImageColorspace(*image,sRGBColorspace);
                InheritException(exception,&(*image)->exception);
                break;
              }
            colorspace=(ColorspaceType) ParseCommandOption(
              MagickColorspaceOptions,MagickFalse,argv[i+1]);
            (void) TransformImageColorspace(*image,colorspace);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("connected-components",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=ConnectedComponentsImage(*image,
              StringToInteger(argv[i+1]),exception);
            break;
          }
        if (LocaleCompare("contrast",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ContrastImage(*image,(*option == '-') ? MagickTrue :
              MagickFalse);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("contrast-stretch",option+1) == 0)
          {
            double
              black_point,
              white_point;

            MagickStatusType
              flags;

            /*
              Contrast stretch image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            black_point=geometry_info.rho;
            white_point=(flags & SigmaValue) != 0 ? geometry_info.sigma :
              black_point;
            if ((flags & PercentValue) != 0)
              {
                black_point*=(double) (*image)->columns*(*image)->rows/100.0;
                white_point*=(double) (*image)->columns*(*image)->rows/100.0;
              }
            white_point=(MagickRealType) (*image)->columns*(*image)->rows-
              white_point;
            (void) ContrastStretchImageChannel(*image,channel,black_point,
              white_point);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("convolve",option+1) == 0)
          {
            double
              gamma;

            KernelInfo
              *kernel_info;

            register ssize_t
              j;

            size_t
              extent;

            (void) SyncImageSettings(mogrify_info,*image);
            kernel_info=AcquireKernelInfo(argv[i+1]);
            if (kernel_info == (KernelInfo *) NULL)
              break;
            extent=kernel_info->width*kernel_info->height;
            gamma=0.0;
            for (j=0; j < (ssize_t) extent; j++)
              gamma+=kernel_info->values[j];
            gamma=1.0/(fabs((double) gamma) <= MagickEpsilon ? 1.0 : gamma);
            for (j=0; j < (ssize_t) extent; j++)
              kernel_info->values[j]*=gamma;
            mogrify_image=MorphologyImage(*image,CorrelateMorphology,1,
              kernel_info,exception);
            kernel_info=DestroyKernelInfo(kernel_info);
            break;
          }
        if (LocaleCompare("crop",option+1) == 0)
          {
            /*
              Crop a image to a smaller size
            */
            (void) SyncImageSettings(mogrify_info,*image);
#if 0
            flags=ParseGravityGeometry(*image,argv[i+1],&geometry,exception);
            if (((geometry.width != 0) || (geometry.height != 0)) &&
                ((flags & XValue) == 0) && ((flags & YValue) == 0))
              break;
#endif
#if 0
            mogrify_image=CloneImage(*image,0,0,MagickTrue,&(*image)->exception);
            mogrify_image->next = mogrify_image->previous = (Image *) NULL;
            (void) TransformImage(&mogrify_image,argv[i+1],(char *) NULL);
            InheritException(exception,&mogrify_image->exception);
#else
            mogrify_image=CropImageToTiles(*image,argv[i+1],exception);
#endif
            break;
          }
        if (LocaleCompare("cycle",option+1) == 0)
          {
            /*
              Cycle an image colormap.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) CycleColormapImage(*image,(ssize_t) StringToLong(argv[i+1]));
            InheritException(exception,&(*image)->exception);
            break;
          }
        break;
      }
      case 'd':
      {
        if (LocaleCompare("decipher",option+1) == 0)
          {
            StringInfo
              *passkey;

            /*
              Decipher pixels.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            passkey=FileToStringInfo(argv[i+1],~0UL,exception);
            if (passkey != (StringInfo *) NULL)
              {
                (void) PasskeyDecipherImage(*image,passkey,exception);
                passkey=DestroyStringInfo(passkey);
              }
            break;
          }
        if (LocaleCompare("density",option+1) == 0)
          {
            /*
              Set image density.
            */
            (void) CloneString(&draw_info->density,argv[i+1]);
            break;
          }
        if (LocaleCompare("depth",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              {
                (void) SetImageDepth(*image,MAGICKCORE_QUANTUM_DEPTH);
                break;
              }
            (void) SetImageDepth(*image,StringToUnsignedLong(argv[i+1]));
            break;
          }
        if (LocaleCompare("deskew",option+1) == 0)
          {
            double
              threshold;

            /*
              Straighten the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              threshold=40.0*QuantumRange/100.0;
            else
              threshold=StringToDoubleInterval(argv[i+1],(double) QuantumRange+
                1.0);
            mogrify_image=DeskewImage(*image,threshold,exception);
            break;
          }
        if (LocaleCompare("despeckle",option+1) == 0)
          {
            /*
              Reduce the speckles within an image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=DespeckleImage(*image,exception);
            break;
          }
        if (LocaleCompare("display",option+1) == 0)
          {
            (void) CloneString(&draw_info->server_name,argv[i+1]);
            break;
          }
        if (LocaleCompare("distort",option+1) == 0)
          {
            char
              *args,
              token[MaxTextExtent];

            const char
              *p;

            DistortImageMethod
              method;

            double
              *arguments;

            register ssize_t
              x;

            size_t
              number_arguments;

            /*
              Distort image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            method=(DistortImageMethod) ParseCommandOption(MagickDistortOptions,
              MagickFalse,argv[i+1]);
            if (method == ResizeDistortion)
              {
                double
                  resize_args[2];

                 /*
                   Resize distortion.
                 */
                 (void) ParseRegionGeometry(*image,argv[i+2],&geometry,
                   exception);
                 resize_args[0]=(double) geometry.width;
                 resize_args[1]=(double) geometry.height;
                 mogrify_image=DistortImage(*image,method,(size_t) 2,
                   resize_args,MagickTrue,exception);
                 break;
              }
            args=InterpretImageProperties(mogrify_info,*image,argv[i+2]);
            InheritException(exception,&(*image)->exception);
            if (args == (char *) NULL)
              break;
            p=(char *) args;
            for (x=0; *p != '\0'; x++)
            {
              GetNextToken(p,&p,MaxTextExtent,token);
              if (*token == ',')
                GetNextToken(p,&p,MaxTextExtent,token);
            }
            number_arguments=(size_t) x;
            arguments=(double *) AcquireQuantumMemory(number_arguments,
              sizeof(*arguments));
            if (arguments == (double *) NULL)
              ThrowWandFatalException(ResourceLimitFatalError,
                "MemoryAllocationFailed",(*image)->filename);
            (void) memset(arguments,0,number_arguments*sizeof(*arguments));
            p=(char *) args;
            for (x=0; (x < (ssize_t) number_arguments) && (*p != '\0'); x++)
            {
              GetNextToken(p,&p,MaxTextExtent,token);
              if (*token == ',')
                GetNextToken(p,&p,MaxTextExtent,token);
              arguments[x]=StringToDouble(token,(char **) NULL);
            }
            args=DestroyString(args);
            mogrify_image=DistortImage(*image,method,number_arguments,arguments,
              (*option == '+') ? MagickTrue : MagickFalse,exception);
            arguments=(double *) RelinquishMagickMemory(arguments);
            break;
          }
        if (LocaleCompare("dither",option+1) == 0)
          {
            if (*option == '+')
              {
                quantize_info->dither=MagickFalse;
                break;
              }
            quantize_info->dither=MagickTrue;
            quantize_info->dither_method=(DitherMethod) ParseCommandOption(
              MagickDitherOptions,MagickFalse,argv[i+1]);
            if (quantize_info->dither_method == NoDitherMethod)
              quantize_info->dither=MagickFalse;
            break;
          }
        if (LocaleCompare("draw",option+1) == 0)
          {
            /*
              Draw image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) CloneString(&draw_info->primitive,argv[i+1]);
            (void) DrawImage(*image,draw_info);
            InheritException(exception,&(*image)->exception);
            break;
          }
        break;
      }
      case 'e':
      {
        if (LocaleCompare("edge",option+1) == 0)
          {
            /*
              Enhance edges in the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=EdgeImage(*image,geometry_info.rho,exception);
            break;
          }
        if (LocaleCompare("emboss",option+1) == 0)
          {
            /*
              Gaussian embossen image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=EmbossImage(*image,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("encipher",option+1) == 0)
          {
            StringInfo
              *passkey;

            /*
              Encipher pixels.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            passkey=FileToStringInfo(argv[i+1],~0UL,exception);
            if (passkey != (StringInfo *) NULL)
              {
                (void) PasskeyEncipherImage(*image,passkey,exception);
                passkey=DestroyStringInfo(passkey);
              }
            break;
          }
        if (LocaleCompare("encoding",option+1) == 0)
          {
            (void) CloneString(&draw_info->encoding,argv[i+1]);
            break;
          }
        if (LocaleCompare("enhance",option+1) == 0)
          {
            /*
              Enhance image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=EnhanceImage(*image,exception);
            break;
          }
        if (LocaleCompare("equalize",option+1) == 0)
          {
            /*
              Equalize image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) EqualizeImageChannel(*image,channel);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("evaluate",option+1) == 0)
          {
            double
              constant;

            MagickEvaluateOperator
              op;

            (void) SyncImageSettings(mogrify_info,*image);
            op=(MagickEvaluateOperator) ParseCommandOption(
              MagickEvaluateOptions,MagickFalse,argv[i+1]);
            constant=StringToDoubleInterval(argv[i+2],(double) QuantumRange+
              1.0);
            (void) EvaluateImageChannel(*image,channel,op,constant,exception);
            break;
          }
        if (LocaleCompare("extent",option+1) == 0)
          {
            /*
              Set the image extent.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGravityGeometry(*image,argv[i+1],&geometry,exception);
            if (geometry.width == 0)
              geometry.width=(*image)->columns;
            if (geometry.height == 0)
              geometry.height=(*image)->rows;
            mogrify_image=ExtentImage(*image,&geometry,exception);
            break;
          }
        break;
      }
      case 'f':
      {
        if (LocaleCompare("family",option+1) == 0)
          {
            if (*option == '+')
              {
                if (draw_info->family != (char *) NULL)
                  draw_info->family=DestroyString(draw_info->family);
                break;
              }
            (void) CloneString(&draw_info->family,argv[i+1]);
            break;
          }
        if (LocaleCompare("features",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) DeleteImageArtifact(*image,"identify:features");
                break;
              }
            (void) SetImageArtifact(*image,"identify:features",argv[i+1]);
            (void) SetImageArtifact(*image,"verbose","true");
            break;
          }
        if (LocaleCompare("fill",option+1) == 0)
          {
            ExceptionInfo
              *sans;

            GetMagickPixelPacket(*image,&fill);
            if (*option == '+')
              {
                (void) QueryMagickColor("none",&fill,exception);
                (void) QueryColorDatabase("none",&draw_info->fill,exception);
                if (draw_info->fill_pattern != (Image *) NULL)
                  draw_info->fill_pattern=DestroyImage(draw_info->fill_pattern);
                break;
              }
            sans=AcquireExceptionInfo();
            (void) QueryMagickColor(argv[i+1],&fill,sans);
            status=QueryColorDatabase(argv[i+1],&draw_info->fill,sans);
            sans=DestroyExceptionInfo(sans);
            if (status == MagickFalse)
              draw_info->fill_pattern=GetImageCache(mogrify_info,argv[i+1],
                exception);
            break;
          }
        if (LocaleCompare("flip",option+1) == 0)
          {
            /*
              Flip image scanlines.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=FlipImage(*image,exception);
            break;
          }
        if (LocaleCompare("floodfill",option+1) == 0)
          {
            MagickPixelPacket
              target;

            /*
              Floodfill image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParsePageGeometry(*image,argv[i+1],&geometry,exception);
            (void) QueryMagickColor(argv[i+2],&target,exception);
            (void) FloodfillPaintImage(*image,channel,draw_info,&target,
              geometry.x,geometry.y,*option == '-' ? MagickFalse : MagickTrue);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("flop",option+1) == 0)
          {
            /*
              Flop image scanlines.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=FlopImage(*image,exception);
            break;
          }
        if (LocaleCompare("font",option+1) == 0)
          {
            if (*option == '+')
              {
                if (draw_info->font != (char *) NULL)
                  draw_info->font=DestroyString(draw_info->font);
                break;
              }
            (void) CloneString(&draw_info->font,argv[i+1]);
            break;
          }
        if (LocaleCompare("format",option+1) == 0)
          {
            format=argv[i+1];
            break;
          }
        if (LocaleCompare("frame",option+1) == 0)
          {
            FrameInfo
              frame_info;

            /*
              Surround image with an ornamental border.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParsePageGeometry(*image,argv[i+1],&geometry,exception);
            frame_info.width=geometry.width;
            frame_info.height=geometry.height;
            frame_info.outer_bevel=geometry.x;
            frame_info.inner_bevel=geometry.y;
            frame_info.x=(ssize_t) frame_info.width;
            frame_info.y=(ssize_t) frame_info.height;
            frame_info.width=(*image)->columns+2*frame_info.width;
            frame_info.height=(*image)->rows+2*frame_info.height;
            mogrify_image=FrameImage(*image,&frame_info,exception);
            break;
          }
        if (LocaleCompare("function",option+1) == 0)
          {
            char
              *arguments,
              token[MaxTextExtent];

            const char
              *p;

            double
              *parameters;

            MagickFunction
              function;

            register ssize_t
              x;

            size_t
              number_parameters;

            /*
              Function Modify Image Values
            */
            (void) SyncImageSettings(mogrify_info,*image);
            function=(MagickFunction) ParseCommandOption(MagickFunctionOptions,
              MagickFalse,argv[i+1]);
            arguments=InterpretImageProperties(mogrify_info,*image,argv[i+2]);
            InheritException(exception,&(*image)->exception);
            if (arguments == (char *) NULL)
              break;
            p=(char *) arguments;
            for (x=0; *p != '\0'; x++)
            {
              GetNextToken(p,&p,MaxTextExtent,token);
              if (*token == ',')
                GetNextToken(p,&p,MaxTextExtent,token);
            }
            number_parameters=(size_t) x;
            parameters=(double *) AcquireQuantumMemory(number_parameters,
              sizeof(*parameters));
            if (parameters == (double *) NULL)
              ThrowWandFatalException(ResourceLimitFatalError,
                "MemoryAllocationFailed",(*image)->filename);
            (void) memset(parameters,0,number_parameters*
              sizeof(*parameters));
            p=(char *) arguments;
            for (x=0; (x < (ssize_t) number_parameters) && (*p != '\0'); x++)
            {
              GetNextToken(p,&p,MaxTextExtent,token);
              if (*token == ',')
                GetNextToken(p,&p,MaxTextExtent,token);
              parameters[x]=StringToDouble(token,(char **) NULL);
            }
            arguments=DestroyString(arguments);
            (void) FunctionImageChannel(*image,channel,function,
              number_parameters,parameters,exception);
            parameters=(double *) RelinquishMagickMemory(parameters);
            break;
          }
        break;
      }
      case 'g':
      {
        if (LocaleCompare("gamma",option+1) == 0)
          {
            /*
              Gamma image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              (*image)->gamma=StringToDouble(argv[i+1],(char **) NULL);
            else
              {
                if (strchr(argv[i+1],',') != (char *) NULL)
                  (void) GammaImage(*image,argv[i+1]);
                else
                  (void) GammaImageChannel(*image,channel,
                    StringToDouble(argv[i+1],(char **) NULL));
                InheritException(exception,&(*image)->exception);
              }
            break;
          }
        if ((LocaleCompare("gaussian-blur",option+1) == 0) ||
            (LocaleCompare("gaussian",option+1) == 0))
          {
            /*
              Gaussian blur image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=GaussianBlurImageChannel(*image,channel,
              geometry_info.rho,geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("geometry",option+1) == 0)
          {
              /*
                Record Image offset, Resize last image.
              */
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              {
                if ((*image)->geometry != (char *) NULL)
                  (*image)->geometry=DestroyString((*image)->geometry);
                break;
              }
            flags=ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            if (((flags & XValue) != 0) || ((flags & YValue) != 0))
              (void) CloneString(&(*image)->geometry,argv[i+1]);
            else
              mogrify_image=ResizeImage(*image,geometry.width,geometry.height,
                (*image)->filter,(*image)->blur,exception);
            break;
          }
        if (LocaleCompare("gravity",option+1) == 0)
          {
            if (*option == '+')
              {
                draw_info->gravity=UndefinedGravity;
                break;
              }
            draw_info->gravity=(GravityType) ParseCommandOption(
              MagickGravityOptions,MagickFalse,argv[i+1]);
            break;
          }
        if (LocaleCompare("grayscale",option+1) == 0)
          {
            PixelIntensityMethod
              method;

            (void) SyncImagesSettings(mogrify_info,*image);
            method=(PixelIntensityMethod) ParseCommandOption(
              MagickPixelIntensityOptions,MagickFalse,argv[i+1]);
            (void) GrayscaleImage(*image,method);
            InheritException(exception,&(*image)->exception);
            break;
          }
        break;
      }
      case 'h':
      {
        if (LocaleCompare("highlight-color",option+1) == 0)
          {
            (void) SetImageArtifact(*image,"compare:highlight-color",argv[i+1]);
            break;
          }
        if (LocaleCompare("hough-lines",option+1) == 0)
          {
            /*
              Identify lines in the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=geometry_info.rho;
            if ((flags & XiValue) == 0)
              geometry_info.xi=40;
            mogrify_image=HoughLineImage(*image,(size_t) geometry_info.rho,
              (size_t) geometry_info.sigma,(size_t) geometry_info.xi,exception);
            break;
          }
        break;
      }
      case 'i':
      {
        if (LocaleCompare("identify",option+1) == 0)
          {
            char
              *text;

            (void) SyncImageSettings(mogrify_info,*image);
            if (format == (char *) NULL)
              {
                (void) IdentifyImage(*image,stdout,mogrify_info->verbose);
                InheritException(exception,&(*image)->exception);
                break;
              }
            text=InterpretImageProperties(mogrify_info,*image,format);
            InheritException(exception,&(*image)->exception);
            if (text == (char *) NULL)
              break;
            (void) fputs(text,stdout);
            text=DestroyString(text);
            break;
          }
        if (LocaleCompare("implode",option+1) == 0)
          {
            /*
              Implode image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGeometry(argv[i+1],&geometry_info);
            mogrify_image=ImplodeImage(*image,geometry_info.rho,exception);
            break;
          }
        if (LocaleCompare("interline-spacing",option+1) == 0)
          {
            if (*option == '+')
              (void) ParseGeometry("0",&geometry_info);
            else
              (void) ParseGeometry(argv[i+1],&geometry_info);
            draw_info->interline_spacing=geometry_info.rho;
            break;
          }
        if (LocaleCompare("interword-spacing",option+1) == 0)
          {
            if (*option == '+')
              (void) ParseGeometry("0",&geometry_info);
            else
              (void) ParseGeometry(argv[i+1],&geometry_info);
            draw_info->interword_spacing=geometry_info.rho;
            break;
          }
        if (LocaleCompare("interpolative-resize",option+1) == 0)
          {
            /*
              Resize image using 'point sampled' interpolation
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=InterpolativeResizeImage(*image,geometry.width,
              geometry.height,(*image)->interpolate,exception);
            break;
          }
        break;
      }
      case 'k':
      {
        if (LocaleCompare("kerning",option+1) == 0)
          {
            if (*option == '+')
              (void) ParseGeometry("0",&geometry_info);
            else
              (void) ParseGeometry(argv[i+1],&geometry_info);
            draw_info->kerning=geometry_info.rho;
            break;
          }
        if (LocaleCompare("kuwahara",option+1) == 0)
          {
            /*
              Edge preserving blur.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=geometry_info.rho-0.5;
            mogrify_image=KuwaharaImageChannel(*image,channel,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        break;
      }
      case 'l':
      {
        if (LocaleCompare("lat",option+1) == 0)
          {
            /*
              Local adaptive threshold image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            if ((flags & PercentValue) != 0)
              geometry_info.xi=(double) QuantumRange*geometry_info.xi/100.0;
            mogrify_image=AdaptiveThresholdImage(*image,(size_t)
              geometry_info.rho,(size_t) geometry_info.sigma,(ssize_t)
              geometry_info.xi,exception);
            break;
          }
        if (LocaleCompare("level",option+1) == 0)
          {
            MagickRealType
              black_point,
              gamma,
              white_point;

            MagickStatusType
              flags;

            /*
              Parse levels.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            black_point=geometry_info.rho;
            white_point=(MagickRealType) QuantumRange;
            if ((flags & SigmaValue) != 0)
              white_point=geometry_info.sigma;
            gamma=1.0;
            if ((flags & XiValue) != 0)
              gamma=geometry_info.xi;
            if ((flags & PercentValue) != 0)
              {
                black_point*=(MagickRealType) (QuantumRange/100.0);
                white_point*=(MagickRealType) (QuantumRange/100.0);
              }
            if ((flags & SigmaValue) == 0)
              white_point=(MagickRealType) QuantumRange-black_point;
            if ((*option == '+') || ((flags & AspectValue) != 0))
              (void) LevelizeImageChannel(*image,channel,black_point,
                white_point,gamma);
            else
              (void) LevelImageChannel(*image,channel,black_point,white_point,
                gamma);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("level-colors",option+1) == 0)
          {
            char
              token[MaxTextExtent];

            const char
              *p;

            MagickPixelPacket
              black_point,
              white_point;

            p=(const char *) argv[i+1];
            GetNextToken(p,&p,MaxTextExtent,token);  /* get black point color */
            if ((isalpha((int) *token) != 0) || ((*token == '#') != 0))
              (void) QueryMagickColor(token,&black_point,exception);
            else
              (void) QueryMagickColor("#000000",&black_point,exception);
            if (isalpha((int) token[0]) || (token[0] == '#'))
              GetNextToken(p,&p,MaxTextExtent,token);
            if (*token == '\0')
              white_point=black_point; /* set everything to that color */
            else
              {
                if ((isalpha((int) *token) == 0) && ((*token == '#') == 0))
                  GetNextToken(p,&p,MaxTextExtent,token); /* Get white point color. */
                if ((isalpha((int) *token) != 0) || ((*token == '#') != 0))
                  (void) QueryMagickColor(token,&white_point,exception);
                else
                  (void) QueryMagickColor("#ffffff",&white_point,exception);
              }
            (void) LevelColorsImageChannel(*image,channel,&black_point,
              &white_point,*option == '+' ? MagickTrue : MagickFalse);
            break;
          }
        if (LocaleCompare("linear-stretch",option+1) == 0)
          {
            double
              black_point,
              white_point;

            MagickStatusType
              flags;

            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            black_point=geometry_info.rho;
            white_point=(MagickRealType) (*image)->columns*(*image)->rows;
            if ((flags & SigmaValue) != 0)
              white_point=geometry_info.sigma;
            if ((flags & PercentValue) != 0)
              {
                black_point*=(double) (*image)->columns*(*image)->rows/100.0;
                white_point*=(double) (*image)->columns*(*image)->rows/100.0;
              }
            if ((flags & SigmaValue) == 0)
              white_point=(MagickRealType) (*image)->columns*(*image)->rows-
                black_point;
            (void) LinearStretchImage(*image,black_point,white_point);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("linewidth",option+1) == 0)
          {
            draw_info->stroke_width=StringToDouble(argv[i+1],(char **) NULL);
            break;
          }
        if (LocaleCompare("liquid-rescale",option+1) == 0)
          {
            /*
              Liquid rescale image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            if ((flags & XValue) == 0)
              geometry.x=1;
            if ((flags & YValue) == 0)
              geometry.y=0;
            mogrify_image=LiquidRescaleImage(*image,geometry.width,
              geometry.height,1.0*geometry.x,1.0*geometry.y,exception);
            break;
          }
        if (LocaleCompare("local-contrast",option+1) == 0)
          {
            MagickStatusType
              flags;

            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & RhoValue) == 0)
              geometry_info.rho=10;
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=12.5;
            mogrify_image=LocalContrastImage(*image,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("lowlight-color",option+1) == 0)
          {
            (void) SetImageArtifact(*image,"compare:lowlight-color",argv[i+1]);
            break;
          }
        break;
      }
      case 'm':
      {
        if (LocaleCompare("magnify",option+1) == 0)
          {
            /*
              Double image size.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=MagnifyImage(*image,exception);
            break;
          }
        if (LocaleCompare("map",option+1) == 0)
          {
            Image
              *remap_image;

            /*
              Transform image colors to match this set of colors.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              break;
            remap_image=GetImageCache(mogrify_info,argv[i+1],exception);
            if (remap_image == (Image *) NULL)
              break;
            (void) RemapImage(quantize_info,*image,remap_image);
            InheritException(exception,&(*image)->exception);
            remap_image=DestroyImage(remap_image);
            break;
          }
        if (LocaleCompare("mask",option+1) == 0)
          {
            Image
              *mask;

            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              {
                /*
                  Remove a mask.
                */
                (void) SetImageMask(*image,(Image *) NULL);
                InheritException(exception,&(*image)->exception);
                break;
              }
            /*
              Set the image mask.
            */
            mask=GetImageCache(mogrify_info,argv[i+1],exception);
            if (mask == (Image *) NULL)
              break;
            (void) SetImageMask(*image,mask);
            mask=DestroyImage(mask);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("matte",option+1) == 0)
          {
            (void) SetImageAlphaChannel(*image,(*option == '-') ?
              SetAlphaChannel : DeactivateAlphaChannel );
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("mean-shift",option+1) == 0)
          {
            /*
              Delineate arbitrarily shaped clusters in the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=geometry_info.rho;
            if ((flags & XiValue) == 0)
              geometry_info.xi=0.10*QuantumRange;
            if ((flags & PercentValue) != 0)
              geometry_info.xi=(double) QuantumRange*geometry_info.xi/100.0;
            mogrify_image=MeanShiftImage(*image,(size_t) geometry_info.rho,
              (size_t) geometry_info.sigma,(size_t) geometry_info.xi,exception);
            break;
          }
        if (LocaleCompare("median",option+1) == 0)
          {
            /*
              Median filter image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGeometry(argv[i+1],&geometry_info);
            mogrify_image=StatisticImageChannel(*image,channel,MedianStatistic,
              (size_t) geometry_info.rho,(size_t) geometry_info.rho,exception);
            break;
          }
        if (LocaleCompare("mode",option+1) == 0)
          {
            /*
              Mode image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGeometry(argv[i+1],&geometry_info);
            mogrify_image=StatisticImageChannel(*image,channel,ModeStatistic,
              (size_t) geometry_info.rho,(size_t) geometry_info.rho,exception);
            break;
          }
        if (LocaleCompare("modulate",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ModulateImage(*image,argv[i+1]);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("moments",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) DeleteImageArtifact(*image,"identify:moments");
                break;
              }
            (void) SetImageArtifact(*image,"identify:moments",argv[i+1]);
            (void) SetImageArtifact(*image,"verbose","true");
            break;
          }
        if (LocaleCompare("monitor",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) SetImageProgressMonitor(*image,
                  (MagickProgressMonitor) NULL,(void *) NULL);
                break;
              }
            (void) SetImageProgressMonitor(*image,MonitorProgress,
              (void *) NULL);
            break;
          }
        if (LocaleCompare("monochrome",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) SetImageType(*image,BilevelType);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("morphology",option+1) == 0)
          {
            char
              token[MaxTextExtent];

            const char
              *p;

            KernelInfo
              *kernel;

            MorphologyMethod
              method;

            ssize_t
              iterations;

            /*
              Morphological Image Operation
            */
            (void) SyncImageSettings(mogrify_info,*image);
            p=argv[i+1];
            GetNextToken(p,&p,MaxTextExtent,token);
            method=(MorphologyMethod) ParseCommandOption(
              MagickMorphologyOptions,MagickFalse,token);
            iterations=1L;
            GetNextToken(p,&p,MaxTextExtent,token);
            if ((*p == ':') || (*p == ','))
              GetNextToken(p,&p,MaxTextExtent,token);
            if ((*p != '\0'))
              iterations=(ssize_t) StringToLong(p);
            kernel=AcquireKernelInfo(argv[i+2]);
            if (kernel == (KernelInfo *) NULL)
              {
                (void) ThrowMagickException(exception,GetMagickModule(),
                  OptionError,"UnabletoParseKernel","morphology");
                status=MagickFalse;
                break;
              }
            mogrify_image=MorphologyImageChannel(*image,channel,method,
              iterations,kernel,exception);
            kernel=DestroyKernelInfo(kernel);
            break;
          }
        if (LocaleCompare("motion-blur",option+1) == 0)
          {
            /*
              Motion blur image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=MotionBlurImageChannel(*image,channel,
              geometry_info.rho,geometry_info.sigma,geometry_info.xi,exception);
            break;
          }
        break;
      }
      case 'n':
      {
        if (LocaleCompare("negate",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) NegateImageChannel(*image,channel,*option == '+' ?
              MagickTrue : MagickFalse);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("noise",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '-')
              {
                flags=ParseGeometry(argv[i+1],&geometry_info);
                if ((flags & SigmaValue) == 0)
                  geometry_info.sigma=geometry_info.rho;
                mogrify_image=StatisticImageChannel(*image,channel,
                  NonpeakStatistic,(size_t) geometry_info.rho,(size_t)
                  geometry_info.sigma,exception);
              }
            else
              {
                NoiseType
                  noise;

                noise=(NoiseType) ParseCommandOption(MagickNoiseOptions,
                  MagickFalse,argv[i+1]);
                mogrify_image=AddNoiseImageChannel(*image,channel,noise,
                  exception);
              }
            break;
          }
        if (LocaleCompare("normalize",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) NormalizeImageChannel(*image,channel);
            InheritException(exception,&(*image)->exception);
            break;
          }
        break;
      }
      case 'o':
      {
        if (LocaleCompare("opaque",option+1) == 0)
          {
            MagickPixelPacket
              target;

            (void) SyncImageSettings(mogrify_info,*image);
            (void) QueryMagickColor(argv[i+1],&target,exception);
            (void) OpaquePaintImageChannel(*image,channel,&target,&fill,
              *option == '-' ? MagickFalse : MagickTrue);
            break;
          }
        if (LocaleCompare("ordered-dither",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) OrderedPosterizeImageChannel(*image,channel,argv[i+1],
              exception);
            break;
          }
        break;
      }
      case 'p':
      {
        if (LocaleCompare("paint",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGeometry(argv[i+1],&geometry_info);
            mogrify_image=OilPaintImage(*image,geometry_info.rho,exception);
            break;
          }
        if (LocaleCompare("pen",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) QueryColorDatabase("none",&draw_info->fill,exception);
                break;
              }
            (void) QueryColorDatabase(argv[i+1],&draw_info->fill,exception);
            break;
          }
        if (LocaleCompare("perceptible",option+1) == 0)
          {
            /*
              Perceptible image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) PerceptibleImageChannel(*image,channel,StringToDouble(
              argv[i+1],(char **) NULL));
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("pointsize",option+1) == 0)
          {
            if (*option == '+')
              (void) ParseGeometry("12",&geometry_info);
            else
              (void) ParseGeometry(argv[i+1],&geometry_info);
            draw_info->pointsize=geometry_info.rho;
            break;
          }
        if (LocaleCompare("polaroid",option+1) == 0)
          {
            double
              angle;

            RandomInfo
              *random_info;

            /*
              Simulate a Polaroid picture.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            random_info=AcquireRandomInfo();
            angle=22.5*(GetPseudoRandomValue(random_info)-0.5);
            random_info=DestroyRandomInfo(random_info);
            if (*option == '-')
              {
                SetGeometryInfo(&geometry_info);
                flags=ParseGeometry(argv[i+1],&geometry_info);
                angle=geometry_info.rho;
              }
            mogrify_image=PolaroidImage(*image,draw_info,angle,exception);
            break;
          }
        if (LocaleCompare("posterize",option+1) == 0)
          {
            /*
              Posterize image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) PosterizeImage(*image,StringToUnsignedLong(argv[i+1]),
              quantize_info->dither);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("preview",option+1) == 0)
          {
            PreviewType
              preview_type;

            /*
              Preview image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              preview_type=UndefinedPreview;
            else
              preview_type=(PreviewType) ParseCommandOption(
                MagickPreviewOptions,MagickFalse,argv[i+1]);
            mogrify_image=PreviewImage(*image,preview_type,exception);
            break;
          }
        if (LocaleCompare("profile",option+1) == 0)
          {
            const char
              *name;

            const StringInfo
              *profile;

            Image
              *profile_image;

            ImageInfo
              *profile_info;

            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              {
                /*
                  Remove a profile from the image.
                */
                (void) ProfileImage(*image,argv[i+1],(const unsigned char *)
                  NULL,0,MagickTrue);
                InheritException(exception,&(*image)->exception);
                break;
              }
            /*
              Associate a profile with the image.
            */
            profile_info=CloneImageInfo(mogrify_info);
            profile=GetImageProfile(*image,"iptc");
            if (profile != (StringInfo *) NULL)
              profile_info->profile=(void *) CloneStringInfo(profile);
            profile_image=GetImageCache(profile_info,argv[i+1],exception);
            profile_info=DestroyImageInfo(profile_info);
            if (profile_image == (Image *) NULL)
              {
                StringInfo
                  *profile;

                profile_info=CloneImageInfo(mogrify_info);
                (void) CopyMagickString(profile_info->filename,argv[i+1],
                  MaxTextExtent);
                profile=FileToStringInfo(profile_info->filename,~0UL,exception);
                if (profile != (StringInfo *) NULL)
                  {
                    (void) SetImageInfo(profile_info,0,exception);
                    (void) ProfileImage(*image,profile_info->magick,
                      GetStringInfoDatum(profile),(size_t)
                      GetStringInfoLength(profile),MagickFalse);
                    profile=DestroyStringInfo(profile);
                  }
                profile_info=DestroyImageInfo(profile_info);
                break;
              }
            ResetImageProfileIterator(profile_image);
            name=GetNextImageProfile(profile_image);
            while (name != (const char *) NULL)
            {
              profile=GetImageProfile(profile_image,name);
              if (profile != (StringInfo *) NULL)
                (void) ProfileImage(*image,name,GetStringInfoDatum(profile),
                  (size_t) GetStringInfoLength(profile),MagickFalse);
              name=GetNextImageProfile(profile_image);
            }
            profile_image=DestroyImage(profile_image);
            break;
          }
        break;
      }
      case 'q':
      {
        if (LocaleCompare("quantize",option+1) == 0)
          {
            if (*option == '+')
              {
                quantize_info->colorspace=UndefinedColorspace;
                break;
              }
            quantize_info->colorspace=(ColorspaceType) ParseCommandOption(
              MagickColorspaceOptions,MagickFalse,argv[i+1]);
            break;
          }
        break;
      }
      case 'r':
      {
        if (LocaleCompare("radial-blur",option+1) == 0 ||
            LocaleCompare("rotational-blur",option+1) == 0)
          {
            /*
              Radial blur image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=RotationalBlurImageChannel(*image,channel,
              StringToDouble(argv[i+1],(char **) NULL),exception);
            break;
          }
        if (LocaleCompare("raise",option+1) == 0)
          {
            /*
              Surround image with a raise of solid color.
            */
            flags=ParsePageGeometry(*image,argv[i+1],&geometry,exception);
            (void) RaiseImage(*image,&geometry,*option == '-' ? MagickTrue :
              MagickFalse);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("random-threshold",option+1) == 0)
          {
            /*
              Threshold image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) RandomThresholdImageChannel(*image,channel,argv[i+1],
              exception);
            break;
          }
        if (LocaleCompare("recolor",option+1) == 0)
          {
            KernelInfo
              *kernel;

            (void) SyncImageSettings(mogrify_info,*image);
            kernel=AcquireKernelInfo(argv[i+1]);
            if (kernel == (KernelInfo *) NULL)
              break;
            mogrify_image=ColorMatrixImage(*image,kernel,exception);
            kernel=DestroyKernelInfo(kernel);
            break;
          }
        if (LocaleCompare("region",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            if (region_image != (Image *) NULL)
              {
                /*
                  Composite region.
                */
                (void) CompositeImage(region_image,region_image->matte !=
                  MagickFalse ? CopyCompositeOp : OverCompositeOp,*image,
                  region_geometry.x,region_geometry.y);
                InheritException(exception,&region_image->exception);
                *image=DestroyImage(*image);
                *image=region_image;
                region_image=(Image *) NULL;
              }
            if (*option == '+')
              break;
            /*
              Apply transformations to a selected region of the image.
            */
            (void) ParseGravityGeometry(*image,argv[i+1],&region_geometry,
              exception);
            mogrify_image=CropImage(*image,&region_geometry,exception);
            if (mogrify_image == (Image *) NULL)
              break;
            region_image=(*image);
            *image=mogrify_image;
            mogrify_image=(Image *) NULL;
            break;
          }
        if (LocaleCompare("render",option+1) == 0)
          {
            (void) SyncImageSettings(mogrify_info,*image);
            draw_info->render=(*option == '+') ? MagickTrue : MagickFalse;
            break;
          }
        if (LocaleCompare("remap",option+1) == 0)
          {
            Image
              *remap_image;

            /*
              Transform image colors to match this set of colors.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              break;
            remap_image=GetImageCache(mogrify_info,argv[i+1],exception);
            if (remap_image == (Image *) NULL)
              break;
            (void) RemapImage(quantize_info,*image,remap_image);
            InheritException(exception,&(*image)->exception);
            remap_image=DestroyImage(remap_image);
            break;
          }
        if (LocaleCompare("repage",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) ParseAbsoluteGeometry("0x0+0+0",&(*image)->page);
                break;
              }
            (void) ResetImagePage(*image,argv[i+1]);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("resample",option+1) == 0)
          {
            /*
              Resample image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=geometry_info.rho;
            mogrify_image=ResampleImage(*image,geometry_info.rho,
              geometry_info.sigma,(*image)->filter,(*image)->blur,exception);
            break;
          }
        if (LocaleCompare("resize",option+1) == 0)
          {
            /*
              Resize image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=ResizeImage(*image,geometry.width,geometry.height,
              (*image)->filter,(*image)->blur,exception);
            break;
          }
        if (LocaleCompare("roll",option+1) == 0)
          {
            /*
              Roll image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParsePageGeometry(*image,argv[i+1],&geometry,exception);
            if ((flags & PercentValue) != 0)
              {
                geometry.x*=(double) (*image)->columns/100.0;
                geometry.y*=(double) (*image)->rows/100.0;
              }
            mogrify_image=RollImage(*image,geometry.x,geometry.y,exception);
            break;
          }
        if (LocaleCompare("rotate",option+1) == 0)
          {
            char
              *geometry;

            /*
              Check for conditional image rotation.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            if (strchr(argv[i+1],'>') != (char *) NULL)
              if ((*image)->columns <= (*image)->rows)
                break;
            if (strchr(argv[i+1],'<') != (char *) NULL)
              if ((*image)->columns >= (*image)->rows)
                break;
            /*
              Rotate image.
            */
            geometry=ConstantString(argv[i+1]);
            (void) SubstituteString(&geometry,">","");
            (void) SubstituteString(&geometry,"<","");
            (void) ParseGeometry(geometry,&geometry_info);
            geometry=DestroyString(geometry);
            mogrify_image=RotateImage(*image,geometry_info.rho,exception);
            break;
          }
        break;
      }
      case 's':
      {
        if (LocaleCompare("sample",option+1) == 0)
          {
            /*
              Sample image with pixel replication.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=SampleImage(*image,geometry.width,geometry.height,
              exception);
            break;
          }
        if (LocaleCompare("scale",option+1) == 0)
          {
            /*
              Resize image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=ScaleImage(*image,geometry.width,geometry.height,
              exception);
            break;
          }
        if (LocaleCompare("selective-blur",option+1) == 0)
          {
            /*
              Selectively blur pixels within a contrast threshold.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & PercentValue) != 0)
              geometry_info.xi=(double) QuantumRange*geometry_info.xi/100.0;
            mogrify_image=SelectiveBlurImageChannel(*image,channel,
              geometry_info.rho,geometry_info.sigma,geometry_info.xi,exception);
            break;
          }
        if (LocaleCompare("separate",option+1) == 0)
          {
            /*
              Break channels into separate images.
              WARNING: This can generate multiple images!
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=SeparateImages(*image,channel,exception);
            break;
          }
        if (LocaleCompare("sepia-tone",option+1) == 0)
          {
            double
              threshold;

            /*
              Sepia-tone image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            threshold=StringToDoubleInterval(argv[i+1],(double) QuantumRange+
              1.0);
            mogrify_image=SepiaToneImage(*image,threshold,exception);
            break;
          }
        if (LocaleCompare("segment",option+1) == 0)
          {
            /*
              Segment image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            (void) SegmentImage(*image,(*image)->colorspace,
              mogrify_info->verbose,geometry_info.rho,geometry_info.sigma);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("set",option+1) == 0)
          {
            char
              *value;

            /*
              Set image option.
            */
            if (*option == '+')
              {
                if (LocaleNCompare(argv[i+1],"registry:",9) == 0)
                  (void) DeleteImageRegistry(argv[i+1]+9);
                else
                  if (LocaleNCompare(argv[i+1],"option:",7) == 0)
                    {
                      (void) DeleteImageOption(mogrify_info,argv[i+1]+7);
                      (void) DeleteImageArtifact(*image,argv[i+1]+7);
                    }
                  else
                    (void) DeleteImageProperty(*image,argv[i+1]);
                break;
              }
            value=InterpretImageProperties(mogrify_info,*image,argv[i+2]);
            InheritException(exception,&(*image)->exception);
            if (value == (char *) NULL)
              break;
            if (LocaleNCompare(argv[i+1],"registry:",9) == 0)
              (void) SetImageRegistry(StringRegistryType,argv[i+1]+9,value,
                exception);
            else
              if (LocaleNCompare(argv[i+1],"option:",7) == 0)
                {
                  (void) SetImageOption(image_info,argv[i+1]+7,value);
                  (void) SetImageOption(mogrify_info,argv[i+1]+7,value);
                  (void) SetImageArtifact(*image,argv[i+1]+7,value);
                }
              else
                (void) SetImageProperty(*image,argv[i+1],value);
            value=DestroyString(value);
            break;
          }
        if (LocaleCompare("shade",option+1) == 0)
          {
            /*
              Shade image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=ShadeImage(*image,(*option == '-') ? MagickTrue :
              MagickFalse,geometry_info.rho,geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("shadow",option+1) == 0)
          {
            /*
              Shadow image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            if ((flags & XiValue) == 0)
              geometry_info.xi=4.0;
            if ((flags & PsiValue) == 0)
              geometry_info.psi=4.0;
            mogrify_image=ShadowImage(*image,geometry_info.rho,
              geometry_info.sigma,(ssize_t) ceil(geometry_info.xi-0.5),(ssize_t)
              ceil(geometry_info.psi-0.5),exception);
            break;
          }
        if (LocaleCompare("sharpen",option+1) == 0)
          {
            /*
              Sharpen image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=SharpenImageChannel(*image,channel,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("shave",option+1) == 0)
          {
            /*
              Shave the image edges.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParsePageGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=ShaveImage(*image,&geometry,exception);
            break;
          }
        if (LocaleCompare("shear",option+1) == 0)
          {
            /*
              Shear image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=geometry_info.rho;
            mogrify_image=ShearImage(*image,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("sigmoidal-contrast",option+1) == 0)
          {
            /*
              Sigmoidal non-linearity contrast control.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=(double) QuantumRange/2.0;
            if ((flags & PercentValue) != 0)
              geometry_info.sigma=(double) QuantumRange*geometry_info.sigma/
                100.0;
            (void) SigmoidalContrastImageChannel(*image,channel,
              (*option == '-') ? MagickTrue : MagickFalse,geometry_info.rho,
              geometry_info.sigma);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("sketch",option+1) == 0)
          {
            /*
              Sketch image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=SketchImage(*image,geometry_info.rho,
              geometry_info.sigma,geometry_info.xi,exception);
            break;
          }
        if (LocaleCompare("solarize",option+1) == 0)
          {
            double
              threshold;

            (void) SyncImageSettings(mogrify_info,*image);
            threshold=StringToDoubleInterval(argv[i+1],(double) QuantumRange+
              1.0);
            (void) SolarizeImageChannel(*image,channel,threshold,exception);
            break;
          }
        if (LocaleCompare("sparse-color",option+1) == 0)
          {
            SparseColorMethod
              method;

            char
              *arguments;

            /*
              Sparse Color Interpolated Gradient
            */
            (void) SyncImageSettings(mogrify_info,*image);
            method=(SparseColorMethod) ParseCommandOption(
              MagickSparseColorOptions,MagickFalse,argv[i+1]);
            arguments=InterpretImageProperties(mogrify_info,*image,argv[i+2]);
            InheritException(exception,&(*image)->exception);
            if (arguments == (char *) NULL)
              break;
            mogrify_image=SparseColorOption(*image,channel,method,arguments,
              option[0] == '+' ? MagickTrue : MagickFalse,exception);
            arguments=DestroyString(arguments);
            break;
          }
        if (LocaleCompare("splice",option+1) == 0)
          {
            /*
              Splice a solid color into the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGravityGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=SpliceImage(*image,&geometry,exception);
            break;
          }
        if (LocaleCompare("spread",option+1) == 0)
          {
            /*
              Spread an image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGeometry(argv[i+1],&geometry_info);
            mogrify_image=SpreadImage(*image,geometry_info.rho,exception);
            break;
          }
        if (LocaleCompare("statistic",option+1) == 0)
          {
            StatisticType
              type;

            (void) SyncImageSettings(mogrify_info,*image);
            type=(StatisticType) ParseCommandOption(MagickStatisticOptions,
              MagickFalse,argv[i+1]);
            (void) ParseGeometry(argv[i+2],&geometry_info);
            mogrify_image=StatisticImageChannel(*image,channel,type,(size_t)
              geometry_info.rho,(size_t) geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("stretch",option+1) == 0)
          {
            if (*option == '+')
              {
                draw_info->stretch=UndefinedStretch;
                break;
              }
            draw_info->stretch=(StretchType) ParseCommandOption(
              MagickStretchOptions,MagickFalse,argv[i+1]);
            break;
          }
        if (LocaleCompare("strip",option+1) == 0)
          {
            /*
              Strip image of profiles and comments.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) StripImage(*image);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("stroke",option+1) == 0)
          {
            ExceptionInfo
              *sans;

            if (*option == '+')
              {
                (void) QueryColorDatabase("none",&draw_info->stroke,exception);
                if (draw_info->stroke_pattern != (Image *) NULL)
                  draw_info->stroke_pattern=DestroyImage(
                    draw_info->stroke_pattern);
                break;
              }
            sans=AcquireExceptionInfo();
            status=QueryColorDatabase(argv[i+1],&draw_info->stroke,sans);
            sans=DestroyExceptionInfo(sans);
            if (status == MagickFalse)
              draw_info->stroke_pattern=GetImageCache(mogrify_info,argv[i+1],
                exception);
            break;
          }
        if (LocaleCompare("strokewidth",option+1) == 0)
          {
            draw_info->stroke_width=StringToDouble(argv[i+1],(char **) NULL);
            break;
          }
        if (LocaleCompare("style",option+1) == 0)
          {
            if (*option == '+')
              {
                draw_info->style=UndefinedStyle;
                break;
              }
            draw_info->style=(StyleType) ParseCommandOption(MagickStyleOptions,
              MagickFalse,argv[i+1]);
            break;
          }
        if (LocaleCompare("swirl",option+1) == 0)
          {
            /*
              Swirl image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseGeometry(argv[i+1],&geometry_info);
            mogrify_image=SwirlImage(*image,geometry_info.rho,exception);
            break;
          }
        break;
      }
      case 't':
      {
        if (LocaleCompare("threshold",option+1) == 0)
          {
            double
              threshold;

            /*
              Threshold image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              threshold=(double) QuantumRange/2;
            else
              threshold=StringToDoubleInterval(argv[i+1],(double) QuantumRange+
                1.0);
            (void) BilevelImageChannel(*image,channel,threshold);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("thumbnail",option+1) == 0)
          {
            /*
              Thumbnail image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) ParseRegionGeometry(*image,argv[i+1],&geometry,exception);
            mogrify_image=ThumbnailImage(*image,geometry.width,geometry.height,
              exception);
            break;
          }
        if (LocaleCompare("tile",option+1) == 0)
          {
            if (*option == '+')
              {
                if (draw_info->fill_pattern != (Image *) NULL)
                  draw_info->fill_pattern=DestroyImage(draw_info->fill_pattern);
                break;
              }
            draw_info->fill_pattern=GetImageCache(mogrify_info,argv[i+1],
              exception);
            break;
          }
        if (LocaleCompare("tint",option+1) == 0)
          {
            /*
              Tint the image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=TintImage(*image,argv[i+1],draw_info->fill,exception);
            break;
          }
        if (LocaleCompare("transform",option+1) == 0)
          {
            /*
              Affine transform image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=AffineTransformImage(*image,&draw_info->affine,
              exception);
            break;
          }
        if (LocaleCompare("transparent",option+1) == 0)
          {
            MagickPixelPacket
              target;

            (void) SyncImageSettings(mogrify_info,*image);
            (void) QueryMagickColor(argv[i+1],&target,exception);
            (void) TransparentPaintImage(*image,&target,(Quantum)
              TransparentOpacity,*option == '-' ? MagickFalse : MagickTrue);
            InheritException(exception,&(*image)->exception);
            break;
          }
        if (LocaleCompare("transpose",option+1) == 0)
          {
            /*
              Transpose image scanlines.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=TransposeImage(*image,exception);
            break;
          }
        if (LocaleCompare("transverse",option+1) == 0)
          {
            /*
              Transverse image scanlines.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=TransverseImage(*image,exception);
            break;
          }
        if (LocaleCompare("treedepth",option+1) == 0)
          {
            quantize_info->tree_depth=StringToUnsignedLong(argv[i+1]);
            break;
          }
        if (LocaleCompare("trim",option+1) == 0)
          {
            /*
              Trim image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=TrimImage(*image,exception);
            break;
          }
        if (LocaleCompare("type",option+1) == 0)
          {
            ImageType
              type;

            (void) SyncImageSettings(mogrify_info,*image);
            if (*option == '+')
              type=UndefinedType;
            else
              type=(ImageType) ParseCommandOption(MagickTypeOptions,MagickFalse,
                argv[i+1]);
            (*image)->type=UndefinedType;
            (void) SetImageType(*image,type);
            InheritException(exception,&(*image)->exception);
            break;
          }
        break;
      }
      case 'u':
      {
        if (LocaleCompare("undercolor",option+1) == 0)
          {
            (void) QueryColorDatabase(argv[i+1],&draw_info->undercolor,
              exception);
            break;
          }
        if (LocaleCompare("unique",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) DeleteImageArtifact(*image,"identify:unique-colors");
                break;
              }
            (void) SetImageArtifact(*image,"identify:unique-colors","true");
            (void) SetImageArtifact(*image,"verbose","true");
            break;
          }
        if (LocaleCompare("unique-colors",option+1) == 0)
          {
            /*
              Unique image colors.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            mogrify_image=UniqueImageColors(*image,exception);
            break;
          }
        if (LocaleCompare("unsharp",option+1) == 0)
          {
            /*
              Unsharp mask image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            if ((flags & XiValue) == 0)
              geometry_info.xi=1.0;
            if ((flags & PsiValue) == 0)
              geometry_info.psi=0.05;
            mogrify_image=UnsharpMaskImageChannel(*image,channel,
              geometry_info.rho,geometry_info.sigma,geometry_info.xi,
              geometry_info.psi,exception);
            break;
          }
        break;
      }
      case 'v':
      {
        if (LocaleCompare("verbose",option+1) == 0)
          {
            (void) SetImageArtifact(*image,option+1,
              *option == '+' ? "false" : "true");
            break;
          }
        if (LocaleCompare("vignette",option+1) == 0)
          {
            /*
              Vignette image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            if ((flags & XiValue) == 0)
              geometry_info.xi=0.1*(*image)->columns;
            if ((flags & PsiValue) == 0)
              geometry_info.psi=0.1*(*image)->rows;
            if ((flags & PercentValue) != 0)
              {
                geometry_info.xi*=(double) (*image)->columns/100.0;
                geometry_info.psi*=(double) (*image)->rows/100.0;
              }
            mogrify_image=VignetteImage(*image,geometry_info.rho,
              geometry_info.sigma,(ssize_t) ceil(geometry_info.xi-0.5),(ssize_t)
              ceil(geometry_info.psi-0.5),exception);
            break;
          }
        if (LocaleCompare("virtual-pixel",option+1) == 0)
          {
            if (*option == '+')
              {
                (void) SetImageVirtualPixelMethod(*image,
                  UndefinedVirtualPixelMethod);
                break;
              }
            (void) SetImageVirtualPixelMethod(*image,(VirtualPixelMethod)
              ParseCommandOption(MagickVirtualPixelOptions,MagickFalse,
              argv[i+1]));
            break;
          }
        break;
      }
      case 'w':
      {
        if (LocaleCompare("wave",option+1) == 0)
          {
            /*
              Wave image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=1.0;
            mogrify_image=WaveImage(*image,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("wavelet-denoise",option+1) == 0)
          {
            /*
              Wavelet denoise image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            flags=ParseGeometry(argv[i+1],&geometry_info);
            if ((flags & PercentValue) != 0)
              {
                geometry_info.rho=QuantumRange*geometry_info.rho/100.0;
                geometry_info.sigma=QuantumRange*geometry_info.sigma/100.0;
              }
            if ((flags & SigmaValue) == 0)
              geometry_info.sigma=0.0;
            mogrify_image=WaveletDenoiseImage(*image,geometry_info.rho,
              geometry_info.sigma,exception);
            break;
          }
        if (LocaleCompare("weight",option+1) == 0)
          {
            ssize_t
              weight;

            weight=ParseCommandOption(MagickWeightOptions,MagickFalse,
              argv[i+1]);
            if (weight == -1)
              weight=StringToUnsignedLong(argv[i+1]);
            draw_info->weight=(size_t) weight;
            break;
          }
        if (LocaleCompare("white-threshold",option+1) == 0)
          {
            /*
              White threshold image.
            */
            (void) SyncImageSettings(mogrify_info,*image);
            (void) WhiteThresholdImageChannel(*image,channel,argv[i+1],
              exception);
            InheritException(exception,&(*image)->exception);
            break;
          }
        break;
      }
      default:
        break;
    }
    /*
      Replace current image with any image that was generated.
    */
    if (mogrify_image != (Image *) NULL)
      ReplaceImageInListReturnLast(image,mogrify_image);
    i+=count;
  }
  if (region_image != (Image *) NULL)
    {
      /*
        Composite transformed region onto image.
      */
      (void) SyncImageSettings(mogrify_info,*image);
      (void) CompositeImage(region_image,region_image->matte != MagickFalse ?
        CopyCompositeOp : OverCompositeOp,*image,region_geometry.x,
        region_geometry.y);
      InheritException(exception,&region_image->exception);
      *image=DestroyImage(*image);
      *image=region_image;
      region_image = (Image *) NULL;
    }
  /*
    Free resources.
  */
  quantize_info=DestroyQuantizeInfo(quantize_info);
  draw_info=DestroyDrawInfo(draw_info);
  mogrify_info=DestroyImageInfo(mogrify_info);
  status=(MagickStatusType) (exception->severity < ErrorException ? 1 : 0);
  return(status == 0 ? MagickFalse : MagickTrue);
}
