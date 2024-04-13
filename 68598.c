static CURLcode smtp_dophase_done(struct connectdata *conn, bool connected)
{
  struct SMTP *smtp = conn->data->req.protop;

  (void)connected;

  if(smtp->transfer != FTPTRANSFER_BODY)
    /* no data to transfer */
    Curl_setup_transfer(conn, -1, -1, FALSE, NULL, -1, NULL);

  return CURLE_OK;
}
