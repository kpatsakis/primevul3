void PluginObserver::OnBlockedOutdatedPlugin(const string16& name,
                                             const GURL& update_url) {
  tab_contents_->AddInfoBar(update_url.is_empty() ?
      static_cast<InfoBarDelegate*>(new BlockedPluginInfoBarDelegate(
          tab_contents(), name)) :
      new OutdatedPluginInfoBarDelegate(tab_contents(), name, update_url));
}
