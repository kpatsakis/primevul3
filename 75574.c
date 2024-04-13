SimpleExtensionLoadPrompt::SimpleExtensionLoadPrompt(
    Profile* profile,
    base::WeakPtr<ExtensionService> extension_service,
    const Extension* extension)
    : extension_service_(extension_service),
      install_ui_(new ExtensionInstallUI(profile)),
      extension_(extension) {
}
