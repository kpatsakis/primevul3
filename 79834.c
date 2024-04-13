SafeBrowsingBlockingPageV1::SafeBrowsingBlockingPageV1(
    SafeBrowsingUIManager* ui_manager,
    WebContents* web_contents,
    const UnsafeResourceList& unsafe_resources)
  : SafeBrowsingBlockingPage(ui_manager, web_contents, unsafe_resources) {
}
