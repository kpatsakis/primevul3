MagickExport MagickBooleanType SetImageDepth(Image *image,
  const size_t depth)
{
  return(SetImageChannelDepth(image,CompositeChannels,depth));
}
