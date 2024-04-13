static CURLcode smtp_init(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp;

  smtp = data->req.protop = calloc(sizeof(struct SMTP), 1);
  if(!smtp)
    result = CURLE_OUT_OF_MEMORY;

  return result;
}
