static CURLcode smtp_connect(struct connectdata *conn, bool *done)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  struct pingpong *pp = &smtpc->pp;

  *done = FALSE; /* default to not done yet */

  /* We always support persistent connections in SMTP */
  connkeep(conn, "SMTP default");

  /* Set the default response time-out */
  pp->response_time = RESP_TIMEOUT;
  pp->statemach_act = smtp_statemach_act;
  pp->endofresp = smtp_endofresp;
  pp->conn = conn;

  /* Initialize the SASL storage */
  Curl_sasl_init(&smtpc->sasl, &saslsmtp);

  /* Initialise the pingpong layer */
  Curl_pp_init(pp);

  /* Parse the URL options */
  result = smtp_parse_url_options(conn);
  if(result)
    return result;

  /* Parse the URL path */
  result = smtp_parse_url_path(conn);
  if(result)
    return result;

  /* Start off waiting for the server greeting response */
  state(conn, SMTP_SERVERGREET);

  result = smtp_multi_statemach(conn, done);

  return result;
}
