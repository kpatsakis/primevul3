bool SafeBrowsingBlockingPage::CanShowMalwareDetailsOption() {
  return (!web_contents_->GetBrowserContext()->IsOffTheRecord() &&
          web_contents_->GetURL().SchemeIs(chrome::kHttpScheme));
}
