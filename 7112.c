void Curl_flush_cookies(struct Curl_easy *data, bool cleanup)
{
  CURLcode res;

  if(data->set.str[STRING_COOKIEJAR]) {
    if(data->state.cookielist) {
      /* If there is a list of cookie files to read, do it first so that
         we have all the told files read before we write the new jar.
         Curl_cookie_loadfiles() LOCKS and UNLOCKS the share itself! */
      Curl_cookie_loadfiles(data);
    }

    Curl_share_lock(data, CURL_LOCK_DATA_COOKIE, CURL_LOCK_ACCESS_SINGLE);

    /* if we have a destination file for all the cookies to get dumped to */
    res = cookie_output(data, data->cookies, data->set.str[STRING_COOKIEJAR]);
    if(res)
      infof(data, "WARNING: failed to save cookies in %s: %s",
            data->set.str[STRING_COOKIEJAR], curl_easy_strerror(res));
  }
  else {
    if(cleanup && data->state.cookielist) {
      /* since nothing is written, we can just free the list of cookie file
         names */
      curl_slist_free_all(data->state.cookielist); /* clean up list */
      data->state.cookielist = NULL;
    }
    Curl_share_lock(data, CURL_LOCK_DATA_COOKIE, CURL_LOCK_ACCESS_SINGLE);
  }

  if(cleanup && (!data->share || (data->cookies != data->share->cookies))) {
    Curl_cookie_cleanup(data->cookies);
    data->cookies = NULL;
  }
  Curl_share_unlock(data, CURL_LOCK_DATA_COOKIE);
}