bool BaseSettingChange::CanBeMerged() const {
  return !GetNewSettingURL().is_empty();
}
