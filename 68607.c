static CURLcode smtp_perform_auth(struct connectdata *conn,
                                  const char *mech,
                                  const char *initresp)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;

  if(initresp) {                                  /* AUTH <mech> ...<crlf> */
    /* Send the AUTH command with the initial response */
    result = Curl_pp_sendf(&smtpc->pp, "AUTH %s %s", mech, initresp);
  }
  else {
    /* Send the AUTH command */
    result = Curl_pp_sendf(&smtpc->pp, "AUTH %s", mech);
  }

  return result;
}
