static CURLcode smtp_perform_starttls(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;

  /* Send the STARTTLS command */
  result = Curl_pp_sendf(&conn->proto.smtpc.pp, "%s", "STARTTLS");

  if(!result)
    state(conn, SMTP_STARTTLS);

  return result;
}
