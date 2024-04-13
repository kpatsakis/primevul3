void RedirectToNtpOrAppsPageWithIds(int child_id,
                                    int route_id,
                                    signin::Source source) {
  content::WebContents* web_contents = tab_util::GetWebContentsByID(child_id,
                                                                    route_id);
  if (!web_contents)
    return;

  RedirectToNtpOrAppsPage(web_contents, source);
}
