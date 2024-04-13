void Curl_cookie_cleanup(struct CookieInfo *c)
{
  if(c) {
    unsigned int i;
    free(c->filename);
    for(i = 0; i < COOKIE_HASH_SIZE; i++)
      Curl_cookie_freelist(c->cookies[i]);
    free(c); /* free the base struct as well */
  }
}