void ExtensionService::UnloadAllExtensions() {
  profile_->GetExtensionSpecialStoragePolicy()->
      RevokeRightsForAllExtensions();

  extensions_.clear();
  disabled_extensions_.clear();
  terminated_extension_ids_.clear();
  terminated_extensions_.clear();
  extension_runtime_data_.clear();

}
