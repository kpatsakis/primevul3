cupsdUpdateCGI(void)
{
  char		*ptr,			/* Pointer to end of line in buffer */
		message[1024];		/* Pointer to message text */
  int		loglevel;		/* Log level for message */


  while ((ptr = cupsdStatBufUpdate(CGIStatusBuffer, &loglevel,
                                   message, sizeof(message))) != NULL)
  {
    if (loglevel == CUPSD_LOG_INFO)
      cupsdLogMessage(CUPSD_LOG_INFO, "%s", message);

    if (!strchr(CGIStatusBuffer->buffer, '\n'))
      break;
  }

  if (ptr == NULL && !CGIStatusBuffer->bufused)
  {
   /*
    * Fatal error on pipe - should never happen!
    */

    cupsdLogMessage(CUPSD_LOG_CRIT,
                    "cupsdUpdateCGI: error reading from CGI error pipe - %s",
                    strerror(errno));
  }
}
