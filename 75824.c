void PromptBrowserLoginFunction::OnLoginSuccess() {
  ProfileSyncService* service = GetSyncService(GetDefaultProfile(profile_));
  syncable::ModelTypeSet types;
  if (service->HasSyncSetupCompleted())
    service->GetPreferredDataTypes(&types);
  types.insert(syncable::APPS);
  service->ChangePreferredDataTypes(types);
  service->SetSyncSetupCompleted();

  waiting_for_token_ = true;
}
