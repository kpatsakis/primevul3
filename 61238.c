static MagickBooleanType WriteImageChannels(const PSDInfo *psd_info,
  const ImageInfo *image_info,Image *image,Image *next_image,
  const MagickBooleanType separate)
{
  int
    i;

  size_t
    channels,
    packet_size;

  unsigned char
    *compact_pixels;

  /*
    Write uncompressed pixels as separate planes.
  */
  channels=1;
  packet_size=next_image->depth > 8UL ? 2UL : 1UL;
  compact_pixels=(unsigned char *) NULL;
  if (next_image->compression == RLECompression)
    {
      compact_pixels=(unsigned char *) AcquireQuantumMemory(2*channels*
        next_image->columns,packet_size*sizeof(*compact_pixels));
      if (compact_pixels == (unsigned char *) NULL)
        ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    }
  i=0;
  if (IsGrayImage(next_image,&next_image->exception) != MagickFalse)
    {
      if (next_image->compression == RLECompression)
        {
          /*
            Packbits compression.
          */
          (void) WriteBlobMSBShort(image,1);
          WritePackbitsLength(psd_info,image_info,image,next_image,
            compact_pixels,GrayQuantum);
          if (next_image->matte != MagickFalse)
            WritePackbitsLength(psd_info,image_info,image,next_image,
              compact_pixels,AlphaQuantum);
        }
      WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
        GrayQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
        MagickFalse);
      if (next_image->matte != MagickFalse)
        WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
          AlphaQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
          MagickFalse);
      (void) SetImageProgress(image,SaveImagesTag,0,1);
    }
  else
    if (next_image->storage_class == PseudoClass)
      {
        if (next_image->compression == RLECompression)
          {
            /*
              Packbits compression.
            */
            (void) WriteBlobMSBShort(image,1);
            WritePackbitsLength(psd_info,image_info,image,next_image,
              compact_pixels,IndexQuantum);
            if (next_image->matte != MagickFalse)
              WritePackbitsLength(psd_info,image_info,image,next_image,
                compact_pixels,AlphaQuantum);
          }
        WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
          IndexQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
          MagickFalse);
        if (next_image->matte != MagickFalse)
          WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
            AlphaQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
            MagickFalse);
        (void) SetImageProgress(image,SaveImagesTag,0,1);
      }
    else
      {
        if (next_image->colorspace == CMYKColorspace)
          (void) NegateImage(next_image,MagickFalse);
        if (next_image->compression == RLECompression)
          {
            /*
              Packbits compression.
            */
            (void) WriteBlobMSBShort(image,1);
            WritePackbitsLength(psd_info,image_info,image,next_image,
              compact_pixels,RedQuantum);
            WritePackbitsLength(psd_info,image_info,image,next_image,
              compact_pixels,GreenQuantum);
            WritePackbitsLength(psd_info,image_info,image,next_image,
              compact_pixels,BlueQuantum);
            if (next_image->colorspace == CMYKColorspace)
              WritePackbitsLength(psd_info,image_info,image,next_image,
                compact_pixels,BlackQuantum);
            if (next_image->matte != MagickFalse)
              WritePackbitsLength(psd_info,image_info,image,next_image,
                compact_pixels,AlphaQuantum);
          }
        (void) SetImageProgress(image,SaveImagesTag,0,6);
        WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
          RedQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
          MagickFalse);
        (void) SetImageProgress(image,SaveImagesTag,1,6);
        WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
          GreenQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
          MagickFalse);
        (void) SetImageProgress(image,SaveImagesTag,2,6);
        WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
          BlueQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
          MagickFalse);
        (void) SetImageProgress(image,SaveImagesTag,3,6);
        if (next_image->colorspace == CMYKColorspace)
          WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
            BlackQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
            MagickFalse);
        (void) SetImageProgress(image,SaveImagesTag,4,6);
        if (next_image->matte != MagickFalse)
          WriteOneChannel(psd_info,image_info,image,next_image,compact_pixels,
            AlphaQuantum,(i++ == 0) || (separate != MagickFalse) ? MagickTrue :
            MagickFalse);
        (void) SetImageProgress(image,SaveImagesTag,5,6);
        if (next_image->colorspace == CMYKColorspace)
          (void) NegateImage(next_image,MagickFalse);
      }
  if (next_image->compression == RLECompression)
    compact_pixels=(unsigned char *) RelinquishMagickMemory(compact_pixels);
  return(MagickTrue);
}
