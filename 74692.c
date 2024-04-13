void AutoFillManager::GetBillingProfileSuggestions(
    FormStructure* form,
    const FormField& field,
    AutoFillType type,
    std::vector<string16>* values,
    std::vector<string16>* labels,
    std::vector<int>* unique_ids) {
  std::vector<CreditCard*> matching_creditcards;
  std::vector<AutoFillProfile*> matching_profiles;
  std::vector<string16> cc_values;
  std::vector<string16> cc_labels;

  if (!FormIsHTTPS(form)) {
    GetProfileSuggestions(form, field, type, false, values, labels, unique_ids);
    return;
  }

  for (std::vector<CreditCard*>::const_iterator cc =
           personal_data_->credit_cards().begin();
       cc != personal_data_->credit_cards().end(); ++cc) {
    string16 label = (*cc)->billing_address();
    AutoFillProfile* billing_profile = NULL;

    string16 profile_field_value;

    for (std::vector<AutoFillProfile*>::const_iterator iter =
             personal_data_->profiles().begin();
         iter != personal_data_->profiles().end(); ++iter) {
      AutoFillProfile* profile = *iter;

      if (profile->Label() == label &&
          !profile->GetFieldText(type).empty() &&
          StartsWith(profile->GetFieldText(type), field.value(), false)) {
        billing_profile = profile;
        break;
      }
    }

    if (!billing_profile)
      continue;

    for (std::vector<AutoFillProfile*>::const_iterator iter =
             personal_data_->profiles().begin();
         iter != personal_data_->profiles().end(); ++iter) {
      values->push_back(billing_profile->GetFieldText(type));

      string16 label = (*iter)->Label() +
                       ASCIIToUTF16("; ") +
                       (*cc)->LastFourDigits();
      labels->push_back(label);
      unique_ids->push_back((*iter)->unique_id());
    }
  }
}
