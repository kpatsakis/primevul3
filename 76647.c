  ~SecurityState() {
    scheme_policy_.clear();
    fileapi::IsolatedContext* isolated_context =
        fileapi::IsolatedContext::GetInstance();
    for (FileSystemMap::iterator iter = filesystem_permissions_.begin();
         iter != filesystem_permissions_.end();
         ++iter) {
      isolated_context->RemoveReference(iter->first);
    }
    UMA_HISTOGRAM_COUNTS("ChildProcessSecurityPolicy.PerChildFilePermissions",
                         file_permissions_.size());
  }
