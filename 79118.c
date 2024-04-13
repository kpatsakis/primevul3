  bool LoadExtensionFromPath(const base::FilePath& path) {
    ExtensionService* service = extensions::ExtensionSystem::Get(
        browser()->profile())->extension_service();
    size_t num_before = service->extensions()->size();
    {
      content::NotificationRegistrar registrar;
      registrar.Add(this, chrome::NOTIFICATION_EXTENSION_LOADED,
                    content::NotificationService::AllSources());
      base::CancelableClosure timeout(
          base::Bind(&TimeoutCallback, "Extension load timed out."));
      base::MessageLoop::current()->PostDelayedTask(
          FROM_HERE, timeout.callback(), TestTimeouts::action_timeout());
      extensions::UnpackedInstaller::Create(service)->Load(path);
      content::RunMessageLoop();
      timeout.Cancel();
    }
    size_t num_after = service->extensions()->size();
    if (num_after != (num_before + 1))
      return false;

    return WaitForExtensionViewsToLoad();
  }
