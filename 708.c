static CURLUcode hostname_check(struct Curl_URL *u, char *hostname)
{
  size_t len;
  size_t hlen = strlen(hostname);

  if(hostname[0] == '[') {
    const char *l = "0123456789abcdefABCDEF:.";
    if(hlen < 4) /* '[::]' is the shortest possible valid string */
      return CURLUE_BAD_IPV6;
    hostname++;
    hlen -= 2;

    if(hostname[hlen] != ']')
      return CURLUE_BAD_IPV6;

    /* only valid letters are ok */
    len = strspn(hostname, l);
    if(hlen != len) {
      hlen = len;
      if(hostname[len] == '%') {
        /* this could now be '%[zone id]' */
        char zoneid[16];
        int i = 0;
        char *h = &hostname[len + 1];
        /* pass '25' if present and is a url encoded percent sign */
        if(!strncmp(h, "25", 2) && h[2] && (h[2] != ']'))
          h += 2;
        while(*h && (*h != ']') && (i < 15))
          zoneid[i++] = *h++;
        if(!i || (']' != *h))
          /* impossible to reach? */
          return CURLUE_MALFORMED_INPUT;
        zoneid[i] = 0;
        u->zoneid = strdup(zoneid);
        if(!u->zoneid)
          return CURLUE_OUT_OF_MEMORY;
        hostname[len] = ']'; /* insert end bracket */
        hostname[len + 1] = 0; /* terminate the hostname */
      }
      else
        return CURLUE_BAD_IPV6;
      /* hostname is fine */
    }
#ifdef ENABLE_IPV6
    {
      char dest[16]; /* fits a binary IPv6 address */
      char norm[MAX_IPADR_LEN];
      hostname[hlen] = 0; /* end the address there */
      if(1 != Curl_inet_pton(AF_INET6, hostname, dest))
        return CURLUE_BAD_IPV6;

      /* check if it can be done shorter */
      if(Curl_inet_ntop(AF_INET6, dest, norm, sizeof(norm)) &&
         (strlen(norm) < hlen)) {
        strcpy(hostname, norm);
        hlen = strlen(norm);
        hostname[hlen + 1] = 0;
      }
      hostname[hlen] = ']'; /* restore ending bracket */
    }
#endif
  }
  else {
    /* letters from the second string is not ok */
    len = strcspn(hostname, " \r\n");
    if(hlen != len)
      /* hostname with bad content */
      return CURLUE_BAD_HOSTNAME;
  }
  if(!hostname[0])
    return CURLUE_NO_HOST;
  return CURLUE_OK;
}