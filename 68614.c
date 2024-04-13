static CURLcode smtp_perform_rcpt_to(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;

  /* Send the RCPT TO command */
  if(smtp->rcpt->data[0] == '<')
    result = Curl_pp_sendf(&conn->proto.smtpc.pp, "RCPT TO:%s",
                           smtp->rcpt->data);
  else
    result = Curl_pp_sendf(&conn->proto.smtpc.pp, "RCPT TO:<%s>",
                           smtp->rcpt->data);
  if(!result)
    state(conn, SMTP_RCPT);

  return result;
}
