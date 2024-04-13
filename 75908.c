WebPlugin* ChromeContentRendererClient::CreatePlugin(
      RenderView* render_view,
      WebFrame* frame,
      const WebPluginParams& original_params) {
  bool found = false;
  CommandLine* cmd = CommandLine::ForCurrentProcess();
  webkit::npapi::WebPluginInfo info;
  GURL url(original_params.url);
  std::string actual_mime_type;
  render_view->Send(new ViewHostMsg_GetPluginInfo(
      render_view->routing_id(), url, frame->top()->document().url(),
      original_params.mimeType.utf8(), &found, &info, &actual_mime_type));

  if (!found)
    return NULL;
  if (!webkit::npapi::IsPluginEnabled(info))
    return NULL;

  const webkit::npapi::PluginGroup* group =
      webkit::npapi::PluginList::Singleton()->GetPluginGroup(info);
  DCHECK(group != NULL);

  ContentSetting plugin_setting = CONTENT_SETTING_DEFAULT;
  std::string resource;
  if (cmd->HasSwitch(switches::kEnableResourceContentSettings))
    resource = group->identifier();
  render_view->Send(new ViewHostMsg_GetPluginContentSetting(
      frame->top()->document().url(), resource, &plugin_setting));
  DCHECK(plugin_setting != CONTENT_SETTING_DEFAULT);

  WebPluginParams params(original_params);
  for (size_t i = 0; i < info.mime_types.size(); ++i) {
    if (info.mime_types[i].mime_type == actual_mime_type) {
      AppendParams(info.mime_types[i].additional_param_names,
                   info.mime_types[i].additional_param_values,
                   &params.attributeNames,
                   &params.attributeValues);
      break;
    }
  }

  ContentSetting outdated_policy = CONTENT_SETTING_ASK;
  ContentSetting authorize_policy = CONTENT_SETTING_ASK;
  if (group->IsVulnerable() || group->RequiresAuthorization()) {
    render_view->Send(new ViewHostMsg_GetPluginPolicies(
        &outdated_policy, &authorize_policy));
  }

  if (group->IsVulnerable()) {
    if (outdated_policy == CONTENT_SETTING_ASK ||
        outdated_policy == CONTENT_SETTING_BLOCK) {
      if (outdated_policy == CONTENT_SETTING_ASK) {
        render_view->Send(new ViewHostMsg_BlockedOutdatedPlugin(
            render_view->routing_id(), group->GetGroupName(),
            GURL(group->GetUpdateURL())));
      }
      return CreatePluginPlaceholder(
          render_view, frame, params, *group, IDR_BLOCKED_PLUGIN_HTML,
          IDS_PLUGIN_OUTDATED, false, outdated_policy == CONTENT_SETTING_ASK);
    } else {
      DCHECK(outdated_policy == CONTENT_SETTING_ALLOW);
    }
  }

  ContentSettingsObserver* observer = ContentSettingsObserver::Get(render_view);
  ContentSetting host_setting =
      observer->GetContentSetting(CONTENT_SETTINGS_TYPE_PLUGINS);

  if (group->RequiresAuthorization() &&
      authorize_policy == CONTENT_SETTING_ASK &&
      (plugin_setting == CONTENT_SETTING_ALLOW ||
       plugin_setting == CONTENT_SETTING_ASK) &&
      host_setting == CONTENT_SETTING_DEFAULT) {
    render_view->Send(new ViewHostMsg_BlockedOutdatedPlugin(
        render_view->routing_id(), group->GetGroupName(), GURL()));
    return CreatePluginPlaceholder(
        render_view, frame, params, *group, IDR_BLOCKED_PLUGIN_HTML,
        IDS_PLUGIN_NOT_AUTHORIZED, false, true);
  }

  if (info.path.value() == webkit::npapi::kDefaultPluginLibraryName ||
      plugin_setting == CONTENT_SETTING_ALLOW ||
      host_setting == CONTENT_SETTING_ALLOW) {
    if (prerender::PrerenderHelper::IsPrerendering(render_view)) {
      return CreatePluginPlaceholder(
          render_view, frame, params, *group, IDR_CLICK_TO_PLAY_PLUGIN_HTML,
          IDS_PLUGIN_LOAD, true, true);
    }

    bool pepper_plugin_was_registered = false;
    scoped_refptr<webkit::ppapi::PluginModule> pepper_module(
        render_view->pepper_delegate()->CreatePepperPlugin(
            info.path, &pepper_plugin_was_registered));
    if (pepper_plugin_was_registered) {
      if (pepper_module) {
        return render_view->CreatePepperPlugin(
            frame, params, info.path, pepper_module.get());
      }
      return NULL;
    }

    return render_view->CreateNPAPIPlugin(
        frame, params, info.path, actual_mime_type);
  }

  observer->DidBlockContentType(CONTENT_SETTINGS_TYPE_PLUGINS, resource);
  if (plugin_setting == CONTENT_SETTING_ASK) {
    return CreatePluginPlaceholder(
        render_view, frame, params, *group, IDR_CLICK_TO_PLAY_PLUGIN_HTML,
        IDS_PLUGIN_LOAD, false, true);
  } else {
    return CreatePluginPlaceholder(
        render_view, frame, params, *group, IDR_BLOCKED_PLUGIN_HTML,
        IDS_PLUGIN_BLOCKED, false, true);
  }
}
