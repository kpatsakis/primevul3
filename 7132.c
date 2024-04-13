void Curl_cookie_loadfiles(struct Curl_easy *data)
{
  struct curl_slist *list = data->state.cookielist;
  if(list) {
    Curl_share_lock(data, CURL_LOCK_DATA_COOKIE, CURL_LOCK_ACCESS_SINGLE);
    while(list) {
      struct CookieInfo *newcookies = Curl_cookie_init(data,
                                        list->data,
                                        data->cookies,
                                        data->set.cookiesession);
      if(!newcookies)
        /*
         * Failure may be due to OOM or a bad cookie; both are ignored
         * but only the first should be
         */
        infof(data, "ignoring failed cookie_init for %s", list->data);
      else
        data->cookies = newcookies;
      list = list->next;
    }
    curl_slist_free_all(data->state.cookielist); /* clean up list */
    data->state.cookielist = NULL; /* don't do this again! */
    Curl_share_unlock(data, CURL_LOCK_DATA_COOKIE);
  }
}