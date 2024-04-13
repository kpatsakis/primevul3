void InterstitialPage::DomOperationResponse(const std::string& json_string,
                                            int automation_id) {
  if (enabled_)
    CommandReceived(json_string);
}
