MagickExport size_t ImportQuantumPixels(const Image *image,
  CacheView *image_view,QuantumInfo *quantum_info,
  const QuantumType quantum_type,const unsigned char *magick_restrict pixels,
  ExceptionInfo *exception)
{
  MagickSizeType
    number_pixels;

  register const unsigned char
    *magick_restrict p;

  register ssize_t
    x;

  register Quantum
    *magick_restrict q;

  size_t
    extent;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickCoreSignature);
  if (pixels == (const unsigned char *) NULL)
    pixels=(const unsigned char *) GetQuantumPixels(quantum_info);
  x=0;
  p=pixels;
  if (image_view == (CacheView *) NULL)
    {
      number_pixels=GetImageExtent(image);
      q=GetAuthenticPixelQueue(image);
    }
  else
    {
      number_pixels=GetCacheViewExtent(image_view);
      q=GetCacheViewAuthenticPixelQueue(image_view);
    }
  ResetQuantumState(quantum_info);
  extent=GetQuantumExtent(image,quantum_info,quantum_type);
  switch (quantum_type)
  {
    case AlphaQuantum:
    {
      ImportAlphaQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case BGRQuantum:
    {
      ImportBGRQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case BGRAQuantum:
    {
      ImportBGRAQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case BGROQuantum:
    {
      ImportBGROQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case BlackQuantum:
    {
      ImportBlackQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case BlueQuantum:
    case YellowQuantum:
    {
      ImportBlueQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case CMYKQuantum:
    {
      ImportCMYKQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case CMYKAQuantum:
    {
      ImportCMYKAQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case CMYKOQuantum:
    {
      ImportCMYKOQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case CbYCrYQuantum:
    {
      ImportCbYCrYQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case GrayQuantum:
    {
      ImportGrayQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case GrayAlphaQuantum:
    {
      ImportGrayAlphaQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case GreenQuantum:
    case MagentaQuantum:
    {
      ImportGreenQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case IndexQuantum:
    {
      ImportIndexQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case IndexAlphaQuantum:
    {
      ImportIndexAlphaQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case OpacityQuantum:
    {
      ImportOpacityQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case RedQuantum:
    case CyanQuantum:
    {
      ImportRedQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case RGBQuantum:
    case CbYCrQuantum:
    {
      ImportRGBQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case RGBAQuantum:
    case CbYCrAQuantum:
    {
      ImportRGBAQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    case RGBOQuantum:
    {
      ImportRGBOQuantum(image,quantum_info,number_pixels,p,q,exception);
      break;
    }
    default:
      break;
  }
  if ((quantum_type == CbYCrQuantum) || (quantum_type == CbYCrAQuantum))
    {
      Quantum
        quantum;

      register Quantum
        *magick_restrict q;

      q=GetAuthenticPixelQueue(image);
      if (image_view != (CacheView *) NULL)
        q=GetCacheViewAuthenticPixelQueue(image_view);
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        quantum=GetPixelRed(image,q);
        SetPixelRed(image,GetPixelGreen(image,q),q);
        SetPixelGreen(image,quantum,q);
        q+=GetPixelChannels(image);
      }
    }
  if (quantum_info->alpha_type == DisassociatedQuantumAlpha)
    {
      double
        gamma,
        Sa;

      register Quantum
        *magick_restrict q;

      /*
        Disassociate alpha.
      */
      q=GetAuthenticPixelQueue(image);
      if (image_view != (CacheView *) NULL)
        q=GetCacheViewAuthenticPixelQueue(image_view);
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        register ssize_t
          i;

        if (GetPixelReadMask(image,q) == 0)
          {
            q+=GetPixelChannels(image);
            continue;
          }
        Sa=QuantumScale*GetPixelAlpha(image,q);
        gamma=PerceptibleReciprocal(Sa);
        for (i=0; i < (ssize_t) GetPixelChannels(image); i++)
        {
          PixelChannel channel=GetPixelChannelChannel(image,i);
          PixelTrait traits=GetPixelChannelTraits(image,channel);
          if ((channel == AlphaPixelChannel) ||
              ((traits & UpdatePixelTrait) == 0))
            continue;
          q[i]=ClampToQuantum(gamma*q[i]);
        }
        q+=GetPixelChannels(image);
      }
    }
  return(extent);
}
