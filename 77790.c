void OneClickSigninHelper::SyncSetupCompletedCallback(
    OneClickSigninSyncStarter::SyncSetupResult result) {
  if (result == OneClickSigninSyncStarter::SYNC_SETUP_FAILURE &&
      web_contents()) {
    GURL current_url = web_contents()->GetVisibleURL();

    if (signin::IsContinueUrlForWebBasedSigninFlow(current_url) &&
        !signin::IsAutoCloseEnabledInURL(original_continue_url_)) {
      RedirectToNtpOrAppsPage(
          web_contents(),
          signin::GetSourceForPromoURL(original_continue_url_));
    }
  }
}
