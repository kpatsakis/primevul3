void RedirectToNtpOrAppsPage(content::WebContents* contents,
                             signin::Source source) {
  VLOG(1) << "RedirectToNtpOrAppsPage";
  GURL url(source == signin::SOURCE_APPS_PAGE_LINK ?
           chrome::kChromeUIAppsURL : chrome::kChromeUINewTabURL);
  content::OpenURLParams params(url,
                                content::Referrer(),
                                CURRENT_TAB,
                                content::PAGE_TRANSITION_AUTO_TOPLEVEL,
                                false);
  contents->OpenURL(params);
}
