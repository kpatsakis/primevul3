void ExtensionService::OnLoadSingleExtension(const Extension* extension,
                                             bool prompt_for_plugins) {
  if (show_extensions_prompts_ && prompt_for_plugins &&
      !extension->plugins().empty() &&
      disabled_extension_paths_.find(extension->id()) ==
          disabled_extension_paths_.end()) {
    SimpleExtensionLoadPrompt* prompt = new SimpleExtensionLoadPrompt(
        profile_, weak_ptr_factory_.GetWeakPtr(), extension);
    prompt->ShowPrompt();
    return;  // continues in SimpleExtensionLoadPrompt::InstallUI*
  }
  OnExtensionInstalled(extension, false, 0);  // Not from web store.
}
