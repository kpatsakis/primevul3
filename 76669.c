void PpapiPluginProcessHost::DidCreateOutOfProcessInstance(
    int plugin_process_id,
    int32 pp_instance,
    int render_process_id,
    int render_view_id) {
  for (PpapiPluginProcessHostIterator iter; !iter.Done(); ++iter) {
    if (iter->process_.get() &&
        iter->process_->GetData().id == plugin_process_id) {
      iter->host_impl_->AddInstanceForView(pp_instance,
                                           render_process_id, render_view_id);
      return;
    }
  }
  DCHECK(plugin_process_id == 0)
      << "Renderer sent a bad plugin process host ID";
}
