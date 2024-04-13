static MagickBooleanType WriteFAXImage(const ImageInfo *image_info,Image *image)
{
  ImageInfo
    *write_info;

  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  /*
    Open output image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  write_info=CloneImageInfo(image_info);
  (void) CopyMagickString(write_info->magick,"FAX",MaxTextExtent);
  scene=0;
  do
  {
    /*
      Convert MIFF to monochrome.
    */
    (void) TransformImageColorspace(image,sRGBColorspace);
    status=HuffmanEncodeImage(write_info,image,image);
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,
      GetImageListLength(image));
    if (status == MagickFalse)
      break;
  } while (write_info->adjoin != MagickFalse);
  write_info=DestroyImageInfo(write_info);
  (void) CloseBlob(image);
  return(status);
}
