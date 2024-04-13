static CURLcode smtp_setup_connection(struct connectdata *conn)
{
  struct Curl_easy *data = conn->data;
  CURLcode result;

  /* Clear the TLS upgraded flag */
  conn->tls_upgraded = FALSE;

  /* Initialise the SMTP layer */
  result = smtp_init(conn);
  if(result)
    return result;

  data->state.path++;   /* don't include the initial slash */

  return CURLE_OK;
}
