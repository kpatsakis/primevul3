struct curl_slist *Curl_cookie_list(struct Curl_easy *data)
{
  struct curl_slist *list;
  Curl_share_lock(data, CURL_LOCK_DATA_COOKIE, CURL_LOCK_ACCESS_SINGLE);
  list = cookie_list(data);
  Curl_share_unlock(data, CURL_LOCK_DATA_COOKIE);
  return list;
}