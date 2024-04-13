void AddEmailToOneClickRejectedList(Profile* profile,
                                    const std::string& email) {
  ListPrefUpdate updater(profile->GetPrefs(),
                         prefs::kReverseAutologinRejectedEmailList);
  updater->AppendIfNotPresent(new base::StringValue(email));
}
