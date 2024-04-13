bool Extension::HasEffectiveAccessToAllHosts() const {
  return HasEffectiveAccessToAllHosts(GetEffectiveHostPermissions(),
                                      api_permissions());
}
