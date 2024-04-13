static CURLcode smtp_do(struct connectdata *conn, bool *done)
{
  CURLcode result = CURLE_OK;

  *done = FALSE; /* default to false */

  /* Parse the custom request */
  result = smtp_parse_custom_request(conn);
  if(result)
    return result;

  result = smtp_regular_transfer(conn, done);

  return result;
}
