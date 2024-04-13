UninstalledExtensionInfo::UninstalledExtensionInfo(
    const Extension& extension)
    : extension_id(extension.id()),
      extension_api_permissions(extension.api_permissions()),
      extension_type(extension.GetType()),
      update_url(extension.update_url()) {}
