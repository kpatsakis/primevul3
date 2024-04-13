cupsd_start_tls(cupsd_client_t    *con,	/* I - Client connection */
                http_encryption_t e)	/* I - Encryption mode */
{
  if (httpEncryption(con->http, e))
  {
    cupsdLogClient(con, CUPSD_LOG_ERROR, "Unable to encrypt connection: %s",
                   cupsLastErrorString());
    return (-1);
  }

  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Connection now encrypted.");
  return (0);
}
