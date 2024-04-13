void PluginInfoMessageFilter::PluginsLoaded(
    const GetPluginInfo_Params& params,
    IPC::Message* reply_msg,
    const std::vector<WebPluginInfo>& plugins) {
  ChromeViewHostMsg_GetPluginInfo_Output output;
  if (!context_.FindEnabledPlugin(params.render_view_id, params.url,
                                  params.top_origin_url, params.mime_type,
                                  &output.status, &output.plugin,
                                  &output.actual_mime_type)) {
    ChromeViewHostMsg_GetPluginInfo::WriteReplyParams(reply_msg, output);
    Send(reply_msg);
    return;
  }

  PluginFinder::Get(base::Bind(&PluginInfoMessageFilter::GotPluginFinder, this,
                               params, reply_msg, output));
}
