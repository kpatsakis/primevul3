PluginInfoMessageFilter::PluginInfoMessageFilter(
    int render_process_id,
    Profile* profile)
    : context_(render_process_id, profile),
      weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {
}
