write_pipe(cupsd_client_t *con)		/* I - Client connection */
{
  cupsdLogClient(con, CUPSD_LOG_DEBUG2, "write_pipe: CGI output on fd %d.", con->file);

  con->file_ready = 1;

  cupsdRemoveSelect(con->file);
  cupsdAddSelect(httpGetFd(con->http), NULL, (cupsd_selfunc_t)cupsdWriteClient, con);

  cupsdLogClient(con, CUPSD_LOG_DEBUG, "CGI data ready to be sent.");
}
