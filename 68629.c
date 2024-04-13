static void smtp_to_smtps(struct connectdata *conn)
{
  /* Change the connection handler */
  conn->handler = &Curl_handler_smtps;

  /* Set the connection's upgraded to TLS flag */
  conn->tls_upgraded = TRUE;
}
