void PluginInfoMessageFilter::OnDestruct() const {
  const_cast<PluginInfoMessageFilter*>(this)->
      weak_ptr_factory_.DetachFromThread();
  const_cast<PluginInfoMessageFilter*>(this)->
      weak_ptr_factory_.InvalidateWeakPtrs();

  content::BrowserThread::DeleteOnUIThread::Destruct(this);
}
