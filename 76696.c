CompositeSettingsChange* BaseSettingChange::MergeWith(
    BaseSettingChange* other) {
  CompositeSettingsChange* composite_change = new CompositeSettingsChange();
  CHECK(composite_change->Init(profile_));
  composite_change->MergeWith(this);
  composite_change->MergeWith(other);
  return composite_change;
}
