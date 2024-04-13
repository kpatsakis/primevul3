void OneClickSigninHelper::RemoveSigninRedirectURLHistoryItem(
    content::WebContents* web_contents) {
  if (signin::IsContinueUrlForWebBasedSigninFlow(
          web_contents->GetLastCommittedURL())) {
    new CurrentHistoryCleaner(web_contents);  // will self-destruct when done
  }
}
