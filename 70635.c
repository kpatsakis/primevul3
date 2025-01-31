static Image *ReadGIFImage(const ImageInfo *image_info,ExceptionInfo *exception)
{
#define BitSet(byte,bit)  (((byte) & (bit)) == (bit))
#define LSBFirstOrder(x,y)  (((y) << 8) | (x))
#define ThrowGIFException(exception,message) \
{ \
  if (profiles != (LinkedListInfo *) NULL) \
    profiles=DestroyLinkedList(profiles,DestroyGIFProfile); \
  if (global_colormap != (unsigned char *) NULL) \
    global_colormap=(unsigned char *) RelinquishMagickMemory(global_colormap); \
  if (meta_image != (Image *) NULL) \
    meta_image=DestroyImage(meta_image); \
  ThrowReaderException((exception),(message)); \
}

  Image
    *image,
    *meta_image;

  LinkedListInfo
    *profiles;

  MagickBooleanType
    status;

  register ssize_t
    i;

  register unsigned char
    *p;

  size_t
    duration,
    global_colors,
    image_count,
    local_colors,
    one;

  ssize_t
    count,
    opacity;

  unsigned char
    background,
    buffer[257],
    c,
    flag,
    *global_colormap,
    magick[12];

  /*
    Open image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  if (image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  image=AcquireImage(image_info);
  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == MagickFalse)
    {
      image=DestroyImageList(image);
      return((Image *) NULL);
    }
  /*
    Determine if this a GIF file.
  */
  count=ReadBlob(image,6,magick);
  if ((count != 6) || ((LocaleNCompare((char *) magick,"GIF87",5) != 0) &&
      (LocaleNCompare((char *) magick,"GIF89",5) != 0)))
    ThrowReaderException(CorruptImageError,"ImproperImageHeader");
  (void) memset(buffer,0,sizeof(buffer));
  meta_image=AcquireImage(image_info);  /* metadata container */
  meta_image->page.width=ReadBlobLSBShort(image);
  meta_image->page.height=ReadBlobLSBShort(image);
  meta_image->iterations=1;
  flag=(unsigned char) ReadBlobByte(image);
  background=(unsigned char) ReadBlobByte(image);
  c=(unsigned char) ReadBlobByte(image);  /* reserved */
  profiles=(LinkedListInfo *) NULL;
  one=1;
  global_colors=one << (((size_t) flag & 0x07)+1);
  global_colormap=(unsigned char *) AcquireQuantumMemory((size_t)
    MagickMax(global_colors,256),3UL*sizeof(*global_colormap));
  if (global_colormap == (unsigned char *) NULL)
    ThrowGIFException(ResourceLimitError,"MemoryAllocationFailed");
  (void) memset(global_colormap,0,3*MagickMax(global_colors,256)*
    sizeof(*global_colormap));
  if (BitSet((int) flag,0x80) != 0)
    {
      count=ReadBlob(image,(size_t) (3*global_colors),global_colormap);
      if (count != (ssize_t) (3*global_colors))
        ThrowGIFException(CorruptImageError,"InsufficientImageDataInFile");
    }
  duration=0;
  opacity=(-1);
  image_count=0;
  for ( ; ; )
  {
    count=ReadBlob(image,1,&c);
    if (count != 1)
      break;
    if (c == (unsigned char) ';')
      break;  /* terminator */
    if (c == (unsigned char) '!')
      {
        /*
          GIF Extension block.
        */
        count=ReadBlob(image,1,&c);
        if (count != 1)
          ThrowGIFException(CorruptImageError,"UnableToReadExtensionBlock");
        (void) memset(buffer,0,sizeof(buffer));
        switch (c)
        {
          case 0xf9:
          {
            /*
              Read graphics control extension.
            */
            while (ReadBlobBlock(image,buffer) != 0) ;
            meta_image->dispose=(DisposeType) ((buffer[0] >> 2) & 0x07);
            meta_image->delay=((size_t) buffer[2] << 8) | buffer[1];
            if ((ssize_t) (buffer[0] & 0x01) == 0x01)
              opacity=(ssize_t) buffer[3];
            break;
          }
          case 0xfe:
          {
            char
              *comments;

            size_t
              extent,
              offset;

            comments=AcquireString((char *) NULL);
            extent=MagickPathExtent;
            for (offset=0; ; offset+=count)
            {
              count=ReadBlobBlock(image,buffer);
              if (count == 0)
                break;
              buffer[count]='\0';
              if (((ssize_t) (count+offset+MagickPathExtent)) >= (ssize_t) extent)
                {
                  extent<<=1;
                  comments=(char *) ResizeQuantumMemory(comments,extent+
                    MagickPathExtent,sizeof(*comments));
                  if (comments == (char *) NULL)
                    ThrowGIFException(ResourceLimitError,
                      "MemoryAllocationFailed");
                }
              (void) CopyMagickString(&comments[offset],(char *) buffer,extent-
                offset);
            }
            (void) SetImageProperty(meta_image,"comment",comments);
            comments=DestroyString(comments);
            break;
          }
          case 0xff:
          {
            MagickBooleanType
              loop;

            /*
              Read Netscape Loop extension.
            */
            loop=MagickFalse;
            if (ReadBlobBlock(image,buffer) != 0)
              loop=LocaleNCompare((char *) buffer,"NETSCAPE2.0",11) == 0 ?
                MagickTrue : MagickFalse;
            if (loop != MagickFalse)
              while (ReadBlobBlock(image,buffer) != 0)
              {
                meta_image->iterations=((size_t) buffer[2] << 8) | buffer[1];
                if (meta_image->iterations != 0)
                  meta_image->iterations++;
              }
            else
              {
                char
                  name[MaxTextExtent];

                int
                  block_length,
                  info_length,
                  reserved_length;

                MagickBooleanType
                  i8bim,
                  icc,
                  iptc,
                  magick;

                StringInfo
                  *profile;

                unsigned char
                  *info;

                /*
                  Store GIF application extension as a generic profile.
                */
                icc=LocaleNCompare((char *) buffer,"ICCRGBG1012",11) == 0 ?
                  MagickTrue : MagickFalse;
                magick=LocaleNCompare((char *) buffer,"ImageMagick",11) == 0 ?
                  MagickTrue : MagickFalse;
                i8bim=LocaleNCompare((char *) buffer,"MGK8BIM0000",11) == 0 ?
                  MagickTrue : MagickFalse;
                iptc=LocaleNCompare((char *) buffer,"MGKIPTC0000",11) == 0 ?
                  MagickTrue : MagickFalse;
                (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                  "    Reading GIF application extension");
                info=(unsigned char *) AcquireQuantumMemory(255UL,
                  sizeof(*info));
                if (info == (unsigned char *) NULL)
                  ThrowGIFException(ResourceLimitError,
                    "MemoryAllocationFailed");
                (void) memset(info,0,255UL*sizeof(*info));
                reserved_length=255;
                for (info_length=0; ; )
                {
                  block_length=(int) ReadBlobBlock(image,&info[info_length]);
                  if (block_length == 0)
                    break;
                  info_length+=block_length;
                  if (info_length > (reserved_length-255))
                    {
                      reserved_length+=4096;
                      info=(unsigned char *) ResizeQuantumMemory(info,(size_t)
                        reserved_length,sizeof(*info));
                      if (info == (unsigned char *) NULL)
                        {
                          info=(unsigned char *) RelinquishMagickMemory(info);
                          ThrowGIFException(ResourceLimitError,
                            "MemoryAllocationFailed");
                        }
                    }
                }
                profile=BlobToStringInfo(info,(size_t) info_length);
                if (profile == (StringInfo *) NULL)
                  {
                    info=(unsigned char *) RelinquishMagickMemory(info);
                    ThrowGIFException(ResourceLimitError,
                      "MemoryAllocationFailed");
                  }
                if (i8bim != MagickFalse)
                  (void) CopyMagickString(name,"8bim",sizeof(name));
                else if (icc != MagickFalse)
                  (void) CopyMagickString(name,"icc",sizeof(name));
                else if (iptc != MagickFalse)
                  (void) CopyMagickString(name,"iptc",sizeof(name));
                else if (magick != MagickFalse)
                  {
                    (void) CopyMagickString(name,"magick",sizeof(name));
                    meta_image->gamma=StringToDouble((char *) info+6,
                      (char **) NULL);
                  }
                else
                  (void) FormatLocaleString(name,sizeof(name),"gif:%.11s",
                    buffer);
                info=(unsigned char *) RelinquishMagickMemory(info);
                (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                  "      profile name=%s",name);
                if (magick != MagickFalse)
                  profile=DestroyStringInfo(profile);
                else
                  {
                    if (profiles == (LinkedListInfo *) NULL)
                      profiles=NewLinkedList(0);
                    SetStringInfoName(profile,name);
                    (void) AppendValueToLinkedList(profiles,profile);
                  }
              }
            break;
          }
          default:
          {
            while (ReadBlobBlock(image,buffer) != 0) ;
            break;
          }
        }
      }
    if (c != (unsigned char) ',')
      continue;
    if (image_count != 0)
      {
        /*
          Allocate next image structure.
        */
        AcquireNextImage(image_info,image);
        if (GetNextImageInList(image) == (Image *) NULL)
          {
            status=MagickFalse;
            break;
          }
        image=SyncNextImageInList(image);
      }
    image_count++;
    /*
      Read image attributes.
    */
    meta_image->page.x=(ssize_t) ReadBlobLSBShort(image);
    meta_image->page.y=(ssize_t) ReadBlobLSBShort(image);
    meta_image->scene=image->scene;
    (void) CloneImageProperties(image,meta_image);
    DestroyImageProperties(meta_image);
    image->storage_class=PseudoClass;
    image->compression=LZWCompression;
    image->columns=ReadBlobLSBShort(image);
    image->rows=ReadBlobLSBShort(image);
    image->depth=8;
    flag=(unsigned char) ReadBlobByte(image);
    image->interlace=BitSet((int) flag,0x40) != 0 ? GIFInterlace : NoInterlace;
    local_colors=BitSet((int) flag,0x80) == 0 ? global_colors : one <<
      ((size_t) (flag & 0x07)+1);
    image->colors=local_colors;
    if (opacity >= (ssize_t) image->colors)
      {
        image->colors++;
        opacity=(-1);
      }
    image->ticks_per_second=100;
    image->matte=opacity >= 0 ? MagickTrue : MagickFalse;
    if ((image->columns == 0) || (image->rows == 0))
      ThrowGIFException(CorruptImageError,"NegativeOrZeroImageSize");
    /*
      Inititialize colormap.
    */
    if (AcquireImageColormap(image,image->colors) == MagickFalse)
      ThrowGIFException(ResourceLimitError,"MemoryAllocationFailed");
    if (BitSet((int) flag,0x80) == 0)
      {
        /*
          Use global colormap.
        */
        p=global_colormap;
        for (i=0; i < (ssize_t) image->colors; i++)
        {
          image->colormap[i].red=ScaleCharToQuantum(*p++);
          image->colormap[i].green=ScaleCharToQuantum(*p++);
          image->colormap[i].blue=ScaleCharToQuantum(*p++);
          if (i == opacity)
            {
              image->colormap[i].opacity=(Quantum) TransparentOpacity;
              image->transparent_color=image->colormap[opacity];
            }
        }
        image->background_color=image->colormap[MagickMin((ssize_t) background,
          (ssize_t) image->colors-1)];
      }
    else
      {
        unsigned char
          *colormap;

        /*
          Read local colormap.
        */
        colormap=(unsigned char *) AcquireQuantumMemory((size_t)
          MagickMax(local_colors,256),3UL*sizeof(*colormap));
        if (colormap == (unsigned char *) NULL)
          ThrowGIFException(ResourceLimitError,"MemoryAllocationFailed");
        (void) memset(colormap,0,3*MagickMax(local_colors,256)*
          sizeof(*colormap));
        count=ReadBlob(image,(3*local_colors)*sizeof(*colormap),colormap);
        if (count != (ssize_t) (3*local_colors))
          {
            colormap=(unsigned char *) RelinquishMagickMemory(colormap);
            ThrowGIFException(CorruptImageError,"InsufficientImageDataInFile");
          }
        p=colormap;
        for (i=0; i < (ssize_t) image->colors; i++)
        {
          image->colormap[i].red=ScaleCharToQuantum(*p++);
          image->colormap[i].green=ScaleCharToQuantum(*p++);
          image->colormap[i].blue=ScaleCharToQuantum(*p++);
          if (i == opacity)
            image->colormap[i].opacity=(Quantum) TransparentOpacity;
        }
        colormap=(unsigned char *) RelinquishMagickMemory(colormap);
      }
    if (image->gamma == 1.0)
      {
        for (i=0; i < (ssize_t) image->colors; i++)
          if (IsGrayPixel(image->colormap+i) == MagickFalse)
            break;
        (void) SetImageColorspace(image,i == (ssize_t) image->colors ?
          LinearGRAYColorspace : RGBColorspace);
      }
    if ((image_info->ping != MagickFalse) && (image_info->number_scenes != 0))
      if (image->scene >= (image_info->scene+image_info->number_scenes-1))
        break;
    status=SetImageExtent(image,image->columns,image->rows);
    if (status == MagickFalse)
      {
        if (profiles != (LinkedListInfo *) NULL)
          profiles=DestroyLinkedList(profiles,DestroyGIFProfile);
        global_colormap=(unsigned char *) RelinquishMagickMemory(
          global_colormap);
        meta_image=DestroyImage(meta_image);
        InheritException(exception,&image->exception);
        return(DestroyImageList(image));
      }
    /*
      Decode image.
    */
    if (image_info->ping != MagickFalse)
      status=PingGIFImage(image);
    else
      status=DecodeImage(image,opacity);
    InheritException(exception,&image->exception);
    if ((image_info->ping == MagickFalse) && (status == MagickFalse))
      ThrowGIFException(CorruptImageError,"CorruptImage");
    if (profiles != (LinkedListInfo *) NULL)
      {
        StringInfo
          *profile;

        /*
          Set image profiles.
        */
        ResetLinkedListIterator(profiles);
        profile=(StringInfo *) GetNextValueInLinkedList(profiles);
        while (profile != (StringInfo *) NULL)
        {
          (void) SetImageProfile(image,GetStringInfoName(profile),profile);
          profile=(StringInfo *) GetNextValueInLinkedList(profiles);
        }
        profiles=DestroyLinkedList(profiles,DestroyGIFProfile);
      }
    duration+=image->delay*image->iterations;
    if (image_info->number_scenes != 0)
      if (image->scene >= (image_info->scene+image_info->number_scenes-1))
        break;
    opacity=(-1);
    status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) image->scene-
      1,image->scene);
    if (status == MagickFalse)
      break;
  }
  image->duration=duration;
  if (profiles != (LinkedListInfo *) NULL)
    profiles=DestroyLinkedList(profiles,DestroyGIFProfile);
  meta_image=DestroyImage(meta_image);
  global_colormap=(unsigned char *) RelinquishMagickMemory(global_colormap);
  if ((image->columns == 0) || (image->rows == 0))
    ThrowReaderException(CorruptImageError,"NegativeOrZeroImageSize");
  (void) CloseBlob(image);
  if (status == MagickFalse)
    return(DestroyImageList(image));
  return(GetFirstImageInList(image));
}
