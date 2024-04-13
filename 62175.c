MagickExport size_t GetImageChannelDepth(const Image *image,
  const ChannelType channel,ExceptionInfo *exception)
{
  CacheView
    *image_view;

  MagickBooleanType
    status;

  register ssize_t
    i;

  size_t
    *current_depth,
    depth,
    number_threads;

  ssize_t
    y;

  /*
    Compute image depth.
  */
  assert(image != (Image *) NULL);

  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  number_threads=(size_t) GetMagickResourceLimit(ThreadResource);
  current_depth=(size_t *) AcquireQuantumMemory(number_threads,
    sizeof(*current_depth));
  if (current_depth == (size_t *) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  status=MagickTrue;
  for (i=0; i < (ssize_t) number_threads; i++)
    current_depth[i]=1;
  if ((image->storage_class == PseudoClass) && (image->matte == MagickFalse))
    {
#if defined(MAGICKCORE_OPENMP_SUPPORT)
      #pragma omp parallel for schedule(static,4) shared(status) \
        magick_threads(image,image,1,1)
#endif
      for (i=0; i < (ssize_t) image->colors; i++)
      {
        const int
          id = GetOpenMPThreadId();

        while (current_depth[id] < MAGICKCORE_QUANTUM_DEPTH)
        {
          MagickBooleanType
            atDepth;

          QuantumAny
            range;

          atDepth=MagickTrue;
          range=GetQuantumRange(current_depth[id]);
          if ((atDepth != MagickFalse) && ((channel & RedChannel) != 0))
            if (IsPixelAtDepth(image->colormap[i].red,range) == MagickFalse)
              atDepth=MagickFalse;
          if ((atDepth != MagickFalse) && ((channel & GreenChannel) != 0))
            if (IsPixelAtDepth(image->colormap[i].green,range) == MagickFalse)
              atDepth=MagickFalse;
          if ((atDepth != MagickFalse) && ((channel & BlueChannel) != 0))
            if (IsPixelAtDepth(image->colormap[i].blue,range) == MagickFalse)
              atDepth=MagickFalse;
          if ((atDepth != MagickFalse))
            break;
          current_depth[id]++;
        }
      }
      depth=current_depth[0];
      for (i=1; i < (ssize_t) number_threads; i++)
        if (depth < current_depth[i])
          depth=current_depth[i];
      current_depth=(size_t *) RelinquishMagickMemory(current_depth);
      return(depth);
    }
  image_view=AcquireVirtualCacheView(image,exception);
#if !defined(MAGICKCORE_HDRI_SUPPORT)
DisableMSCWarning(4127)
  if (QuantumRange <= MaxMap)
RestoreMSCWarning
    {
      size_t
        *depth_map;

      /*
        Scale pixels to desired (optimized with depth map).
      */
      depth_map=(size_t *) AcquireQuantumMemory(MaxMap+1,sizeof(*depth_map));
      if (depth_map == (size_t *) NULL)
        ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
      for (i=0; i <= (ssize_t) MaxMap; i++)
      {
        unsigned int
          depth;

        for (depth=1; depth < MAGICKCORE_QUANTUM_DEPTH; depth++)
        {
          Quantum
            pixel;

          QuantumAny
            range;

          range=GetQuantumRange(depth);
          pixel=(Quantum) i;
          if (pixel == ScaleAnyToQuantum(ScaleQuantumToAny(pixel,range),range))
            break;
        }
        depth_map[i]=depth;
      }
#if defined(MAGICKCORE_OPENMP_SUPPORT)
      #pragma omp parallel for schedule(static,4) shared(status) \
        magick_threads(image,image,image->rows,1)
#endif
      for (y=0; y < (ssize_t) image->rows; y++)
      {
        const int
          id = GetOpenMPThreadId();

        register const IndexPacket
          *magick_restrict indexes;

        register const PixelPacket
          *magick_restrict p;

        register ssize_t
          x;

        if (status == MagickFalse)
          continue;
        p=GetCacheViewVirtualPixels(image_view,0,y,image->columns,1,exception);
        if (p == (const PixelPacket *) NULL)
          continue;
        indexes=GetCacheViewVirtualIndexQueue(image_view);
        for (x=0; x < (ssize_t) image->columns; x++)
        {
          Quantum
            pixel;

          if ((channel & RedChannel) != 0)
            {
              pixel=GetPixelRed(p);
              if (depth_map[ScaleQuantumToMap(pixel)] > current_depth[id])
                current_depth[id]=depth_map[ScaleQuantumToMap(pixel)];
            }
          if ((channel & GreenChannel) != 0)
            {
              pixel=GetPixelGreen(p);
              if (depth_map[ScaleQuantumToMap(pixel)] > current_depth[id])
                current_depth[id]=depth_map[ScaleQuantumToMap(pixel)];
            }
          if ((channel & BlueChannel) != 0)
            {
              pixel=GetPixelBlue(p);
              if (depth_map[ScaleQuantumToMap(pixel)] > current_depth[id])
                current_depth[id]=depth_map[ScaleQuantumToMap(pixel)];
            }
          if (((channel & OpacityChannel) != 0) &&
               (image->matte != MagickFalse))
            {
              pixel=GetPixelOpacity(p);
              if (depth_map[ScaleQuantumToMap(pixel)] > current_depth[id])
                current_depth[id]=depth_map[ScaleQuantumToMap(pixel)];
            }
          if (((channel & IndexChannel) != 0) &&
              (image->colorspace == CMYKColorspace))
            {
              pixel=GetPixelIndex(indexes+x);
              if (depth_map[ScaleQuantumToMap(pixel)] > current_depth[id])
                current_depth[id]=depth_map[ScaleQuantumToMap(pixel)];
            }
          p++;
        }
        if (current_depth[id] == MAGICKCORE_QUANTUM_DEPTH)
          status=MagickFalse;
      }
      image_view=DestroyCacheView(image_view);
      depth=current_depth[0];
      for (i=1; i < (ssize_t) number_threads; i++)
        if (depth < current_depth[i])
          depth=current_depth[i];
      depth_map=(size_t *) RelinquishMagickMemory(depth_map);
      current_depth=(size_t *) RelinquishMagickMemory(current_depth);
      return(depth);
    }
#endif
#if defined(MAGICKCORE_OPENMP_SUPPORT)
  #pragma omp parallel for schedule(static,4) shared(status) \
    magick_threads(image,image,image->rows,1)
#endif
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    const int
      id = GetOpenMPThreadId();

    register const IndexPacket
      *magick_restrict indexes;

    register const PixelPacket
      *magick_restrict p;

    register ssize_t
      x;

    if (status == MagickFalse)
      continue;
    p=GetCacheViewVirtualPixels(image_view,0,y,image->columns,1,exception);
    if (p == (const PixelPacket *) NULL)
      continue;
    indexes=GetCacheViewVirtualIndexQueue(image_view);
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      while (current_depth[id] < MAGICKCORE_QUANTUM_DEPTH)
      {
        MagickBooleanType
          atDepth;

        QuantumAny
          range;

        atDepth=MagickTrue;
        range=GetQuantumRange(current_depth[id]);
        if ((atDepth != MagickFalse) && ((channel & RedChannel) != 0))
          if (IsPixelAtDepth(GetPixelRed(p),range) == MagickFalse)
            atDepth=MagickFalse;
        if ((atDepth != MagickFalse) && ((channel & GreenChannel) != 0))
          if (IsPixelAtDepth(GetPixelGreen(p),range) == MagickFalse)
            atDepth=MagickFalse;
        if ((atDepth != MagickFalse) && ((channel & BlueChannel) != 0))
          if (IsPixelAtDepth(GetPixelBlue(p),range) == MagickFalse)
            atDepth=MagickFalse;
        if ((atDepth != MagickFalse) && ((channel & OpacityChannel) != 0) &&
             (image->matte != MagickFalse))
          if (IsPixelAtDepth(GetPixelOpacity(p),range) == MagickFalse)
            atDepth=MagickTrue;
        if ((atDepth != MagickFalse) && ((channel & IndexChannel) != 0) &&
             (image->colorspace == CMYKColorspace))
          if (IsPixelAtDepth(GetPixelIndex(indexes+x),range) == MagickFalse)
            atDepth=MagickFalse;
        if ((atDepth != MagickFalse))
          break;
        current_depth[id]++;
      }
      p++;
    }
    if (current_depth[id] == MAGICKCORE_QUANTUM_DEPTH)
      status=MagickFalse;
  }
  image_view=DestroyCacheView(image_view);
  depth=current_depth[0];
  for (i=1; i < (ssize_t) number_threads; i++)
    if (depth < current_depth[i])
      depth=current_depth[i];
  current_depth=(size_t *) RelinquishMagickMemory(current_depth);
  return(depth);
}
