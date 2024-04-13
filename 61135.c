static MagickBooleanType WriteEXRImage(const ImageInfo *image_info,Image *image)
{
  ImageInfo
    *write_info;

  ImfHalf
    half_quantum;

  ImfHeader
    *hdr_info;

  ImfOutputFile
    *file;

  ImfRgba
    *scanline;

  int
    compression;

  MagickBooleanType
    status;

  register const PixelPacket
    *p;

  register ssize_t
    x;

  ssize_t
    y;

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
  (void) SetImageColorspace(image,RGBColorspace);
  write_info=CloneImageInfo(image_info);
  (void) AcquireUniqueFilename(write_info->filename);
  hdr_info=ImfNewHeader();
  ImfHeaderSetDataWindow(hdr_info,0,0,(int) image->columns-1,(int)
    image->rows-1);
  ImfHeaderSetDisplayWindow(hdr_info,0,0,(int) image->columns-1,(int)
    image->rows-1);
  compression=IMF_NO_COMPRESSION;
  if (write_info->compression == ZipSCompression)
    compression=IMF_ZIPS_COMPRESSION;
  if (write_info->compression == ZipCompression)
    compression=IMF_ZIP_COMPRESSION;
  if (write_info->compression == PizCompression)
    compression=IMF_PIZ_COMPRESSION;
  if (write_info->compression == Pxr24Compression)
    compression=IMF_PXR24_COMPRESSION;
#if defined(B44Compression)
  if (write_info->compression == B44Compression)
    compression=IMF_B44_COMPRESSION;
#endif
#if defined(B44ACompression)
  if (write_info->compression == B44ACompression)
    compression=IMF_B44A_COMPRESSION;
#endif
  ImfHeaderSetCompression(hdr_info,compression);
  ImfHeaderSetLineOrder(hdr_info,IMF_INCREASING_Y);
  file=ImfOpenOutputFile(write_info->filename,hdr_info,IMF_WRITE_RGBA);
  ImfDeleteHeader(hdr_info);
  if (file == (ImfOutputFile *) NULL)
    {
      (void) RelinquishUniqueFileResource(write_info->filename);
      write_info=DestroyImageInfo(write_info);
      ThrowFileException(&image->exception,BlobError,"UnableToOpenBlob",
        ImfErrorMessage());
      return(MagickFalse);
    }
  scanline=(ImfRgba *) AcquireQuantumMemory(image->columns,sizeof(*scanline));
  if (scanline == (ImfRgba *) NULL)
    {
      (void) ImfCloseOutputFile(file);
      (void) RelinquishUniqueFileResource(write_info->filename);
      write_info=DestroyImageInfo(write_info);
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    }
  ResetMagickMemory(scanline,0,image->columns*sizeof(*scanline));
  for (y=0; y < (ssize_t) image->rows; y++)
  {
    p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      ImfFloatToHalf(QuantumScale*GetPixelRed(p),&half_quantum);
      scanline[x].r=half_quantum;
      ImfFloatToHalf(QuantumScale*GetPixelGreen(p),&half_quantum);
      scanline[x].g=half_quantum;
      ImfFloatToHalf(QuantumScale*GetPixelBlue(p),&half_quantum);
      scanline[x].b=half_quantum;
      if (image->matte == MagickFalse)
        ImfFloatToHalf(1.0,&half_quantum);
      else
        ImfFloatToHalf(1.0-QuantumScale*GetPixelOpacity(p),
          &half_quantum);
      scanline[x].a=half_quantum;
      p++;
    }
    ImfOutputSetFrameBuffer(file,scanline-(y*image->columns),1,image->columns);
    ImfOutputWritePixels(file,1);
  }
  (void) ImfCloseOutputFile(file);
  scanline=(ImfRgba *) RelinquishMagickMemory(scanline);
  (void) FileToImage(image,write_info->filename);
  (void) RelinquishUniqueFileResource(write_info->filename);
  write_info=DestroyImageInfo(write_info);
  (void) CloseBlob(image);
  return(MagickTrue);
}
