static MagickBooleanType WriteCMYKImage(const ImageInfo *image_info,
  Image *image)
{
  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  QuantumInfo
    *quantum_info;

  QuantumType
    quantum_type;

  ssize_t
    count,
    y;

  size_t
    length;

  unsigned char
    *pixels;

  /*
    Allocate memory for pixels.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  if (image_info->interlace != PartitionInterlace)
    {
      /*
        Open output image file.
      */
      status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
      if (status == MagickFalse)
        return(status);
    }
  quantum_type=CMYKQuantum;
  if (LocaleCompare(image_info->magick,"CMYKA") == 0)
    {
      quantum_type=CMYKAQuantum;
      image->matte=MagickTrue;
    }
  scene=0;
  do
  {
    /*
      Convert MIFF to CMYK raster pixels.
    */
    if (image->colorspace != CMYKColorspace)
      (void) TransformImageColorspace(image,CMYKColorspace);
    if ((LocaleCompare(image_info->magick,"CMYKA") == 0) &&
        (image->matte == MagickFalse))
      (void) SetImageAlphaChannel(image,ResetAlphaChannel);
    quantum_info=AcquireQuantumInfo(image_info,image);
    if (quantum_info == (QuantumInfo *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    pixels=GetQuantumPixels(quantum_info);
    switch (image_info->interlace)
    {
      case NoInterlace:
      default:
      {
        /*
          No interlacing:  CMYKCMYKCMYKCMYKCMYKCMYK...
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,quantum_type,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
        break;
      }
      case LineInterlace:
      {
        /*
          Line interlacing:  CCC...MMM...YYY...KKK...CCC...MMM...YYY...KKK...
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,CyanQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,MagentaQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,YellowQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,BlackQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
          if (quantum_type == CMYKAQuantum)
            {
              length=ExportQuantumPixels(image,(const CacheView *) NULL,
                quantum_info,AlphaQuantum,pixels,&image->exception);
              count=WriteBlob(image,length,pixels);
              if (count != (ssize_t) length)
                break;
            }
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
        break;
      }
      case PlaneInterlace:
      {
        /*
          Plane interlacing:  CCCCCC...MMMMMM...YYYYYY...KKKKKK...
        */
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,CyanQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,1,6);
            if (status == MagickFalse)
              break;
          }
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,MagentaQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,2,6);
            if (status == MagickFalse)
              break;
          }
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,YellowQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,3,6);
            if (status == MagickFalse)
              break;
          }
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,BlackQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,4,6);
            if (status == MagickFalse)
              break;
          }
        if (quantum_type == CMYKAQuantum)
          {
            for (y=0; y < (ssize_t) image->rows; y++)
            {
              register const PixelPacket
                *restrict p;

              p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
              if (p == (const PixelPacket *) NULL)
                break;
              length=ExportQuantumPixels(image,(const CacheView *) NULL,
                quantum_info,AlphaQuantum,pixels,&image->exception);
              count=WriteBlob(image,length,pixels);
              if (count != (ssize_t) length)
              break;
            }
            if (image->previous == (Image *) NULL)
              {
                status=SetImageProgress(image,SaveImageTag,5,6);
                if (status == MagickFalse)
                  break;
              }
          }
        if (image_info->interlace == PartitionInterlace)
          (void) CopyMagickString(image->filename,image_info->filename,
            MaxTextExtent);
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,6,6);
            if (status == MagickFalse)
              break;
          }
        break;
      }
      case PartitionInterlace:
      {
        /*
          Partition interlacing:  CCCCCC..., MMMMMM..., YYYYYY..., KKKKKK...
        */
        AppendImageFormat("C",image->filename);
        status=OpenBlob(image_info,image,scene == 0 ? WriteBinaryBlobMode :
          AppendBinaryBlobMode,&image->exception);
        if (status == MagickFalse)
          return(status);
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,CyanQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,1,6);
            if (status == MagickFalse)
              break;
          }
        (void) CloseBlob(image);
        AppendImageFormat("M",image->filename);
        status=OpenBlob(image_info,image,scene == 0 ? WriteBinaryBlobMode :
          AppendBinaryBlobMode,&image->exception);
        if (status == MagickFalse)
          return(status);
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,MagentaQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,2,6);
            if (status == MagickFalse)
              break;
          }
        (void) CloseBlob(image);
        AppendImageFormat("Y",image->filename);
        status=OpenBlob(image_info,image,scene == 0 ? WriteBinaryBlobMode :
          AppendBinaryBlobMode,&image->exception);
        if (status == MagickFalse)
          return(status);
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,YellowQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,3,6);
            if (status == MagickFalse)
              break;
          }
        (void) CloseBlob(image);
        AppendImageFormat("K",image->filename);
        status=OpenBlob(image_info,image,scene == 0 ? WriteBinaryBlobMode :
          AppendBinaryBlobMode,&image->exception);
        if (status == MagickFalse)
          return(status);
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          register const PixelPacket
            *restrict p;

          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          length=ExportQuantumPixels(image,(const CacheView *) NULL,
            quantum_info,BlackQuantum,pixels,&image->exception);
          count=WriteBlob(image,length,pixels);
          if (count != (ssize_t) length)
            break;
        }
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,4,6);
            if (status == MagickFalse)
              break;
          }
        if (quantum_type == CMYKAQuantum)
          {
            (void) CloseBlob(image);
            AppendImageFormat("A",image->filename);
            status=OpenBlob(image_info,image,scene == 0 ? WriteBinaryBlobMode :
              AppendBinaryBlobMode,&image->exception);
            if (status == MagickFalse)
              return(status);
            for (y=0; y < (ssize_t) image->rows; y++)
            {
              register const PixelPacket
                *restrict p;

              p=GetVirtualPixels(image,0,y,image->columns,1,
                &image->exception);
              if (p == (const PixelPacket *) NULL)
                break;
              length=ExportQuantumPixels(image,(const CacheView *) NULL,
                quantum_info,AlphaQuantum,pixels,&image->exception);
              count=WriteBlob(image,length,pixels);
              if (count != (ssize_t) length)
                break;
            }
            if (image->previous == (Image *) NULL)
              {
                status=SetImageProgress(image,SaveImageTag,5,6);
                if (status == MagickFalse)
                  break;
              }
          }
        (void) CloseBlob(image);
        (void) CopyMagickString(image->filename,image_info->filename,
          MaxTextExtent);
        if (image->previous == (Image *) NULL)
          {
            status=SetImageProgress(image,SaveImageTag,6,6);
            if (status == MagickFalse)
              break;
          }
        break;
      }
    }
    quantum_info=DestroyQuantumInfo(quantum_info);
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,
      GetImageListLength(image));
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  (void) CloseBlob(image);
  return(MagickTrue);
}
