static void ImportIndexQuantum(const Image *image,QuantumInfo *quantum_info,
  const MagickSizeType number_pixels,const unsigned char *magick_restrict p,
  Quantum *magick_restrict q,ExceptionInfo *exception)
{
  MagickBooleanType
    range_exception;

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

      for (x=0; x < ((ssize_t) number_pixels-7); x+=8)
      {
        for (bit=0; bit < 8; bit++)
        {
          if (quantum_info->min_is_white == MagickFalse)
            pixel=(unsigned char) (((*p) & (1 << (7-bit))) == 0 ?
              0x00 : 0x01);
          else
            pixel=(unsigned char) (((*p) & (1 << (7-bit))) != 0 ?
              0x00 : 0x01);
          SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),
            q);
          SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
            GetPixelIndex(image,q),q);
          q+=GetPixelChannels(image);
        }
        p++;
      }
      for (bit=0; bit < (ssize_t) (number_pixels % 8); bit++)
      {
        if (quantum_info->min_is_white == MagickFalse)
          pixel=(unsigned char) (((*p) & (1 << (7-bit))) == 0 ? 0x00 : 0x01);
        else
          pixel=(unsigned char) (((*p) & (1 << (7-bit))) != 0 ? 0x00 : 0x01);
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
        q+=GetPixelChannels(image);
      }
      break;
    }
    case 4:
    {
      register unsigned char
        pixel;

      for (x=0; x < ((ssize_t) number_pixels-1); x+=2)
      {
        pixel=(unsigned char) ((*p >> 4) & 0xf);
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
        q+=GetPixelChannels(image);
        pixel=(unsigned char) ((*p) & 0xf);
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
        p++;
        q+=GetPixelChannels(image);
      }
      for (bit=0; bit < (ssize_t) (number_pixels % 2); bit++)
      {
        pixel=(unsigned char) ((*p++ >> 4) & 0xf);
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
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
            SetPixelIndex(image,PushColormapIndex(image,ClampToQuantum(pixel),
              &range_exception),q);
            SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
              GetPixelIndex(image,q),q);
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
            p+=quantum_info->pad;
            q+=GetPixelChannels(image);
          }
          break;
        }
    }
    default:
    {
      for (x=0; x < (ssize_t) number_pixels; x++)
      {
        p=PushQuantumPixel(quantum_info,p,&pixel);
        SetPixelIndex(image,PushColormapIndex(image,pixel,&range_exception),q);
        SetPixelViaPixelInfo(image,image->colormap+(ssize_t)
          GetPixelIndex(image,q),q);
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
