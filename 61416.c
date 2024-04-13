static void ImportGrayAlphaQuantum(const Image *image,QuantumInfo *quantum_info,
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
    case 1:
    {
      register unsigned char
        pixel;

      bit=0;
      for (x=((ssize_t) number_pixels-3); x > 0; x-=4)
      {
        for (bit=0; bit < 8; bit+=2)
        {
          pixel=(unsigned char) (((*p) & (1 << (7-bit))) != 0 ? 0x00 : 0x01);
          SetPixelGray(image,(Quantum) (pixel == 0 ? 0 : QuantumRange),q);
          SetPixelAlpha(image,((*p) & (1UL << (unsigned char) (6-bit))) == 0 ?
            TransparentAlpha : OpaqueAlpha,q);
          q+=GetPixelChannels(image);
        }
        p++;
      }
      if ((number_pixels % 4) != 0)
        for (bit=3; bit >= (ssize_t) (4-(number_pixels % 4)); bit-=2)
        {
          pixel=(unsigned char) (((*p) & (1 << (7-bit))) != 0 ? 0x00 : 0x01);
          SetPixelGray(image,(Quantum) (pixel != 0 ? 0 : QuantumRange),q);
          SetPixelAlpha(image,((*p) & (1UL << (unsigned char) (6-bit))) == 0 ?
            TransparentAlpha : OpaqueAlpha,q);
          q+=GetPixelChannels(image);
        }
      if (bit != 0)
        p++;
      break;
    }
    case 4:
    {
      register unsigned char
        pixel;

      range=GetQuantumRange(quantum_info->depth);
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        pixel=(unsigned char) ((*p >> 4) & 0xf);
        SetPixelGray(image,ScaleAnyToQuantum(pixel,range),q);
        pixel=(unsigned char) ((*p) & 0xf);
        SetPixelAlpha(image,ScaleAnyToQuantum(pixel,range),q);
        p++;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 8:
    {
      unsigned char
        pixel;

      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushCharPixel(p,&pixel);
        SetPixelGray(image,ScaleCharToQuantum(pixel),q);
        p=PushCharPixel(p,&pixel);
        SetPixelAlpha(image,ScaleCharToQuantum(pixel),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 10:
    {
      range=GetQuantumRange(quantum_info->depth);
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelGray(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelAlpha(image,ScaleAnyToQuantum(pixel,range),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 12:
    {
      range=GetQuantumRange(quantum_info->depth);
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelGray(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelAlpha(image,ScaleAnyToQuantum(pixel,range),q);
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
            SetPixelGray(image,ClampToQuantum(QuantumRange*
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
        SetPixelGray(image,ScaleShortToQuantum(pixel),q);
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
            SetPixelGray(image,ClampToQuantum(pixel),q);
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
        SetPixelGray(image,ScaleLongToQuantum(pixel),q);
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
            SetPixelGray(image,ClampToQuantum(pixel),q);
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
        SetPixelGray(image,ScaleAnyToQuantum(pixel,range),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelAlpha(image,ScaleAnyToQuantum(pixel,range),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
  }
}
