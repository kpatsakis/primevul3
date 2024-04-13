ExtensionService::ExtensionService(Profile* profile,
                                   const CommandLine* command_line,
                                   const FilePath& install_directory,
                                   ExtensionPrefs* extension_prefs,
                                   ExtensionSettings* extension_settings,
                                   bool autoupdate_enabled,
                                   bool extensions_enabled)
    : weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      method_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      profile_(profile),
      extension_prefs_(extension_prefs),
      extension_settings_(extension_settings),
      pending_extension_manager_(*ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      install_directory_(install_directory),
      extensions_enabled_(extensions_enabled),
      show_extensions_prompts_(true),
      ready_(false),
      toolbar_model_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      permissions_manager_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      apps_promo_(profile->GetPrefs()),
      event_routers_initialized_(false) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (command_line->HasSwitch(switches::kDisableExtensions)) {
    extensions_enabled_ = false;
  } else if (profile->GetPrefs()->GetBoolean(prefs::kDisableExtensions)) {
    extensions_enabled_ = false;
  }

  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_PROCESS_TERMINATED,
                 NotificationService::AllSources());
  registrar_.Add(this, content::NOTIFICATION_RENDERER_PROCESS_CREATED,
                 NotificationService::AllSources());
  registrar_.Add(this, content::NOTIFICATION_RENDERER_PROCESS_TERMINATED,
                 NotificationService::AllSources());
  pref_change_registrar_.Init(profile->GetPrefs());
  pref_change_registrar_.Add(prefs::kExtensionInstallAllowList, this);
  pref_change_registrar_.Add(prefs::kExtensionInstallDenyList, this);

  if (autoupdate_enabled) {
    int update_frequency = kDefaultUpdateFrequencySeconds;
    if (command_line->HasSwitch(switches::kExtensionsUpdateFrequency)) {
      base::StringToInt(command_line->GetSwitchValueASCII(
          switches::kExtensionsUpdateFrequency),
          &update_frequency);
    }
    updater_.reset(new ExtensionUpdater(this,
                                        extension_prefs,
                                        profile->GetPrefs(),
                                        profile,
                                        update_frequency));
  }

  backend_ =
      new ExtensionServiceBackend(weak_ptr_factory_.GetWeakPtr(),
                                  install_directory_);

  if (extensions_enabled_) {
    ExternalExtensionProviderImpl::CreateExternalProviders(
        this, profile_, &external_extension_providers_);
  }

  omnibox_popup_icon_manager_.set_monochrome(true);
  omnibox_icon_manager_.set_monochrome(true);
  omnibox_icon_manager_.set_padding(gfx::Insets(0, kOmniboxIconPaddingLeft,
                                                0, kOmniboxIconPaddingRight));

  UMA_HISTOGRAM_CUSTOM_COUNTS("Extensions.ExtensionRootPathLength",
                              install_directory_.value().length(), 0, 500, 100);
}
