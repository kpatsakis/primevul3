  const Extension* GetExtension() {
    const ExtensionSet* extensions = extensions::ExtensionSystem::Get(
        browser()->profile())->extension_service()->extensions();
    for (ExtensionSet::const_iterator it = extensions->begin();
         it != extensions->end(); ++it) {
      if ((*it)->name() == "App Test")
        return it->get();
    }
    NOTREACHED();
    return NULL;
  }
