static MagickStatusType ReadPSDMergedImage(Image* image,
  const PSDInfo* psd_info,ExceptionInfo *exception)
{
  MagickOffsetType
    *offsets;

  MagickStatusType
    status;

  PSDCompressionType
    compression;

  register ssize_t
    i;

  compression=(PSDCompressionType) ReadBlobMSBShort(image);
  image->compression=ConvertPSDCompression(compression);

  if (compression != Raw && compression != RLE)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),
        TypeWarning,"CompressionNotSupported","'%.20g'",(double) compression);
      return(MagickFalse);
    }

  offsets=(MagickOffsetType *) NULL;
  if (compression == RLE)
  {
    offsets=ReadPSDRLEOffsets(image,psd_info,image->rows*psd_info->channels);
    if (offsets == (MagickOffsetType *) NULL)
      ThrowBinaryException(ResourceLimitError,"MemoryAllocationFailed",
        image->filename);
  }

  status=MagickTrue;
  for (i=0; i < (ssize_t) psd_info->channels; i++)
  {
    if (compression == RLE)
      status=ReadPSDChannelRLE(image,psd_info,i,offsets+(i*image->rows),
        exception);
    else
      status=ReadPSDChannelRaw(image,psd_info->channels,i,exception);

    if (status == MagickFalse)
      break;
    status=SetImageProgress(image,LoadImagesTag,i,psd_info->channels);
    if (status == MagickFalse)
      break;
  }

  if (image->colorspace == CMYKColorspace)
    (void) NegateImage(image,MagickFalse);

  if (offsets != (MagickOffsetType *) NULL)
    offsets=(MagickOffsetType *) RelinquishMagickMemory(offsets);

  return(status);
}
