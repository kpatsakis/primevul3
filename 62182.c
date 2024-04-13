MagickExport MagickBooleanType SetImageChannelDepth(Image *image,
  const ChannelType channel,const size_t depth)
{
  CacheView
    *image_view;

  ExceptionInfo
    *exception;

  MagickBooleanType
    status;

  QuantumAny
    range;

  ssize_t
    y;

  assert(image != (Image *) NULL);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  assert(image->signature == MagickSignature);
  if (depth >= MAGICKCORE_QUANTUM_DEPTH)
    {
      image->depth=depth;
      return(MagickTrue);
    }
  range=GetQuantumRange(depth);
  if (image->storage_class == PseudoClass)
    {
      register ssize_t
        i;

#if defined(MAGICKCORE_OPENMP_SUPPORT)
      #pragma omp parallel for schedule(static,4) shared(status) \
        magick_threads(image,image,1,1)
#endif
      for (i=0; i < (ssize_t) image->colors; i++)
      {
        if ((channel & RedChannel) != 0)
          image->colormap[i].red=ScaleAnyToQuantum(ScaleQuantumToAny(
            ClampPixel(image->colormap[i].red),range),range);
        if ((channel & GreenChannel) != 0)
          image->colormap[i].green=ScaleAnyToQuantum(ScaleQuantumToAny(
            ClampPixel(image->colormap[i].green),range),range);
        if ((channel & BlueChannel) != 0)
          image->colormap[i].blue=ScaleAnyToQuantum(ScaleQuantumToAny(
            ClampPixel(image->colormap[i].blue),range),range);
        if ((channel & OpacityChannel) != 0)
          image->colormap[i].opacity=ScaleAnyToQuantum(ScaleQuantumToAny(
            ClampPixel(image->colormap[i].opacity),range),range);
      }
    }
  status=MagickTrue;
  exception=(&image->exception);
  image_view=AcquireAuthenticCacheView(image,exception);
#if !defined(MAGICKCORE_HDRI_SUPPORT)
DisableMSCWarning(4127)
  if (QuantumRange <= MaxMap)
RestoreMSCWarning
    {
      Quantum
        *depth_map;

      register ssize_t
        i;

      /*
        Scale pixels to desired (optimized with depth map).
      */
      depth_map=(Quantum *) AcquireQuantumMemory(MaxMap+1,sizeof(*depth_map));
      if (depth_map == (Quantum *) NULL)
        ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
      for (i=0; i <= (ssize_t) MaxMap; i++)
        depth_map[i]=ScaleAnyToQuantum(ScaleQuantumToAny((Quantum) i,range),
          range);
#if defined(MAGICKCORE_OPENMP_SUPPORT)
      #pragma omp parallel for schedule(static,4) shared(status) \
        magick_threads(image,image,image->rows,1)
#endif
      for (y=0; y < (ssize_t) image->rows; y++)
      {
        register ssize_t
          x;

        register PixelPacket
          *magick_restrict q;

        if (status == MagickFalse)
          continue;
        q=GetCacheViewAuthenticPixels(image_view,0,y,image->columns,1,
          exception);
        if (q == (PixelPacket *) NULL)
          {
            status=MagickFalse;
            continue;
          }
        for (x=0; x < (ssize_t) image->columns; x++)
        {
          if ((channel & RedChannel) != 0)
            SetPixelRed(q,depth_map[ScaleQuantumToMap(GetPixelRed(q))]);
          if ((channel & GreenChannel) != 0)
            SetPixelGreen(q,depth_map[ScaleQuantumToMap(GetPixelGreen(q))]);
          if ((channel & BlueChannel) != 0)
            SetPixelBlue(q,depth_map[ScaleQuantumToMap(GetPixelBlue(q))]);
          if (((channel & OpacityChannel) != 0) &&
              (image->matte != MagickFalse))
            SetPixelOpacity(q,depth_map[ScaleQuantumToMap(GetPixelOpacity(q))]);
          q++;
        }
        if (SyncCacheViewAuthenticPixels(image_view,exception) == MagickFalse)
          {
            status=MagickFalse;
            continue;
          }
      }
      image_view=DestroyCacheView(image_view);
      depth_map=(Quantum *) RelinquishMagickMemory(depth_map);
      if (status != MagickFalse)
        image->depth=depth;
      return(status);
    }
#endif
  /*
    Scale pixels to desired depth.
  */
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp parallel for schedule(static,4) shared(status) \
    magick_threads(image,image,image->rows,1)
#endif
  for (y=0; y < (ssize_t) image->rows; y++)
  {
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
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      if ((channel & RedChannel) != 0)
        SetPixelRed(q,ScaleAnyToQuantum(ScaleQuantumToAny(ClampPixel(
          GetPixelRed(q)),range),range));
      if ((channel & GreenChannel) != 0)
        SetPixelGreen(q,ScaleAnyToQuantum(ScaleQuantumToAny(ClampPixel(
          GetPixelGreen(q)),range),range));
      if ((channel & BlueChannel) != 0)
        SetPixelBlue(q,ScaleAnyToQuantum(ScaleQuantumToAny(ClampPixel(
          GetPixelBlue(q)),range),range));
      if (((channel & OpacityChannel) != 0) && (image->matte != MagickFalse))
        SetPixelOpacity(q,ScaleAnyToQuantum(ScaleQuantumToAny(ClampPixel(
          GetPixelOpacity(q)),range),range));
      q++;
    }
    if (SyncCacheViewAuthenticPixels(image_view,exception) == MagickFalse)
      {
        status=MagickFalse;
        continue;
      }
  }
  image_view=DestroyCacheView(image_view);
  if (status != MagickFalse)
    image->depth=depth;
  return(status);
}
