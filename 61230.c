static MagickStatusType ReadPSDChannel(Image *image,const PSDInfo *psd_info,
  const LayerInfo* layer_info,const size_t channel,
  const PSDCompressionType compression,ExceptionInfo *exception)
{
  MagickOffsetType
    offset;

  MagickStatusType
    status;

  if (layer_info->channel_info[channel].type < -1)
  {
    /* ignore user supplied layer mask */
    SeekBlob(image,layer_info->channel_info[channel].size-2,SEEK_CUR);
    return(MagickTrue);
  }

  offset=TellBlob(image);
  status=MagickTrue;
  switch(compression)
  {
    case Raw:
      return(ReadPSDChannelRaw(image,psd_info->channels,
        layer_info->channel_info[channel].type,exception));
    case RLE:
      {
        MagickOffsetType
          *offsets;

        offsets=ReadPSDRLEOffsets(image,psd_info,image->rows);
        if (offsets == (MagickOffsetType *) NULL)
          ThrowBinaryException(ResourceLimitError,"MemoryAllocationFailed",
            image->filename);
        status=ReadPSDChannelRLE(image,psd_info,
                 layer_info->channel_info[channel].type,offsets,exception);
        offsets=(MagickOffsetType *) RelinquishMagickMemory(offsets);
      }
      break;
    case ZipWithPrediction:
    case ZipWithoutPrediction:
#ifdef MAGICKCORE_ZLIB_DELEGATE
      status=ReadPSDChannelZip(image,layer_info->channels,
        layer_info->channel_info[channel].type,compression,
        layer_info->channel_info[channel].size-2,exception);
#else
      SeekBlob(image,offset+layer_info->channel_info[channel].size-2,SEEK_SET);
      (void) ThrowMagickException(exception,GetMagickModule(),
          MissingDelegateWarning,"DelegateLibrarySupportNotBuiltIn",
            "'%s' (ZLIB)",image->filename);
#endif
      break;
    default:
      SeekBlob(image,offset+layer_info->channel_info[channel].size-2,SEEK_SET);
      (void) ThrowMagickException(exception,GetMagickModule(),TypeWarning,
        "CompressionNotSupported","'%.20g'",(double) compression);
      break;
  }

  if (status == MagickFalse)
    SeekBlob(image,offset+layer_info->channel_info[channel].size-2,SEEK_SET);

  return(status);
}
