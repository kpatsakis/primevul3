static MagickBooleanType InverseFourierTransform(FourierInfo *fourier_info,
  fftw_complex *fourier_pixels,Image *image,ExceptionInfo *exception)
{
  CacheView
    *image_view;

  double
    *source_pixels;

  const char
    *value;

  fftw_plan
    fftw_c2r_plan;

  MemoryInfo
    *source_info;

  register IndexPacket
    *indexes;

  register PixelPacket
    *q;

  register ssize_t
    i,
    x;

  ssize_t
    y;

  source_info=AcquireVirtualMemory((size_t) fourier_info->width,
    fourier_info->height*sizeof(*source_pixels));
  if (source_info == (MemoryInfo *) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),
        ResourceLimitError,"MemoryAllocationFailed","`%s'",image->filename);
      return(MagickFalse);
    }
  source_pixels=(double *) GetVirtualMemoryBlob(source_info);
  value=GetImageArtifact(image,"fourier:normalize");
  if (LocaleCompare(value,"inverse") == 0)
    {
      double
        gamma;

      /*
        Normalize inverse transform.
      */
      i=0L;
      gamma=PerceptibleReciprocal((double) fourier_info->width*
        fourier_info->height);
      for (y=0L; y < (ssize_t) fourier_info->height; y++)
        for (x=0L; x < (ssize_t) fourier_info->center; x++)
        {
#if defined(MAGICKCORE_HAVE_COMPLEX_H)
          fourier_pixels[i]*=gamma;
#else
          fourier_pixels[i][0]*=gamma;
          fourier_pixels[i][1]*=gamma;
#endif
          i++;
        }
    }
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp critical (MagickCore_InverseFourierTransform)
#endif
  fftw_c2r_plan=fftw_plan_dft_c2r_2d(fourier_info->width,fourier_info->height,
    fourier_pixels,source_pixels,FFTW_ESTIMATE);
  fftw_execute_dft_c2r(fftw_c2r_plan,fourier_pixels,source_pixels);
  fftw_destroy_plan(fftw_c2r_plan);
  i=0L;
  image_view=AcquireAuthenticCacheView(image,exception);
  for (y=0L; y < (ssize_t) fourier_info->height; y++)
  {
    if (y >= (ssize_t) image->rows)
      break;
    q=GetCacheViewAuthenticPixels(image_view,0L,y,fourier_info->width >
      image->columns ? image->columns : fourier_info->width,1UL,exception);
    if (q == (PixelPacket *) NULL)
      break;
    indexes=GetCacheViewAuthenticIndexQueue(image_view);
    for (x=0L; x < (ssize_t) fourier_info->width; x++)
    {
      if (x < (ssize_t) image->columns)
        switch (fourier_info->channel)
        {
          case RedChannel:
          default:
          {
            SetPixelRed(q,ClampToQuantum(QuantumRange*source_pixels[i]));
            break;
          }
          case GreenChannel:
          {
            SetPixelGreen(q,ClampToQuantum(QuantumRange*source_pixels[i]));
            break;
          }
          case BlueChannel:
          {
            SetPixelBlue(q,ClampToQuantum(QuantumRange*source_pixels[i]));
            break;
          }
          case OpacityChannel:
          {
            SetPixelOpacity(q,ClampToQuantum(QuantumRange*source_pixels[i]));
            break;
          }
          case IndexChannel:
          {
            SetPixelIndex(indexes+x,ClampToQuantum(QuantumRange*
              source_pixels[i]));
            break;
          }
          case GrayChannels:
          {
            SetPixelGray(q,ClampToQuantum(QuantumRange*source_pixels[i]));
            break;
          }
        }
      i++;
      q++;
    }
    if (SyncCacheViewAuthenticPixels(image_view,exception) == MagickFalse)
      break;
  }
  image_view=DestroyCacheView(image_view);
  source_info=RelinquishVirtualMemory(source_info);
  return(MagickTrue);
}
