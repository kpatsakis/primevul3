void InterstitialPage::Proceed() {
  if (action_taken_ != NO_ACTION) {
    NOTREACHED();
    return;
  }
  Disable();
  action_taken_ = PROCEED_ACTION;

  tab_->SetIsLoading(true, NULL);

  if (new_navigation_)
    TakeActionOnResourceDispatcher(CANCEL);
  else
    TakeActionOnResourceDispatcher(RESUME);

  if (!new_navigation_) {
    Hide();
  }
}
