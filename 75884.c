  ExtensionConfig() {
    for (size_t i = 0; i < Extension::kNumPermissions; ++i)
      permission_map_[Extension::kPermissions[i].name] = i;
  };
