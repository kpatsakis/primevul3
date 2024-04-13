bool ExtensionService::IsBackgroundPageReady(const Extension* extension) {
  return (extension->background_url().is_empty() ||
          extension_runtime_data_[extension->id()].background_page_ready);
}
