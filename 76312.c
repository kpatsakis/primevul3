void NetworkScreen::OnHelpLinkActivated() {
  ClearErrors();
  if (!help_app_.get())
    help_app_.reset(new HelpAppLauncher(view()->GetNativeWindow()));
  help_app_->ShowHelpTopic(HelpAppLauncher::HELP_CONNECTIVITY);
}
