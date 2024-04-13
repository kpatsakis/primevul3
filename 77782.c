void OneClickSigninHelper::RedirectToSignin() {
  VLOG(1) << "OneClickSigninHelper::RedirectToSignin";

  signin::Source source = signin::GetSourceForPromoURL(continue_url_);
  if (source == signin::SOURCE_UNKNOWN)
    source = signin::SOURCE_MENU;
  GURL page = signin::GetPromoURL(source, false);

  content::WebContents* contents = web_contents();
  contents->GetController().LoadURL(page,
                                    content::Referrer(),
                                    content::PAGE_TRANSITION_AUTO_TOPLEVEL,
                                    std::string());
}
