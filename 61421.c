static void ImportRGBAQuantum(const Image *image,QuantumInfo *quantum_info,
  const MagickSizeType number_pixels,const unsigned char *magick_restrict p,
  Quantum *magick_restrict q,ExceptionInfo *exception)
{
  QuantumAny
    range;

  register ssize_t
    x;

  unsigned int
    pixel;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  switch (quantum_info->depth)
  {
    case 8:
    {
      unsigned char
        pixel;

      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushCharPixel(p,&pixel);
        SetPixelRed(image,ScaleCharToQuantum(pixel),q);
        p=PushCharPixel(p,&pixel);
        SetPixelGreen(image,ScaleCharToQuantum(pixel),q);
        p=PushCharPixel(p,&pixel);
        SetPixelBlue(image,ScaleCharToQuantum(pixel),q);
        p=PushCharPixel(p,&pixel);
        SetPixelAlpha(image,ScaleCharToQuantum(pixel),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 10:
    {
      pixel=0;
      if (quantum_info->pack == MagickFalse)
        {
          register ssize_t
            i;

          size_t
            quantum;

          ssize_t
            n;

          n=0;
          quantum=0;
          for (x=0; x < (ssize_t) number_pixels; x++)
          {
            for (i=0; i < 4; i++)
            {
              switch (n % 3)
              {
                case 0:
                {
                  p=PushLongPixel(quantum_info->endian,p,&pixel);
                  quantum=(size_t) (ScaleShortToQuantum((unsigned short)
                    (((pixel >> 22) & 0x3ff) << 6)));
                  break;
                }
                case 1:
                {
                  quantum=(size_t) (ScaleShortToQuantum((unsigned short)
                    (((pixel >> 12) & 0x3ff) << 6)));
                  break;
                }
                case 2:
                {
                  quantum=(size_t) (ScaleShortToQuantum((unsigned short)
                    (((pixel >> 2) & 0x3ff) << 6)));
                  break;
                }
              }
              switch (i)
              {
                case 0: SetPixelRed(image,(Quantum) quantum,q); break;
                case 1: SetPixelGreen(image,(Quantum) quantum,q); break;
                case 2: SetPixelBlue(image,(Quantum) quantum,q); break;
                case 3: SetPixelAlpha(image,(Quantum) quantum,q); break;
              }
              n++;
            }
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelRed(image,ScaleShortToQuantum((unsigned short) (pixel << 6)),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelGreen(image,ScaleShortToQuantum((unsigned short) (pixel << 6)),
          q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelBlue(image,ScaleShortToQuantum((unsigned short) (pixel << 6)),
          q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelAlpha(image,ScaleShortToQuantum((unsigned short) (pixel << 6)),
          q);
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
            SetPixelRed(image,ClampToQuantum(QuantumRange*
              HalfToSinglePrecision(pixel)),q);
            p=PushShortPixel(quantum_info->endian,p,&pixel);
            SetPixelGreen(image,ClampToQuantum(QuantumRange*
              HalfToSinglePrecision(pixel)),q);
            p=PushShortPixel(quantum_info->endian,p,&pixel);
            SetPixelBlue(image,ClampToQuantum(QuantumRange*
              HalfToSinglePrecision(pixel)),q);
            p=PushShortPixel(quantum_info->endian,p,&pixel);
            SetPixelAlpha(image,ClampToQuantum(QuantumRange*
              HalfToSinglePrecision(pixel)),q);
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelRed(image,ScaleShortToQuantum(pixel),q);
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelGreen(image,ScaleShortToQuantum(pixel),q);
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelBlue(image,ScaleShortToQuantum(pixel),q);
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelAlpha(image,ScaleShortToQuantum(pixel),q);
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
            SetPixelRed(image,ClampToQuantum(pixel),q);
            p=PushFloatPixel(quantum_info,p,&pixel);
            SetPixelGreen(image,ClampToQuantum(pixel),q);
            p=PushFloatPixel(quantum_info,p,&pixel);
            SetPixelBlue(image,ClampToQuantum(pixel),q);
            p=PushFloatPixel(quantum_info,p,&pixel);
            SetPixelAlpha(image,ClampToQuantum(pixel),q);
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelRed(image,ScaleLongToQuantum(pixel),q);
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelGreen(image,ScaleLongToQuantum(pixel),q);
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelBlue(image,ScaleLongToQuantum(pixel),q);
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelAlpha(image,ScaleLongToQuantum(pixel),q);
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
            SetPixelRed(image,ClampToQuantum(pixel),q);
            p=PushDoublePixel(quantum_info,p,&pixel);
            SetPixelGreen(image,ClampToQuantum(pixel),q);
            p=PushDoublePixel(quantum_info,p,&pixel);
            SetPixelBlue(image,ClampToQuantum(pixel),q);
            p=PushDoublePixel(quantum_info,p,&pixel);
            SetPixelAlpha(image,ClampToQuantum(pixel),q);
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
        SetPixelRed(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelGreen(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelBlue(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelAlpha(image,ScaleAnyToQuantum(pixel,range),q);
        q+=GetPixelChannels(image);
      }
      break;
    }
  }
}
