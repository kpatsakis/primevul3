static size_t cookiehash(const char * const domain)
{
  const char *top;
  size_t len;

  if(!domain || Curl_host_is_ipnum(domain))
    return 0;

  top = get_top_domain(domain, &len);
  return cookie_hash_domain(top, len);
}