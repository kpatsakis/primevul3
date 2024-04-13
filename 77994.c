BrowserPluginGuest* WebContentsImpl::CreateGuest(
    BrowserContext* browser_context,
    SiteInstance* site_instance,
    int guest_instance_id,
    scoped_ptr<base::DictionaryValue> extra_params) {
  WebContentsImpl* new_contents = new WebContentsImpl(browser_context, NULL);

  BrowserPluginGuest::Create(
      guest_instance_id, new_contents, extra_params.Pass());

  WebContents::CreateParams create_params(browser_context, site_instance);
  new_contents->Init(create_params);

  static_cast<RenderViewHostImpl*>(
      new_contents->GetRenderViewHost())->set_is_subframe(true);

  return new_contents->browser_plugin_guest_.get();
}
