static void ImportBGRQuantum(const Image *image,QuantumInfo *quantum_info,
  const MagickSizeType number_pixels,const unsigned char *magick_restrict p,
  Quantum *magick_restrict q,ExceptionInfo *exception)
{
  QuantumAny
    range;

  register ssize_t
    x;

  ssize_t
    bit;

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
        SetPixelBlue(image,ScaleCharToQuantum(pixel),q);
        p=PushCharPixel(p,&pixel);
        SetPixelGreen(image,ScaleCharToQuantum(pixel),q);
        p=PushCharPixel(p,&pixel);
        SetPixelRed(image,ScaleCharToQuantum(pixel),q);
        SetPixelAlpha(image,OpaqueAlpha,q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 10:
    {
      range=GetQuantumRange(quantum_info->depth);
      if (quantum_info->pack == MagickFalse)
        {
          for (x=0; x < (ssize_t) number_pixels; x++)
          {
            p=PushLongPixel(quantum_info->endian,p,&pixel);
            SetPixelRed(image,ScaleAnyToQuantum((pixel >> 22) & 0x3ff,range),q);
            SetPixelGreen(image,ScaleAnyToQuantum((pixel >> 12) & 0x3ff,range),
              q);
            SetPixelBlue(image,ScaleAnyToQuantum((pixel >> 2) & 0x3ff,range),q);
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      if (quantum_info->quantum == 32U)
        {
          for (x=0; x < (ssize_t) number_pixels; x++)
          {
            p=PushQuantumLongPixel(quantum_info,p,&pixel);
            SetPixelBlue(image,ScaleAnyToQuantum(pixel,range),q);
            p=PushQuantumLongPixel(quantum_info,p,&pixel);
            SetPixelGreen(image,ScaleAnyToQuantum(pixel,range),q);
            p=PushQuantumLongPixel(quantum_info,p,&pixel);
            SetPixelRed(image,ScaleAnyToQuantum(pixel,range),q);
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelBlue(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelGreen(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelRed(image,ScaleAnyToQuantum(pixel,range),q);
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 12:
    {
      range=GetQuantumRange(quantum_info->depth);
      if (quantum_info->pack == MagickFalse)
        {
          unsigned short
            pixel;

          for (x=0; x < (ssize_t) (3*number_pixels-1); x+=2)
          {
            p=PushShortPixel(quantum_info->endian,p,&pixel);
            switch (x % 3)
            {
              default:
              case 0:
              {
                SetPixelRed(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                break;
              }
              case 1:
              {
                SetPixelGreen(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                break;
              }
              case 2:
              {
                SetPixelBlue(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                q+=GetPixelChannels(image);
                break;
              }
            }
            p=PushShortPixel(quantum_info->endian,p,&pixel);
            switch ((x+1) % 3)
            {
              default:
              case 0:
              {
                SetPixelRed(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                break;
              }
              case 1:
              {
                SetPixelGreen(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                break;
              }
              case 2:
              {
                SetPixelBlue(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                q+=GetPixelChannels(image);
                break;
              }
            }
            p+=quantum_info->pad;
          }
          for (bit=0; bit < (ssize_t) (3*number_pixels % 2); bit++)
          {
            p=PushShortPixel(quantum_info->endian,p,&pixel);
            switch ((x+bit) % 3)
            {
              default:
              case 0:
              {
                SetPixelRed(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                break;
              }
              case 1:
              {
                SetPixelGreen(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                break;
              }
              case 2:
              {
                SetPixelBlue(image,ScaleAnyToQuantum((QuantumAny) (pixel >> 4),
                  range),q);
                q+=GetPixelChannels(image);
                break;
              }
            }
            p+=quantum_info->pad;
          }
          if (bit != 0)
            p++;
          break;
        }
      if (quantum_info->quantum == 32U)
        {
          for (x=0; x < (ssize_t) number_pixels; x++)
          {
            p=PushQuantumLongPixel(quantum_info,p,&pixel);
            SetPixelBlue(image,ScaleAnyToQuantum(pixel,range),q);
            p=PushQuantumLongPixel(quantum_info,p,&pixel);
            SetPixelGreen(image,ScaleAnyToQuantum(pixel,range),q);
            p=PushQuantumLongPixel(quantum_info,p,&pixel);
            SetPixelRed(image,ScaleAnyToQuantum(pixel,range),q);
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelBlue(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelGreen(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelRed(image,ScaleAnyToQuantum(pixel,range),q);
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
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelBlue(image,ScaleShortToQuantum(pixel),q);
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelGreen(image,ScaleShortToQuantum(pixel),q);
        p=PushShortPixel(quantum_info->endian,p,&pixel);
        SetPixelRed(image,ScaleShortToQuantum(pixel),q);
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
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelBlue(image,ScaleLongToQuantum(pixel),q);
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelGreen(image,ScaleLongToQuantum(pixel),q);
        p=PushLongPixel(quantum_info->endian,p,&pixel);
        SetPixelRed(image,ScaleLongToQuantum(pixel),q);
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
        SetPixelBlue(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelGreen(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelRed(image,ScaleAnyToQuantum(pixel,range),q);
        q+=GetPixelChannels(image);
      }
      break;
    }
  }
}
