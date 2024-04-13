static MagickStatusType ReadPSDLayer(Image *image,const PSDInfo *psd_info,
  LayerInfo* layer_info,ExceptionInfo *exception)
{
  char
    message[MaxTextExtent];

  MagickStatusType
    status;

  PSDCompressionType
    compression;

  ssize_t
    j;

  if (image->debug != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
      "    setting up new layer image");
  (void) SetImageBackgroundColor(layer_info->image);
  layer_info->image->compose=PSDBlendModeToCompositeOperator(
    layer_info->blendkey);
  if (layer_info->visible == MagickFalse)
    layer_info->image->compose=NoCompositeOp;
  if (psd_info->mode == CMYKMode)
    SetImageColorspace(layer_info->image,CMYKColorspace);
  if ((psd_info->mode == BitmapMode) || (psd_info->mode == GrayscaleMode) ||
     (psd_info->mode == DuotoneMode))
   SetImageColorspace(layer_info->image,GRAYColorspace);
  /*
    Set up some hidden attributes for folks that need them.
  */
  (void) FormatLocaleString(message,MaxTextExtent,"%.20gld",
    (double) layer_info->page.x);
  (void) SetImageArtifact(layer_info->image,"psd:layer.x",message);
  (void) FormatLocaleString(message,MaxTextExtent,"%.20g",
    (double) layer_info->page.y);
  (void) SetImageArtifact(layer_info->image,"psd:layer.y",message);
  (void) FormatLocaleString(message,MaxTextExtent,"%.20g",(double)
    layer_info->opacity);
  (void) SetImageArtifact(layer_info->image,"psd:layer.opacity",message);
  (void) SetImageProperty(layer_info->image,"label",(char *) layer_info->name);

  status=MagickTrue;
  for (j=0; j < (ssize_t) layer_info->channels; j++)
  {
    if (image->debug != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
        "    reading data for channel %.20g",(double) j);

    compression=(PSDCompressionType) ReadBlobMSBShort(layer_info->image);
    layer_info->image->compression=ConvertPSDCompression(compression);
    if (layer_info->channel_info[j].type == -1)
      layer_info->image->matte=MagickTrue;

    status=ReadPSDChannel(layer_info->image,psd_info,layer_info,j,
      compression,exception);
    InheritException(exception,&layer_info->image->exception);

    if (status == MagickFalse)
      break;
  }

  if (status != MagickFalse)
    status=CorrectPSDOpacity(layer_info,exception);

  if (status != MagickFalse && layer_info->image->colorspace == CMYKColorspace)
   (void) NegateImage(layer_info->image,MagickFalse);

  return(status);
}
