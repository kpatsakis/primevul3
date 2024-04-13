void DevToolsWindow::BeforeUnloadFired(content::WebContents* tab,
                                       bool proceed,
                                       bool* proceed_to_fire_unload) {
  if (!intercepted_page_beforeunload_) {
    if (proceed) {
      content::DevToolsManager::GetInstance()->ClientHostClosing(
          frontend_host_.get());
    }
    *proceed_to_fire_unload = proceed;
  } else {
    content::WebContents* inspected_web_contents = GetInspectedWebContents();
    if (proceed) {
      inspected_web_contents->GetRenderViewHost()->FirePageBeforeUnload(false);
    } else {
      bool should_proceed;
      inspected_web_contents->GetDelegate()->BeforeUnloadFired(
          inspected_web_contents, false, &should_proceed);
      DCHECK(!should_proceed);
    }
    *proceed_to_fire_unload = false;
  }
}
