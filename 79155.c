DevToolsWindow::DevToolsWindow(Profile* profile,
                               const GURL& url,
                               content::RenderViewHost* inspected_rvh,
                               DevToolsDockSide dock_side)
    : profile_(profile),
      browser_(NULL),
      dock_side_(dock_side),
      is_loaded_(false),
      action_on_load_(DevToolsToggleAction::Show()),
      width_(-1),
      height_(-1),
      dock_side_before_minimized_(dock_side),
      intercepted_page_beforeunload_(false),
      weak_factory_(this) {
  web_contents_ =
      content::WebContents::Create(content::WebContents::CreateParams(profile));
  frontend_contents_observer_.reset(new FrontendWebContentsObserver(this));

  web_contents_->GetController().LoadURL(url, content::Referrer(),
      content::PAGE_TRANSITION_AUTO_TOPLEVEL, std::string());

  frontend_host_.reset(content::DevToolsClientHost::CreateDevToolsFrontendHost(
      web_contents_, this));
  file_helper_.reset(new DevToolsFileHelper(web_contents_, profile));
  file_system_indexer_ = new DevToolsFileSystemIndexer();
  extensions::ExtensionWebContentsObserver::CreateForWebContents(web_contents_);

  g_instances.Get().push_back(this);

  content::NavigationEntry* entry =
      web_contents_->GetController().GetActiveEntry();
  entry->GetFavicon().image = gfx::Image();
  entry->GetFavicon().valid = true;

  registrar_.Add(
      this, chrome::NOTIFICATION_BROWSER_THEME_CHANGED,
      content::Source<ThemeService>(
          ThemeServiceFactory::GetForProfile(profile_)));

  if (inspected_rvh)
    inspected_contents_observer_.reset(new InspectedWebContentsObserver(
        content::WebContents::FromRenderViewHost(inspected_rvh)));

  embedder_message_dispatcher_.reset(
      new DevToolsEmbedderMessageDispatcher(this));
}
