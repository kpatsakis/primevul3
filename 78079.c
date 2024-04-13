std::set<GURL> WebContentsImpl::GetSitesInTab() const {
  BrowserContext* browser_context = GetBrowserContext();
  std::set<GURL> sites;
  if (!frame_tree_root_.get())
    return sites;

  FrameTreeNode* node = NULL;
  std::queue<FrameTreeNode*> queue;
  queue.push(frame_tree_root_.get());

  while (!queue.empty()) {
    node = queue.front();
    queue.pop();
    sites.insert(SiteInstance::GetSiteForURL(browser_context,
                                             node->current_url()));

    for (size_t i = 0; i < node->child_count(); ++i)
      queue.push(node->child_at(i));
  }

  return sites;
}
