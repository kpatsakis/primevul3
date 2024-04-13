bool BasePrefsChange::Init(Profile* profile) {
  if (!BaseSettingChange::Init(profile))
    return false;
  pref_observer_.reset(new PrefSetObserver(profile->GetPrefs(), this));
   return true;
 }
