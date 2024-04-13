WebStorageNamespace* RenderView::createSessionStorageNamespace(unsigned quota) {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kSingleProcess))
    return WebStorageNamespace::createSessionStorageNamespace(quota);
  CHECK(session_storage_namespace_id_ != kInvalidSessionStorageNamespaceId);
  return new RendererWebStorageNamespaceImpl(DOM_STORAGE_SESSION,
                                             session_storage_namespace_id_);
}
