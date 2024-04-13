bool IsValidGaiaSigninRedirectOrResponseURL(const GURL& url) {
  std::string hostname = url.host();
  if (google_util::IsGoogleHostname(hostname, google_util::ALLOW_SUBDOMAIN)) {
    return gaia::IsGaiaSignonRealm(url.GetOrigin()) ||
        StartsWithASCII(hostname, "accounts.", false);
  }

  GURL origin = url.GetOrigin();
  if (origin == GURL("https://accounts.youtube.com") ||
      origin == GURL("https://accounts.blogger.com"))
    return true;

  return false;
}
