MagickExport MagickBooleanType PerceptibleImageChannel(Image *image,
  const ChannelType channel,const double epsilon)
{
#define PerceptibleImageTag  "Perceptible/Image"

  CacheView
    *image_view;

  ExceptionInfo
    *exception;

  MagickBooleanType
    status;

  MagickOffsetType
    progress;

  ssize_t
    y;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  if (image->storage_class == PseudoClass)
    {
      register ssize_t
        i;

      register PixelPacket
        *magick_restrict q;

      q=image->colormap;
      for (i=0; i < (ssize_t) image->colors; i++)
      {
        SetPixelRed(q,PerceptibleThreshold(GetPixelRed(q),epsilon));
        SetPixelGreen(q,PerceptibleThreshold(GetPixelGreen(q),epsilon));
        SetPixelBlue(q,PerceptibleThreshold(GetPixelBlue(q),epsilon));
        SetPixelOpacity(q,PerceptibleThreshold(GetPixelOpacity(q),epsilon));
        q++;
      }
      return(SyncImage(image));
    }
  /*
    Perceptible image.
  */
  status=MagickTrue;
  progress=0;
  exception=(&image->exception);
  image_view=AcquireAuthenticCacheView(image,exception);
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp parallel for schedule(static) shared(progress,status) \
    magick_number_threads(image,image,image->rows,1)
#endif
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    register IndexPacket
      *magick_restrict indexes;

    register ssize_t
      x;

    register PixelPacket
      *magick_restrict q;

    if (status == MagickFalse)
      continue;
    q=GetCacheViewAuthenticPixels(image_view,0,y,image->columns,1,exception);
    if (q == (PixelPacket *) NULL)
      {
        status=MagickFalse;
        continue;
      }
    indexes=GetCacheViewAuthenticIndexQueue(image_view);
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      if ((channel & RedChannel) != 0)
        SetPixelRed(q,PerceptibleThreshold(GetPixelRed(q),epsilon));
      if ((channel & GreenChannel) != 0)
        SetPixelGreen(q,PerceptibleThreshold(GetPixelGreen(q),epsilon));
      if ((channel & BlueChannel) != 0)
        SetPixelBlue(q,PerceptibleThreshold(GetPixelBlue(q),epsilon));
      if ((channel & OpacityChannel) != 0)
        SetPixelOpacity(q,PerceptibleThreshold(GetPixelOpacity(q),epsilon));
      if (((channel & IndexChannel) != 0) &&
          (image->colorspace == CMYKColorspace))
        SetPixelIndex(indexes+x,PerceptibleThreshold(GetPixelIndex(indexes+x),
          epsilon));
      q++;
    }
    if (SyncCacheViewAuthenticPixels(image_view,exception) == MagickFalse)
      status=MagickFalse;
    if (image->progress_monitor != (MagickProgressMonitor) NULL)
      {
        MagickBooleanType
          proceed;

#if defined(MAGICKCORE_OPENMP_SUPPORT)
        #pragma omp atomic
#endif
        progress++;
        proceed=SetImageProgress(image,PerceptibleImageTag,progress,
          image->rows);
        if (proceed == MagickFalse)
          status=MagickFalse;
      }
  }
  image_view=DestroyCacheView(image_view);
  return(status);
}
