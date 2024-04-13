static void ParseImageResourceBlocks(Image *image,
  const unsigned char *blocks,size_t length,
  MagickBooleanType *has_merged_image,ExceptionInfo *exception)
{
  const unsigned char
    *p;

  StringInfo
    *profile;

  unsigned char
    name_length;

  unsigned int
    count;

  unsigned short
    id,
    short_sans;

  if (length < 16)
    return;
  profile=BlobToStringInfo((const unsigned char *) NULL,length);
  SetStringInfoDatum(profile,blocks);
  (void) SetImageProfile(image,"8bim",profile,exception);
  profile=DestroyStringInfo(profile);
  for (p=blocks; (p >= blocks) && (p < (blocks+length-7)); )
  {
    if (LocaleNCompare((const char *) p,"8BIM",4) != 0)
      break;
    p+=4;
    p=PushShortPixel(MSBEndian,p,&id);
    p=PushCharPixel(p,&name_length);
    if ((name_length % 2) == 0)
      name_length++;
    p+=name_length;
    if (p > (blocks+length-4))
      return;
    p=PushLongPixel(MSBEndian,p,&count);
    if ((p+count) > (blocks+length))
      return;
    switch (id)
    {
      case 0x03ed:
      {
        char
          value[MagickPathExtent];

        unsigned short
          resolution;

        /*
          Resolution info.
        */
        if (count < 16)
          return;
        p=PushShortPixel(MSBEndian,p,&resolution);
        image->resolution.x=(double) resolution;
        (void) FormatLocaleString(value,MagickPathExtent,"%g",
          image->resolution.x);
        (void) SetImageProperty(image,"tiff:XResolution",value,exception);
        p=PushShortPixel(MSBEndian,p,&short_sans);
        p=PushShortPixel(MSBEndian,p,&short_sans);
        p=PushShortPixel(MSBEndian,p,&short_sans);
        p=PushShortPixel(MSBEndian,p,&resolution);
        image->resolution.y=(double) resolution;
        (void) FormatLocaleString(value,MagickPathExtent,"%g",
          image->resolution.y);
        (void) SetImageProperty(image,"tiff:YResolution",value,exception);
        p=PushShortPixel(MSBEndian,p,&short_sans);
        p=PushShortPixel(MSBEndian,p,&short_sans);
        p=PushShortPixel(MSBEndian,p,&short_sans);
        image->units=PixelsPerInchResolution;
        break;
      }
      case 0x0421:
      {
        if ((count > 3) && (*(p+4) == 0))
          *has_merged_image=MagickFalse;
        p+=count;
        break;
      }
      default:
      {
        p+=count;
        break;
      }
    }
    if ((count & 0x01) != 0)
      p++;
  }
  return;
}
