GURL DevToolsWindow::GetDevToolsURL(Profile* profile,
                                    const GURL& base_url,
                                    DevToolsDockSide dock_side,
                                    bool shared_worker_frontend,
                                    bool external_frontend) {
  if (base_url.SchemeIs("data"))
    return base_url;

  std::string frontend_url(
      base_url.is_empty() ? chrome::kChromeUIDevToolsURL : base_url.spec());
  ThemeService* tp = ThemeServiceFactory::GetForProfile(profile);
  DCHECK(tp);
  std::string url_string(
      frontend_url +
      ((frontend_url.find("?") == std::string::npos) ? "?" : "&") +
      "dockSide=" + SideToString(dock_side) +
      "&toolbarColor=" +
      SkColorToRGBAString(tp->GetColor(ThemeProperties::COLOR_TOOLBAR)) +
      "&textColor=" +
      SkColorToRGBAString(tp->GetColor(ThemeProperties::COLOR_BOOKMARK_TEXT)));
  if (shared_worker_frontend)
    url_string += "&isSharedWorker=true";
  if (external_frontend)
    url_string += "&remoteFrontend=true";
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableDevToolsExperiments))
    url_string += "&experiments=true";
  url_string += "&updateAppcache";
  return GURL(url_string);
}
