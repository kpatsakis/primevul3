static CURLcode smtp_state_postdata_resp(struct connectdata *conn,
                                         int smtpcode,
                                         smtpstate instate)
{
  CURLcode result = CURLE_OK;

  (void)instate; /* no use for this yet */

  if(smtpcode != 250)
    result = CURLE_RECV_ERROR;

  /* End of DONE phase */
  state(conn, SMTP_STOP);

  return result;
}
