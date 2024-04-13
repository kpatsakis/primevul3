cupsdCloseAllClients(void)
{
  cupsd_client_t	*con;		/* Current client */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cupsdCloseAllClients() Clients=%d", cupsArrayCount(Clients));

  for (con = (cupsd_client_t *)cupsArrayFirst(Clients);
       con;
       con = (cupsd_client_t *)cupsArrayNext(Clients))
    if (cupsdCloseClient(con))
      cupsdCloseClient(con);
}
