void ExtensionService::NotifyExtensionLoaded(const Extension* extension) {
  profile_->RegisterExtensionWithRequestContexts(extension);

  NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSION_LOADED,
      Source<Profile>(profile_),
      Details<const Extension>(extension));

  for (RenderProcessHost::iterator i(RenderProcessHost::AllHostsIterator());
       !i.IsAtEnd(); i.Advance()) {
    RenderProcessHost* host = i.GetCurrentValue();
    Profile* host_profile =
        Profile::FromBrowserContext(host->browser_context());
    if (host_profile->GetOriginalProfile() == profile_->GetOriginalProfile()) {
      host->Send(
          new ExtensionMsg_Loaded(ExtensionMsg_Loaded_Params(extension)));
    }
  }


  profile_->GetExtensionSpecialStoragePolicy()->
      GrantRightsForExtension(extension);

  UpdateActiveExtensionsInCrashReporter();

  ExtensionWebUI::RegisterChromeURLOverrides(
      profile_, extension->GetChromeURLOverrides());

  TemplateURLService* url_service =
      TemplateURLServiceFactory::GetForProfile(profile_);
  if (url_service)
    url_service->RegisterExtensionKeyword(extension);

  if (!extension->omnibox_keyword().empty()) {
    omnibox_popup_icon_manager_.LoadIcon(extension);
    omnibox_icon_manager_.LoadIcon(extension);
  }

  if (extension->HasHostPermission(GURL(chrome::kChromeUIFaviconURL))) {
    FaviconSource* favicon_source = new FaviconSource(profile_,
                                                      FaviconSource::FAVICON);
    profile_->GetChromeURLDataManager()->AddDataSource(favicon_source);
  }

  bool plugins_changed = false;
  for (size_t i = 0; i < extension->plugins().size(); ++i) {
    const Extension::PluginInfo& plugin = extension->plugins()[i];
    webkit::npapi::PluginList::Singleton()->RefreshPlugins();
    webkit::npapi::PluginList::Singleton()->AddExtraPluginPath(plugin.path);
    plugins_changed = true;
    if (!plugin.is_public) {
      PluginService::GetInstance()->RestrictPluginToUrl(
          plugin.path, extension->url());
    }
  }

  bool nacl_modules_changed = false;
  for (size_t i = 0; i < extension->nacl_modules().size(); ++i) {
    const Extension::NaClModuleInfo& module = extension->nacl_modules()[i];
    RegisterNaClModule(module.url, module.mime_type);
    nacl_modules_changed = true;
  }

  if (nacl_modules_changed)
    UpdatePluginListWithNaClModules();

  if (plugins_changed || nacl_modules_changed)
    PluginService::GetInstance()->PurgePluginListCache(false);

#if defined(OS_CHROMEOS)
#if defined(TOUCH_UI)
  chromeos::input_method::InputMethodManager* input_method_manager =
      chromeos::input_method::InputMethodManager::GetInstance();
#endif
  for (std::vector<Extension::InputComponentInfo>::const_iterator component =
           extension->input_components().begin();
       component != extension->input_components().end();
       ++component) {
    if (component->type == Extension::INPUT_COMPONENT_TYPE_IME) {
      ExtensionInputImeEventRouter::GetInstance()->RegisterIme(
          profile_, extension->id(), *component);
    }
#if defined(TOUCH_UI)
    if (component->type == Extension::INPUT_COMPONENT_TYPE_VIRTUAL_KEYBOARD &&
        !component->layouts.empty()) {
      const bool is_system =
          !Extension::IsExternalLocation(extension->location());
      input_method_manager->RegisterVirtualKeyboard(
          extension->url(),
          component->name,  // human-readable name of the keyboard extension.
          component->layouts,
          is_system);
    }
#endif
  }
#endif
}
