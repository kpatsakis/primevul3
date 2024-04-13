void AutoFillManager::GetProfileSuggestions(FormStructure* form,
                                            const FormField& field,
                                            AutoFillType type,
                                            bool include_cc_labels,
                                            std::vector<string16>* values,
                                            std::vector<string16>* labels,
                                            std::vector<int>* unique_ids) {
  const std::vector<AutoFillProfile*>& profiles = personal_data_->profiles();
  std::vector<AutoFillProfile*> matched_profiles;
  for (std::vector<AutoFillProfile*>::const_iterator iter = profiles.begin();
       iter != profiles.end(); ++iter) {
    AutoFillProfile* profile = *iter;

    string16 profile_field_value = profile->GetFieldText(type);

    if (!profile_field_value.empty() &&
        StartsWith(profile_field_value, field.value(), false)) {
      matched_profiles.push_back(profile);
      values->push_back(profile_field_value);
      unique_ids->push_back(profile->unique_id());
    }
  }

  AutoFillProfile::CreateInferredLabels(&matched_profiles, labels, 0,
                                        type.field_type());

  if (!include_cc_labels || !form->HasBillingFields() || !FormIsHTTPS(form))
    return;

  size_t i = 0;
  std::vector<string16> expanded_values;
  std::vector<string16> expanded_labels;
  for (std::vector<AutoFillProfile*>::const_iterator iter =
       matched_profiles.begin(); iter != matched_profiles.end();
       ++iter, ++i) {
    AutoFillProfile* profile = *iter;
    for (std::vector<CreditCard*>::const_iterator cc =
         personal_data_->credit_cards().begin();
         cc != personal_data_->credit_cards().end(); ++cc) {
      expanded_values.push_back((*values)[i]);
      string16 label = (*labels)[i] + kLabelSeparator +
          (*cc)->LastFourDigits();
      expanded_labels.push_back(label);
      unique_ids->push_back(profile->unique_id());
    }
  }
  expanded_labels.swap(*labels);
  expanded_values.swap(*values);
}
