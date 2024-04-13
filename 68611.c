static CURLcode smtp_perform_helo(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;

  smtpc->sasl.authused = SASL_AUTH_NONE; /* No authentication mechanism used
                                            in smtp connections */

  /* Send the HELO command */
  result = Curl_pp_sendf(&smtpc->pp, "HELO %s", smtpc->domain);

  if(!result)
    state(conn, SMTP_HELO);

  return result;
}
