static MagickBooleanType WriteFITSImage(const ImageInfo *image_info,
  Image *image)
{
  char
    header[FITSBlocksize],
    *fits_info;

  MagickBooleanType
    status;

  QuantumInfo
    *quantum_info;

  register const PixelPacket
    *p;

  size_t
    length;

  ssize_t
    count,
    offset,
    y;

  unsigned char
    *pixels;

  /*
    Open output image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  (void) TransformImageColorspace(image,sRGBColorspace);
  /*
    Allocate image memory.
  */
  fits_info=(char *) AcquireQuantumMemory(FITSBlocksize,sizeof(*fits_info));
  if (fits_info == (char *) NULL)
    ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
  (void) ResetMagickMemory(fits_info,' ',FITSBlocksize*sizeof(*fits_info));
  /*
    Initialize image header.
  */
  image->depth=GetImageQuantumDepth(image,MagickFalse);
  image->endian=MSBEndian;
  quantum_info=AcquireQuantumInfo(image_info,image);
  if (quantum_info == (QuantumInfo *) NULL)
    ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
  offset=0;
  (void) FormatLocaleString(header,FITSBlocksize,
    "SIMPLE  =                    T");
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) FormatLocaleString(header,FITSBlocksize,"BITPIX  =           %10ld",
    (long) ((quantum_info->format == FloatingPointQuantumFormat ? -1 : 1)*
    image->depth));
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) FormatLocaleString(header,FITSBlocksize,"NAXIS   =           %10lu",
    IsGrayImage(image,&image->exception) != MagickFalse ? 2UL : 3UL);
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) FormatLocaleString(header,FITSBlocksize,"NAXIS1  =           %10lu",
    (unsigned long) image->columns);
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) FormatLocaleString(header,FITSBlocksize,"NAXIS2  =           %10lu",
    (unsigned long) image->rows);
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  if (IsGrayImage(image,&image->exception) == MagickFalse)
    {
      (void) FormatLocaleString(header,FITSBlocksize,
        "NAXIS3  =           %10lu",3UL);
      (void) strncpy(fits_info+offset,header,strlen(header));
      offset+=80;
    }
  (void) FormatLocaleString(header,FITSBlocksize,"BSCALE  =         %E",1.0);
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) FormatLocaleString(header,FITSBlocksize,"BZERO   =         %E",
    image->depth > 8 ? GetFITSPixelRange(image->depth)/2.0 : 0.0);
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) FormatLocaleString(header,FITSBlocksize,"DATAMAX =         %E",
    1.0*((MagickOffsetType) GetQuantumRange(image->depth)));
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) FormatLocaleString(header,FITSBlocksize,"DATAMIN =         %E",0.0);
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  if (image->endian == LSBEndian)
    {
      (void) FormatLocaleString(header,FITSBlocksize,"XENDIAN = 'SMALL'");
      (void) strncpy(fits_info+offset,header,strlen(header));
      offset+=80;
    }
  (void) FormatLocaleString(header,FITSBlocksize,"HISTORY %.72s",
    GetMagickVersion((size_t *) NULL));
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) strncpy(header,"END",FITSBlocksize);
  (void) strncpy(fits_info+offset,header,strlen(header));
  offset+=80;
  (void) WriteBlob(image,FITSBlocksize,(unsigned char *) fits_info);
  /*
    Convert image to fits scale PseudoColor class.
  */
  pixels=GetQuantumPixels(quantum_info);
  if (IsGrayImage(image,&image->exception) != MagickFalse)
    {
      length=GetQuantumExtent(image,quantum_info,GrayQuantum);
      for (y=(ssize_t) image->rows-1; y >= 0; y--)
      {
        p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
        if (p == (const PixelPacket *) NULL)
          break;
        length=ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
          GrayQuantum,pixels,&image->exception);
        if (image->depth == 16)
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        if (((image->depth == 32) || (image->depth == 64)) &&
            (quantum_info->format != FloatingPointQuantumFormat))
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        count=WriteBlob(image,length,pixels);
        if (count != (ssize_t) length)
          break;
        status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
          image->rows);
        if (status == MagickFalse)
          break;
      }
    }
  else
    {
      length=GetQuantumExtent(image,quantum_info,RedQuantum);
      for (y=(ssize_t) image->rows-1; y >= 0; y--)
      {
        p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
        if (p == (const PixelPacket *) NULL)
          break;
        length=ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
          RedQuantum,pixels,&image->exception);
        if (image->depth == 16)
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        if (((image->depth == 32) || (image->depth == 64)) &&
            (quantum_info->format != FloatingPointQuantumFormat))
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        count=WriteBlob(image,length,pixels);
        if (count != (ssize_t) length)
          break;
        status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
          image->rows);
        if (status == MagickFalse)
          break;
      }
      length=GetQuantumExtent(image,quantum_info,GreenQuantum);
      for (y=(ssize_t) image->rows-1; y >= 0; y--)
      {
        p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
        if (p == (const PixelPacket *) NULL)
          break;
        length=ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
          GreenQuantum,pixels,&image->exception);
        if (image->depth == 16)
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        if (((image->depth == 32) || (image->depth == 64)) &&
            (quantum_info->format != FloatingPointQuantumFormat))
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        count=WriteBlob(image,length,pixels);
        if (count != (ssize_t) length)
          break;
        status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
          image->rows);
        if (status == MagickFalse)
          break;
      }
      length=GetQuantumExtent(image,quantum_info,BlueQuantum);
      for (y=(ssize_t) image->rows-1; y >= 0; y--)
      {
        p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
        if (p == (const PixelPacket *) NULL)
          break;
        length=ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
          BlueQuantum,pixels,&image->exception);
        if (image->depth == 16)
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        if (((image->depth == 32) || (image->depth == 64)) &&
            (quantum_info->format != FloatingPointQuantumFormat))
          SetFITSUnsignedPixels(image->columns,image->depth,image->endian,
            pixels);
        count=WriteBlob(image,length,pixels);
        if (count != (ssize_t) length)
          break;
        status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
          image->rows);
        if (status == MagickFalse)
          break;
      }
    }
  quantum_info=DestroyQuantumInfo(quantum_info);
  length=(size_t) (FITSBlocksize-TellBlob(image) % FITSBlocksize);
  if (length != 0)
    {
      (void) ResetMagickMemory(fits_info,0,length*sizeof(*fits_info));
      (void) WriteBlob(image,length,(unsigned char *) fits_info);
    }
  fits_info=DestroyString(fits_info);
  (void) CloseBlob(image);
  return(MagickTrue);
}
