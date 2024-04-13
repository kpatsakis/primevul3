void ExtensionService::NotifyExtensionUnloaded(
    const Extension* extension,
    extension_misc::UnloadedExtensionReason reason) {
  UnloadedExtensionInfo details(extension, reason);
  NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSION_UNLOADED,
      Source<Profile>(profile_),
      Details<UnloadedExtensionInfo>(&details));

  for (RenderProcessHost::iterator i(RenderProcessHost::AllHostsIterator());
       !i.IsAtEnd(); i.Advance()) {
    RenderProcessHost* host = i.GetCurrentValue();
    Profile* host_profile =
        Profile::FromBrowserContext(host->browser_context());
    if (host_profile->GetOriginalProfile() == profile_->GetOriginalProfile())
      host->Send(new ExtensionMsg_Unloaded(extension->id()));
  }

  profile_->UnregisterExtensionWithRequestContexts(extension->id(), reason);
  profile_->GetExtensionSpecialStoragePolicy()->
      RevokeRightsForExtension(extension);

  ExtensionWebUI::UnregisterChromeURLOverrides(
      profile_, extension->GetChromeURLOverrides());

#if defined(OS_CHROMEOS)
  if (profile_->GetFileSystemContext() &&
      profile_->GetFileSystemContext()->path_manager() &&
      profile_->GetFileSystemContext()->path_manager()->external_provider()) {
    profile_->GetFileSystemContext()->path_manager()->external_provider()->
        RevokeAccessForExtension(extension->id());
  }
#endif

  UpdateActiveExtensionsInCrashReporter();

  bool plugins_changed = false;
  for (size_t i = 0; i < extension->plugins().size(); ++i) {
    const Extension::PluginInfo& plugin = extension->plugins()[i];
    if (!BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                                 NewRunnableFunction(&ForceShutdownPlugin,
                                                     plugin.path)))
      NOTREACHED();
    webkit::npapi::PluginList::Singleton()->RefreshPlugins();
    webkit::npapi::PluginList::Singleton()->RemoveExtraPluginPath(
        plugin.path);
    plugins_changed = true;
    if (!plugin.is_public)
      PluginService::GetInstance()->RestrictPluginToUrl(plugin.path, GURL());
  }

  bool nacl_modules_changed = false;
  for (size_t i = 0; i < extension->nacl_modules().size(); ++i) {
    const Extension::NaClModuleInfo& module = extension->nacl_modules()[i];
    UnregisterNaClModule(module.url);
    nacl_modules_changed = true;
  }

  if (nacl_modules_changed)
    UpdatePluginListWithNaClModules();

  if (plugins_changed || nacl_modules_changed)
    PluginService::GetInstance()->PurgePluginListCache(false);
}
