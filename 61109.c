process_message(ddjvu_message_t *message)
{

#if 0
   ddjvu_context_t* context= message->m_any.context;
#endif

   if (! message)
      return(-1);
#if DEBUG
   printf("*** %s: %s.\n",__FUNCTION__, message_tag_name(message->m_any.tag));
#endif


   switch (message->m_any.tag){
   case DDJVU_DOCINFO:
   {
      ddjvu_document_t* document= message->m_any.document;
      /* ddjvu_document_decoding_status  is set by libdjvu! */
      /* we have some info on the document  */
      LoadContext *lc = (LoadContext *) ddjvu_document_get_user_data(document);
      lc->pages = ddjvu_document_get_pagenum(document);
#if DEBUG
      printf("the doc has %d pages\n", ddjvu_document_get_pagenum(document));
#endif
      break;
   }
   case DDJVU_CHUNK:
#if DEBUG
           printf("the name of the chunk is: %s\n", message->m_chunk.chunkid);
#endif
           break;


   case DDJVU_RELAYOUT:
   case DDJVU_PAGEINFO:
   {
#if 0
      ddjvu_page_t* page = message->m_any.page;
      page_info* info = ddjvu_page_get_user_data(page);

      printf("page decoding status: %d %s%s%s\n",
             ddjvu_page_decoding_status(page),
             status_color, status_name(ddjvu_page_decoding_status(page)), color_reset);

      printf("the page LAYOUT changed: width x height: %d x %d @ %d dpi. Version %d, type %d\n",
             ddjvu_page_get_width(page),
             ddjvu_page_get_height(page),
             ddjvu_page_get_resolution(page),
             ddjvu_page_get_version(page),
             /* DDJVU_PAGETYPE_BITONAL */
             ddjvu_page_get_type(page));

      info->info = 1;
#endif
      break;
   }

   case DDJVU_REDISPLAY:
   {

#if 0
    ddjvu_page_t* page = message->m_any.page;
      page_info* info = ddjvu_page_get_user_data(page);

      printf("the page can/should be REDISPLAYED\n");
      info->display = 1;
#endif
      break;
   }

   case DDJVU_PROGRESS:
#if DEBUG
           printf("PROGRESS:\n");
#endif
           break;
   case DDJVU_ERROR:
           printf("simply ERROR!\n message:\t%s\nfunction:\t%s(file %s)\nlineno:\t%d\n",
                  message->m_error.message,
                  message->m_error.function,
                  message->m_error.filename,
                  message->m_error.lineno);
           break;
   case DDJVU_INFO:
#if DEBUG
           printf("INFO: %s!\n", message->m_info.message);
#endif
           break;
   default:
      printf("unexpected\n");
   };
  return(message->m_any.tag);
}
