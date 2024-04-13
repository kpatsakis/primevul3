message_tag_name(ddjvu_message_tag_t tag)
{
   static char* names[] =
      {
         "ERROR",
         "INFO",
         "NEWSTREAM",
         "DOCINFO",
         "PAGEINFO",
         "RELAYOUT",
         "REDISPLAY",
         "CHUNK",
         "THUMBNAIL",
         "PROGRESS",
      };
   if (tag <= DDJVU_PROGRESS)
      return names[tag];
   else {
      /* bark! */
      return 0;
   }
}
