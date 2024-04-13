ManifestReloadReason ShouldReloadExtensionManifest(const ExtensionInfo& info) {
  if (info.extension_location == Extension::LOAD)
    return UNPACKED_DIR;

  if (extension_l10n_util::ShouldRelocalizeManifest(info))
    return NEEDS_RELOCALIZATION;

  return NOT_NEEDED;
}
