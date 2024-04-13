void OneClickSigninHelper::DidNavigateMainFrame(
    const content::LoadCommittedDetails& details,
    const content::FrameNavigateParams& params) {
  if (!SigninManager::IsWebBasedSigninFlowURL(params.url)) {
    Profile* profile =
        Profile::FromBrowserContext(web_contents()->GetBrowserContext());
    SigninManager* manager = profile ?
        SigninManagerFactory::GetForProfile(profile) : NULL;
    int process_id = web_contents()->GetRenderProcessHost()->GetID();
    if (manager && manager->IsSigninProcess(process_id))
      manager->ClearSigninProcess();

    if (!content::PageTransitionIsWebTriggerable(params.transition) &&
        auto_accept_ != AUTO_ACCEPT_NONE) {
      CleanTransientState();
    }
  }
}
