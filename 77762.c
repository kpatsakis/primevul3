    OneClickSigninHelper::CreateSyncStarterCallback() {
  return base::Bind(&OneClickSigninHelper::SyncSetupCompletedCallback,
                    weak_pointer_factory_.GetWeakPtr());
}
