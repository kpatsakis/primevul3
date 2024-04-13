MagickExport MagickBooleanType DrawClipPath(Image *image,
  const DrawInfo *draw_info,const char *name)
{
  char
    clip_mask[MaxTextExtent];

  const char
    *value;

  DrawInfo
    *clone_info;

  MagickStatusType
    status;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(draw_info != (const DrawInfo *) NULL);
  (void) FormatLocaleString(clip_mask,MaxTextExtent,"%s",name);
  value=GetImageArtifact(image,clip_mask);
  if (value == (const char *) NULL)
    return(MagickFalse);
  if (image->clip_mask == (Image *) NULL)
    {
      Image
        *clip_mask;

      clip_mask=CloneImage(image,image->columns,image->rows,MagickTrue,
        &image->exception);
      if (clip_mask == (Image *) NULL)
        return(MagickFalse);
      (void) SetImageClipMask(image,clip_mask);
      clip_mask=DestroyImage(clip_mask);
    }
  (void) QueryColorDatabase("#00000000",&image->clip_mask->background_color,
    &image->exception);
  image->clip_mask->background_color.opacity=(Quantum) TransparentOpacity;
  (void) SetImageBackgroundColor(image->clip_mask);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(DrawEvent,GetMagickModule(),"\nbegin clip-path %s",
      draw_info->clip_mask);
  clone_info=CloneDrawInfo((ImageInfo *) NULL,draw_info);
  (void) CloneString(&clone_info->primitive,value);
  (void) QueryColorDatabase("#ffffff",&clone_info->fill,&image->exception);
  clone_info->clip_mask=(char *) NULL;
  status=DrawImage(image->clip_mask,clone_info);
  status&=NegateImage(image->clip_mask,MagickFalse);
  clone_info=DestroyDrawInfo(clone_info);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(DrawEvent,GetMagickModule(),"end clip-path");
  return(status != 0 ? MagickTrue : MagickFalse);
}
