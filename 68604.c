static CURLcode smtp_parse_custom_request(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;
  const char *custom = data->set.str[STRING_CUSTOMREQUEST];

  /* URL decode the custom request */
  if(custom)
    result = Curl_urldecode(data, custom, 0, &smtp->custom, NULL, TRUE);

  return result;
}
