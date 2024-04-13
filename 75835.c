  SafeBeginInstallHelper(BeginInstallWithManifestFunction* client,
                         const std::string& icon_data,
                         const std::string& manifest)
      : client_(client),
        icon_data_(icon_data),
        manifest_(manifest),
        utility_host_(NULL),
        icon_decode_complete_(false),
        manifest_parse_complete_(false),
        parse_error_(BeginInstallWithManifestFunction::UNKNOWN_ERROR) {}
