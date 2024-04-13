void PluginInfoMessageFilter::GotPluginFinder(
    const GetPluginInfo_Params& params,
    IPC::Message* reply_msg,
    ChromeViewHostMsg_GetPluginInfo_Output output,
    PluginFinder* plugin_finder) {
  context_.DecidePluginStatus(params, output.plugin, plugin_finder,
                              &output.status, &output.group_identifier,
                              &output.group_name);
  ChromeViewHostMsg_GetPluginInfo::WriteReplyParams(reply_msg, output);
  Send(reply_msg);
}
