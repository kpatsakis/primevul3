void Curl_cookie_clearall(struct CookieInfo *cookies)
{
  if(cookies) {
    unsigned int i;
    for(i = 0; i < COOKIE_HASH_SIZE; i++) {
      Curl_cookie_freelist(cookies->cookies[i]);
      cookies->cookies[i] = NULL;
    }
    cookies->numcookies = 0;
  }
}