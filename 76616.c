  bool CanSendCookiesForOrigin(const GURL& gurl) {
    const CommandLine& command_line = *CommandLine::ForCurrentProcess();
    if (!command_line.HasSwitch(switches::kEnableStrictSiteIsolation))
      return true;

    if (origin_lock_.is_empty())
      return true;
    GURL site_gurl = SiteInstanceImpl::GetSiteForURL(NULL, gurl);
    return origin_lock_ == site_gurl;
  }
