static void ImportBlackQuantum(const Image *image,QuantumInfo *quantum_info,
  const MagickSizeType number_pixels,const unsigned char *magick_restrict p,
  Quantum *magick_restrict q,ExceptionInfo *exception)
{
  QuantumAny
    range;

  register ssize_t
    x;

  unsigned int
    pixel;

  if (image->colorspace != CMYKColorspace)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ImageError,
        "ColorSeparatedImageRequired","`%s'",image->filename);
      return;
    }
  switch (quantum_info->depth)
  {
    case 8:
    {
      unsigned char
        pixel;

      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushCharPixel(p,&pixel);
        SetPixelBlack(image,ScaleCharToQuantum(pixel),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 16:
    {
      unsigned short
        pixel;

      if (quantum_info->format == FloatingPointQuantumFormat)
        {
          for (x=0; x < (ssize_t) number_pixels; x++)
          {
            p=PushShortPixel(quantum_info->endian,p,&pixel);
            SetPixelBlack(image,ClampToQuantum(QuantumRange*
              HalfToSinglePrecision(pixel)),q);
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelBlack(image,ScaleShortToQuantum(pixel),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 32:
    {
      unsigned int
        pixel;

      if (quantum_info->format == FloatingPointQuantumFormat)
        {
          float
            pixel;

          for (x=0; x < (ssize_t) number_pixels; x++)
          {
            p=PushFloatPixel(quantum_info,p,&pixel);
            SetPixelBlack(image,ClampToQuantum(pixel),q);
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelBlack(image,ScaleLongToQuantum(pixel),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 64:
    {
      if (quantum_info->format == FloatingPointQuantumFormat)
        {
          double
            pixel;

          for (x=0; x < (ssize_t) number_pixels; x++)
          {
            p=PushDoublePixel(quantum_info,p,&pixel);
            SetPixelBlack(image,ClampToQuantum(pixel),q);
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
    }
    default:
    {
      range=GetQuantumRange(quantum_info->depth);
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelBlack(image,ScaleAnyToQuantum(pixel,range),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
  }
}
