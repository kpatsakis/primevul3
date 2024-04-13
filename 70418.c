static inline MagickBooleanType SetStreamBuffering(const ImageInfo *image_info,
  Image *image)
{
  const char
    *option;

  int
    status;

  size_t
    size;

  size=16384;
  option=GetImageOption(image_info,"stream:buffer-size");
  if (option != (const char *) NULL)
    size=StringToUnsignedLong(option);
  status=setvbuf(image->blob->file_info.file,(char *) NULL,size == 0 ?
    _IONBF : _IOFBF,size);
  return(status == 0 ? MagickTrue : MagickFalse);
}
