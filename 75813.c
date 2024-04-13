Profile* GetDefaultProfile(Profile* profile) {
  if (!profile->IsOffTheRecord())
    return profile;
  else
    return g_browser_process->profile_manager()->GetDefaultProfile();
}
