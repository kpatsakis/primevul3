static MagickBooleanType WriteVICARImage(const ImageInfo *image_info,
  Image *image)
{
  char
    header[MaxTextExtent];

  int
    y;

  MagickBooleanType
    status;

  QuantumInfo
    *quantum_info;

  register const PixelPacket
    *p;

  size_t
    length;

  ssize_t
    count;

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
    Write header.
  */
  (void) ResetMagickMemory(header,' ',MaxTextExtent);
  (void) FormatLocaleString(header,MaxTextExtent,
    "LBLSIZE=%.20g FORMAT='BYTE' TYPE='IMAGE' BUFSIZE=20000 DIM=2 EOL=0 "
    "RECSIZE=%.20g ORG='BSQ' NL=%.20g NS=%.20g NB=1 N1=0 N2=0 N3=0 N4=0 NBB=0 "
    "NLB=0 TASK='ImageMagick'",(double) MaxTextExtent,(double) image->columns,
    (double) image->rows,(double) image->columns);
  (void) WriteBlob(image,MaxTextExtent,(unsigned char *) header);
  /*
    Write VICAR pixels.
  */
  image->depth=8;
  quantum_info=AcquireQuantumInfo(image_info,image);
  if (quantum_info == (QuantumInfo *) NULL)
    ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
  pixels=GetQuantumPixels(quantum_info);
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    length=ExportQuantumPixels(image,(const CacheView *) NULL,quantum_info,
      GrayQuantum,pixels,&image->exception);
    count=WriteBlob(image,length,pixels);
    if (count != (ssize_t) length)
      break;
    status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }
  quantum_info=DestroyQuantumInfo(quantum_info);
  (void) CloseBlob(image);
  return(MagickTrue);
}
