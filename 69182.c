cupsdCloseClient(cupsd_client_t *con)	/* I - Client to close */
{
  int		partial;		/* Do partial close for SSL? */


  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Closing connection.");

 /*
  * Flush pending writes before closing...
  */

  httpFlushWrite(con->http);

  partial = 0;

  if (con->pipe_pid != 0)
  {
   /*
    * Stop any CGI process...
    */

    cupsdEndProcess(con->pipe_pid, 1);
    con->pipe_pid = 0;
  }

  if (con->file >= 0)
  {
    cupsdRemoveSelect(con->file);

    close(con->file);
    con->file = -1;
  }

 /*
  * Close the socket and clear the file from the input set for select()...
  */

  if (httpGetFd(con->http) >= 0)
  {
    cupsArrayRemove(ActiveClients, con);
    cupsdSetBusyState();

#ifdef HAVE_SSL
   /*
    * Shutdown encryption as needed...
    */

    if (httpIsEncrypted(con->http))
      partial = 1;
#endif /* HAVE_SSL */

    if (partial)
    {
     /*
      * Only do a partial close so that the encrypted client gets everything.
      */

      httpShutdown(con->http);
      cupsdAddSelect(httpGetFd(con->http), (cupsd_selfunc_t)cupsdReadClient,
                     NULL, con);

      cupsdLogClient(con, CUPSD_LOG_DEBUG, "Waiting for socket close.");
    }
    else
    {
     /*
      * Shut the socket down fully...
      */

      cupsdRemoveSelect(httpGetFd(con->http));
      httpClose(con->http);
      con->http = NULL;
    }
  }

  if (!partial)
  {
   /*
    * Free memory...
    */

    cupsdRemoveSelect(httpGetFd(con->http));

    httpClose(con->http);

    if (con->filename)
    {
      unlink(con->filename);
      cupsdClearString(&con->filename);
    }

    cupsdClearString(&con->command);
    cupsdClearString(&con->options);
    cupsdClearString(&con->query_string);

    if (con->request)
    {
      ippDelete(con->request);
      con->request = NULL;
    }

    if (con->response)
    {
      ippDelete(con->response);
      con->response = NULL;
    }

    if (con->language)
    {
      cupsLangFree(con->language);
      con->language = NULL;
    }

#ifdef HAVE_AUTHORIZATION_H
    if (con->authref)
    {
      AuthorizationFree(con->authref, kAuthorizationFlagDefaults);
      con->authref = NULL;
    }
#endif /* HAVE_AUTHORIZATION_H */

   /*
    * Re-enable new client connections if we are going back under the
    * limit...
    */

    if (cupsArrayCount(Clients) == MaxClients)
      cupsdResumeListening();

   /*
    * Compact the list of clients as necessary...
    */

    cupsArrayRemove(Clients, con);

    free(con);
  }

  return (partial);
}
