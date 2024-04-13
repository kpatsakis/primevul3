void StartExplicitSync(const StartSyncArgs& args,
                       content::WebContents* contents,
                       OneClickSigninSyncStarter::StartSyncMode start_mode,
                       ConfirmEmailDialogDelegate::Action action) {
  if (action == ConfirmEmailDialogDelegate::START_SYNC) {
    StartSync(args, start_mode);
    RedirectToNtpOrAppsPageIfNecessary(contents, args.source);
  } else {
    if (signin::IsContinueUrlForWebBasedSigninFlow(
            contents->GetVisibleURL())) {
      base::MessageLoopProxy::current()->PostNonNestableTask(
          FROM_HERE,
          base::Bind(RedirectToNtpOrAppsPageWithIds,
                     contents->GetRenderProcessHost()->GetID(),
                     contents->GetRoutingID(),
                     args.source));
    }
    if (action == ConfirmEmailDialogDelegate::CREATE_NEW_USER) {
      chrome::ShowSettingsSubPage(args.browser,
                                  std::string(chrome::kSearchUsersSubPage));
    }
  }
}
