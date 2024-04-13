static CURLcode smtp_perform_upgrade_tls(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;

  /* Start the SSL connection */
  result = Curl_ssl_connect_nonblocking(conn, FIRSTSOCKET, &smtpc->ssldone);

  if(!result) {
    if(smtpc->state != SMTP_UPGRADETLS)
      state(conn, SMTP_UPGRADETLS);

    if(smtpc->ssldone) {
      smtp_to_smtps(conn);
      result = smtp_perform_ehlo(conn);
    }
  }

  return result;
}
