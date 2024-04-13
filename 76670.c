void PpapiPluginProcessHost::DidDeleteOutOfProcessInstance(
    int plugin_process_id,
    int32 pp_instance) {
  for (PpapiPluginProcessHostIterator iter; !iter.Done(); ++iter) {
    if (iter->process_.get() &&
        iter->process_->GetData().id == plugin_process_id) {
      iter->host_impl_->DeleteInstanceForView(pp_instance);
      return;
    }
  }
}
