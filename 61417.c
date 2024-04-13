static void ImportIndexAlphaQuantum(const Image *image,
  QuantumInfo *quantum_info,const MagickSizeType number_pixels,
  const unsigned char *magick_restrict p,Quantum *magick_restrict q,
  ExceptionInfo *exception)
{
  MagickBooleanType
    range_exception;

  QuantumAny
    range;

  register ssize_t
    x;

  ssize_t
    bit;

  unsigned int
    pixel;

  if (image->storage_class != PseudoClass)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ImageError,
        "ColormappedImageRequired","`%s'",image->filename);
      return;
    }
  range_exception=MagickFalse;
  switch (quantum_info->depth)
  {
    case 1:
    {
      register unsigned char
        pixel;

      for (x=((ssize_t) number_pixels-3); x > 0; x-=4)
      {
        for (bit=0; bit < 8; bit+=2)
        {
          if (quantum_info->min_is_white == MagickFalse)
            pixel=(unsigned char) (((*p) & (1 << (7-bit))) == 0 ? 0x00 : 0x01);
          else
            pixel=(unsigned char) (((*p) & (1 << (7-bit))) != 0 ? 0x00 : 0x01);
          SetPixelGray(image,(Quantum) (pixel == 0 ? 0 : QuantumRange),q);
          SetPixelAlpha(image,((*p) & (1UL << (unsigned char) (6-bit))) == 0 ?
            TransparentAlpha : OpaqueAlpha,q);
          SetPixelIndex(image,(Quantum) (pixel == 0 ? 0 : 1),q);
          q+=GetPixelChannels(image);
        }
      }
      if ((number_pixels % 4) != 0)
        for (bit=3; bit >= (ssize_t) (4-(number_pixels % 4)); bit-=2)
        {
          if (quantum_info->min_is_white == MagickFalse)
            pixel=(unsigned char) (((*p) & (1 << (7-bit))) == 0 ? 0x00 : 0x01);
          else
            pixel=(unsigned char) (((*p) & (1 << (7-bit))) != 0 ? 0x00 : 0x01);
          SetPixelIndex(image,(Quantum) (pixel == 0 ? 0 : 1),q);
          SetPixelGray(image,(Quantum) (pixel == 0 ? 0 : QuantumRange),q);
          SetPixelAlpha(image,((*p) & (1UL << (unsigned char) (6-bit))) == 0 ?
            TransparentAlpha : OpaqueAlpha,q);
          q+=GetPixelChannels(image);
        }
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
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
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
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
        p=PushCharPixel(p,&pixel);
        SetPixelAlpha(image,ScaleCharToQuantum(pixel),q);
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
            SetPixelIndex(image,PushColormapIndex(image,ClampToQuantum(
              (double) QuantumRange*HalfToSinglePrecision(pixel)),
              &range_exception),q);
            SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
              GetPixelIndex(image,q),q);
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
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
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
            SetPixelIndex(image,PushColormapIndex(image,
              ClampToQuantum(pixel),&range_exception),q);
            SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
              GetPixelIndex(image,q),q);
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
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
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
            SetPixelIndex(image,PushColormapIndex(image,ClampToQuantum(pixel),
              &range_exception),q);
            SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
              GetPixelIndex(image,q),q);
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
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelAlpha(image,ScaleAnyToQuantum(pixel,range),q);
        p+=quantum_info->pad;
        q+=GetPixelChannels(image);
      }
      break;
    }
  }
  if (range_exception != MagickFalse)
    (void) ThrowMagickException(exception,GetMagickModule(),CorruptImageError,
      "InvalidColormapIndex","`%s'",image->filename);
}
