static void InitPSDInfo(const Image *image,PSDInfo *info)
{
  info->version=1;
  info->columns=image->columns;
  info->rows=image->rows;
  info->mode=10; /* Set the mode to a value that won't change the colorspace */
  info->channels=1U;
  info->min_channels=1U;
  if (image->storage_class == PseudoClass)
    info->mode=2; /* indexed mode */
  else
    {
      info->channels=(unsigned short) image->number_channels;
      info->min_channels=info->channels;
      if (image->alpha_trait == BlendPixelTrait)
        info->min_channels--;
    }
}
