static CURLcode smtp_continue_auth(struct connectdata *conn, const char *resp)
{
  struct smtp_conn *smtpc = &conn->proto.smtpc;

  return Curl_pp_sendf(&smtpc->pp, "%s", resp);
}
