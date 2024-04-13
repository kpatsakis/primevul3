static CURLcode smtp_disconnect(struct connectdata *conn, bool dead_connection)
{
  struct smtp_conn *smtpc = &conn->proto.smtpc;

  /* We cannot send quit unconditionally. If this connection is stale or
     bad in any way, sending quit and waiting around here will make the
     disconnect wait in vain and cause more problems than we need to. */

  /* The SMTP session may or may not have been allocated/setup at this
     point! */
  if(!dead_connection && smtpc->pp.conn && smtpc->pp.conn->bits.protoconnstart)
    if(!smtp_perform_quit(conn))
      (void)smtp_block_statemach(conn); /* ignore errors on QUIT */

  /* Disconnect from the server */
  Curl_pp_disconnect(&smtpc->pp);

  /* Cleanup the SASL module */
  Curl_sasl_cleanup(conn, smtpc->sasl.authused);

  /* Cleanup our connection based variables */
  Curl_safefree(smtpc->domain);

  return CURLE_OK;
}
