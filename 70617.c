static MagickBooleanType InverseFourier(FourierInfo *fourier_info,
  const Image *magnitude_image,const Image *phase_image,
  fftw_complex *fourier_pixels,ExceptionInfo *exception)
{
  CacheView
    *magnitude_view,
    *phase_view;

  double
    *inverse_pixels,
    *magnitude_pixels,
    *phase_pixels;

  MagickBooleanType
    status;

  MemoryInfo
    *inverse_info,
    *magnitude_info,
    *phase_info;

  register const IndexPacket
    *indexes;

  register const PixelPacket
    *p;

  register ssize_t
    i,
    x;

  ssize_t
    y;

  /*
    Inverse Fourier - read image and break down into a double array.
  */
  magnitude_info=AcquireVirtualMemory((size_t) fourier_info->width,
    fourier_info->height*sizeof(*magnitude_pixels));
  phase_info=AcquireVirtualMemory((size_t) fourier_info->width,
    fourier_info->height*sizeof(*phase_pixels));
  inverse_info=AcquireVirtualMemory((size_t) fourier_info->width,
    (fourier_info->height/2+1)*sizeof(*inverse_pixels));
  if ((magnitude_info == (MemoryInfo *) NULL) ||
      (phase_info == (MemoryInfo *) NULL) ||
      (inverse_info == (MemoryInfo *) NULL))
    {
      if (magnitude_info != (MemoryInfo *) NULL)
        magnitude_info=RelinquishVirtualMemory(magnitude_info);
      if (phase_info != (MemoryInfo *) NULL)
        phase_info=RelinquishVirtualMemory(phase_info);
      if (inverse_info != (MemoryInfo *) NULL)
        inverse_info=RelinquishVirtualMemory(inverse_info);
      (void) ThrowMagickException(exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",
        magnitude_image->filename);
      return(MagickFalse);
    }
  magnitude_pixels=(double *) GetVirtualMemoryBlob(magnitude_info);
  phase_pixels=(double *) GetVirtualMemoryBlob(phase_info);
  inverse_pixels=(double *) GetVirtualMemoryBlob(inverse_info);
  i=0L;
  magnitude_view=AcquireVirtualCacheView(magnitude_image,exception);
  for (y=0L; y < (ssize_t) fourier_info->height; y++)
  {
    p=GetCacheViewVirtualPixels(magnitude_view,0L,y,fourier_info->width,1UL,
      exception);
    if (p == (const PixelPacket *) NULL)
      break;
    indexes=GetCacheViewAuthenticIndexQueue(magnitude_view);
    for (x=0L; x < (ssize_t) fourier_info->width; x++)
    {
      switch (fourier_info->channel)
      {
        case RedChannel:
        default:
        {
          magnitude_pixels[i]=QuantumScale*GetPixelRed(p);
          break;
        }
        case GreenChannel:
        {
          magnitude_pixels[i]=QuantumScale*GetPixelGreen(p);
          break;
        }
        case BlueChannel:
        {
          magnitude_pixels[i]=QuantumScale*GetPixelBlue(p);
          break;
        }
        case OpacityChannel:
        {
          magnitude_pixels[i]=QuantumScale*GetPixelOpacity(p);
          break;
        }
        case IndexChannel:
        {
          magnitude_pixels[i]=QuantumScale*GetPixelIndex(indexes+x);
          break;
        }
        case GrayChannels:
        {
          magnitude_pixels[i]=QuantumScale*GetPixelGray(p);
          break;
        }
      }
      i++;
      p++;
    }
  }
  magnitude_view=DestroyCacheView(magnitude_view);
  status=InverseQuadrantSwap(fourier_info->width,fourier_info->height,
    magnitude_pixels,inverse_pixels);
  (void) memcpy(magnitude_pixels,inverse_pixels,fourier_info->height*
    fourier_info->center*sizeof(*magnitude_pixels));
  i=0L;
  phase_view=AcquireVirtualCacheView(phase_image,exception);
  for (y=0L; y < (ssize_t) fourier_info->height; y++)
  {
    p=GetCacheViewVirtualPixels(phase_view,0,y,fourier_info->width,1,
      exception);
    if (p == (const PixelPacket *) NULL)
      break;
    indexes=GetCacheViewAuthenticIndexQueue(phase_view);
    for (x=0L; x < (ssize_t) fourier_info->width; x++)
    {
      switch (fourier_info->channel)
      {
        case RedChannel:
        default:
        {
          phase_pixels[i]=QuantumScale*GetPixelRed(p);
          break;
        }
        case GreenChannel:
        {
          phase_pixels[i]=QuantumScale*GetPixelGreen(p);
          break;
        }
        case BlueChannel:
        {
          phase_pixels[i]=QuantumScale*GetPixelBlue(p);
          break;
        }
        case OpacityChannel:
        {
          phase_pixels[i]=QuantumScale*GetPixelOpacity(p);
          break;
        }
        case IndexChannel:
        {
          phase_pixels[i]=QuantumScale*GetPixelIndex(indexes+x);
          break;
        }
        case GrayChannels:
        {
          phase_pixels[i]=QuantumScale*GetPixelGray(p);
          break;
        }
      }
      i++;
      p++;
    }
  }
  if (fourier_info->modulus != MagickFalse)
    {
      i=0L;
      for (y=0L; y < (ssize_t) fourier_info->height; y++)
        for (x=0L; x < (ssize_t) fourier_info->width; x++)
        {
          phase_pixels[i]-=0.5;
          phase_pixels[i]*=(2.0*MagickPI);
          i++;
        }
    }
  phase_view=DestroyCacheView(phase_view);
  CorrectPhaseLHS(fourier_info->width,fourier_info->height,phase_pixels);
  if (status != MagickFalse)
    status=InverseQuadrantSwap(fourier_info->width,fourier_info->height,
      phase_pixels,inverse_pixels);
  (void) memcpy(phase_pixels,inverse_pixels,fourier_info->height*
    fourier_info->center*sizeof(*phase_pixels));
  inverse_info=RelinquishVirtualMemory(inverse_info);
  /*
    Merge two sets.
  */
  i=0L;
  if (fourier_info->modulus != MagickFalse)
    for (y=0L; y < (ssize_t) fourier_info->height; y++)
       for (x=0L; x < (ssize_t) fourier_info->center; x++)
       {
#if defined(MAGICKCORE_HAVE_COMPLEX_H)
         fourier_pixels[i]=magnitude_pixels[i]*cos(phase_pixels[i])+I*
           magnitude_pixels[i]*sin(phase_pixels[i]);
#else
         fourier_pixels[i][0]=magnitude_pixels[i]*cos(phase_pixels[i]);
         fourier_pixels[i][1]=magnitude_pixels[i]*sin(phase_pixels[i]);
#endif
         i++;
      }
  else
    for (y=0L; y < (ssize_t) fourier_info->height; y++)
      for (x=0L; x < (ssize_t) fourier_info->center; x++)
      {
#if defined(MAGICKCORE_HAVE_COMPLEX_H)
        fourier_pixels[i]=magnitude_pixels[i]+I*phase_pixels[i];
#else
        fourier_pixels[i][0]=magnitude_pixels[i];
        fourier_pixels[i][1]=phase_pixels[i];
#endif
        i++;
      }
  magnitude_info=RelinquishVirtualMemory(magnitude_info);
  phase_info=RelinquishVirtualMemory(phase_info);
  return(status);
}
