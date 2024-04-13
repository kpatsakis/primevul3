void ExtensionService::UpdatePluginListWithNaClModules() {
  const PepperPluginInfo* pepper_info = NULL;
  std::vector<PepperPluginInfo> plugins;
  PepperPluginRegistry::ComputeList(&plugins);

  for (size_t i = 0; i < plugins.size(); ++i) {
    pepper_info = &plugins[i];
    CHECK(pepper_info);
    std::vector<webkit::WebPluginMimeType>::const_iterator mime_iter;
    for (mime_iter = pepper_info->mime_types.begin();
         mime_iter != pepper_info->mime_types.end(); ++mime_iter) {
      if (mime_iter->mime_type == kNaClPluginMimeType) {

        webkit::npapi::PluginList::Singleton()->
            UnregisterInternalPlugin(pepper_info->path);

        webkit::WebPluginInfo info = pepper_info->ToWebPluginInfo();

        for (ExtensionService::NaClModuleInfoList::const_iterator iter =
            nacl_module_list_.begin();
            iter != nacl_module_list_.end(); ++iter) {
          webkit::WebPluginMimeType mime_type_info;
          mime_type_info.mime_type = iter->mime_type;
          mime_type_info.additional_param_names.push_back(UTF8ToUTF16("nacl"));
          mime_type_info.additional_param_values.push_back(
              UTF8ToUTF16(iter->url.spec()));
          info.mime_types.push_back(mime_type_info);
        }

        webkit::npapi::PluginList::Singleton()->RefreshPlugins();
        webkit::npapi::PluginList::Singleton()->RegisterInternalPlugin(info);
        break;
      }
    }
  }
}
