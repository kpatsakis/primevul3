    ExtensionService::FindNaClModule(const GURL& url) {
  for (NaClModuleInfoList::iterator iter = nacl_module_list_.begin();
       iter != nacl_module_list_.end(); ++iter) {
    if (iter->url == url)
      return iter;
  }
  return nacl_module_list_.end();
}
