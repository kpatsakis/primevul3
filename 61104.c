static Image *ReadDJVUImage(const ImageInfo *image_info,
  ExceptionInfo *exception)
{
  const char
    *url;

  ddjvu_message_t
    *message;

  Image
    *image,
    *images;

  int
    logging,
    use_cache;

  LoadContext
    *lc;

  MagickBooleanType
    status;

  register ssize_t
    i;

  /*
   * Open image file.
   */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);


  if (image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s", image_info->filename);

  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickSignature);


  logging = LogMagickEvent(CoderEvent,GetMagickModule(),"enter ReadDJVUImage()");
  (void) logging;

  image = AcquireImage(image_info); /* mmc: ?? */


  lc = (LoadContext *) NULL;
  status = OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == MagickFalse)
    ThrowReaderException(FileOpenError,"UnableToOpenFile");
  /*
    Verify DJVU signature.
  */
#if 0
  count = ReadBlob(image,8,(unsigned char *) magic_number);

  /* IsDJVU(const unsigned char *magick,const size_t length) */
  if (memcmp(magic_number,"AT&TFORM",8) != 0)
    ThrowReaderException(CorruptImageError,"ImproperImageHeader");
#endif


  /*
   * Allocate a LoadContext structure.
   */
  lc = (LoadContext *) AcquireMagickMemory(sizeof(*lc));
  if (lc == NULL)
    ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");


  /*
   * Initialize members of the MngInfo structure.
   */
  (void) ResetMagickMemory(lc,0,sizeof(LoadContext));

  lc->image = image;
  lc->pages = 0;
  lc->context = ddjvu_context_create("ImageMagick djvu loader"); /* g_program_name */

  ddjvu_cache_set_size(lc->context, 1); /* right? */
  use_cache = 0;
  /* document: here we don't have a filename, but, for the sake of generality, a FILE* ! */
  url="http://www.imagemagick.org/fake.djvu";
  lc->document = ddjvu_document_create(lc->context, url, use_cache); /* don't cache */
  ddjvu_document_set_user_data(lc->document, lc);


  /* now we wait the message-request for data: */
  message = ddjvu_message_wait(lc->context);

  if (message->m_any.tag != DDJVU_NEWSTREAM) {
          /* fixme: the djvu context, document! */

          ddjvu_document_release(lc->document);
          ddjvu_context_release(lc->context);

          RelinquishMagickMemory(lc);

          ThrowReaderException(ResourceLimitError,"Djvu initial message: unexpected type");
          return NULL;    /* error! */
  };

  lc->streamid = message->m_newstream.streamid;
  ddjvu_message_pop(lc->context);

  message = pump_data_until_message(lc,image);
  /* now process the messages: */


  if (message) do {
          process_message(message);
          ddjvu_message_pop(lc->context);
  } while ((message = ddjvu_message_peek(lc->context)));

  /* fixme: i hope we have not read any messages pertinent(?) related to the page itself!  */

  while (lc->pages == 0) {
          message = ddjvu_message_wait(lc->context);
          process_message(message);
          ddjvu_message_pop(lc->context);
  }

  images=NewImageList();
  i=0;
  if (image_info->number_scenes != 0)
    i=image_info->scene;
  for ( ; i < (ssize_t) lc->pages; i++)
  {
    image=ReadOneDJVUImage(lc,i,image_info,exception);
    if (image == (Image *) NULL)
      break;
    image->scene=i;
    AppendImageToList(&images,CloneImageList(image,exception));
    images->extent=GetBlobSize(image);
    if (image_info->number_scenes != 0)
      if (image->scene >= (image_info->scene+image_info->number_scenes-1))
        break;
  }
  djvu_close_lc(lc);
  (void) CloseBlob(images);
  if (image != (Image *) NULL)
    image=DestroyImageList(image);

#if 0
  if ((image->page.width == 0) && (image->page.height == 0))
    {
      image->page.width = image->columns+image->page.x;
      image->page.height = image->rows+image->page.y;
    }
  if (image->columns == 0 || image->rows == 0)
    {
      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "exit ReadDJVUImage() with error.");
      ThrowReaderException(CorruptImageError,"CorruptImage");
    }

  if (logging != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),"exit ReadDJVUImage()");
#endif


  return(GetFirstImageInList(images));
}
