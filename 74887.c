WebPlugin* RenderView::createPlugin(WebFrame* frame,
                                    const WebPluginParams& params) {
  bool found = false;
  WebPluginInfo info;
  GURL url(params.url);
  std::string actual_mime_type;
  Send(new ViewHostMsg_GetPluginInfo(url,
                                     frame->top()->url(),
                                     params.mimeType.utf8(),
                                     &found,
                                     &info,
                                     &actual_mime_type));

  if (!found)
    return NULL;

  scoped_ptr<PluginGroup> group(PluginGroup::FindHardcodedPluginGroup(info));
  group->AddPlugin(info, 0);

  if (!info.enabled) {
    if (CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kDisableOutdatedPlugins) &&
        group->IsVulnerable()) {
      Send(new ViewHostMsg_DisabledOutdatedPlugin(routing_id_,
                                                  group->GetGroupName(),
                                                  GURL(group->GetUpdateURL())));
      return CreatePluginPlaceholder(frame, params, group.get());
    }
    return NULL;
  }

  if (info.path.value() != kDefaultPluginLibraryName) {
    if (!AllowContentType(CONTENT_SETTINGS_TYPE_PLUGINS)) {
      DCHECK(CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableClickToPlay));
      didNotAllowPlugins(frame);
      return CreatePluginPlaceholder(frame, params, NULL);
    }
    scoped_refptr<pepper::PluginModule> pepper_module =
        PepperPluginRegistry::GetInstance()->GetModule(info.path);
    if (pepper_module)
      return CreatePepperPlugin(frame, params, info.path, pepper_module.get());
    if (CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kBlockNonSandboxedPlugins)) {
      Send(new ViewHostMsg_NonSandboxedPluginBlocked(routing_id_,
                                                     group->GetGroupName()));
      return CreatePluginPlaceholder(frame, params, NULL);
    }
  }
  return CreateNPAPIPlugin(frame, params, info.path, actual_mime_type);
}
