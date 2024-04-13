bool SafeBrowsingBlockingPage::IsMainPageLoadBlocked(
    const UnsafeResourceList& unsafe_resources) {
  if (unsafe_resources[0].threat_type ==
      SB_THREAT_TYPE_CLIENT_SIDE_PHISHING_URL) {
    return false;
  }

  return unsafe_resources.size() == 1 && !unsafe_resources[0].is_subresource;
}
