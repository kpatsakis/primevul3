bool AutoFillManager::FillAutoFillFormData(int query_id,
                                           const FormData& form,
                                           const string16& value,
                                           const string16& label,
                                           int unique_id) {
  if (!IsAutoFillEnabled())
    return false;

  RenderViewHost* host = tab_contents_->render_view_host();
  if (!host)
    return false;

  const std::vector<AutoFillProfile*>& profiles = personal_data_->profiles();
  const std::vector<CreditCard*>& credit_cards = personal_data_->credit_cards();

  if (profiles.empty() && credit_cards.empty())
    return false;

  FormData result = form;
  FormStructure* form_structure = NULL;
  for (std::vector<FormStructure*>::const_iterator iter =
           form_structures_.begin();
       iter != form_structures_.end(); ++iter) {
    if (**iter == form) {
      form_structure = *iter;
      break;
    }
  }

  if (!form_structure)
    return false;

  if (!form_structure->autofill_count())
    return false;

  string16 cc_digits;

  if (form_structure->HasBillingFields()) {
    size_t index = label.find_last_of(kLabelSeparator);
    if (index != string16::npos) {
      size_t cc_index = index + 1;
      cc_digits = label.substr(cc_index);
    }
  }

  const AutoFillProfile* profile = NULL;
  for (std::vector<AutoFillProfile*>::const_iterator iter = profiles.begin();
       iter != profiles.end(); ++iter) {
    if ((*iter)->unique_id() == unique_id) {
      profile = *iter;
    }
  }

  const CreditCard* credit_card = NULL;
  if (!cc_digits.empty()) {
    for (std::vector<CreditCard*>::const_iterator iter = credit_cards.begin();
         iter != credit_cards.end(); ++iter) {
      if ((*iter)->LastFourDigits() == cc_digits) {
        credit_card = *iter;
        break;
      }
    }
  }

  if (!profile && !credit_card)
    return false;

  for (size_t i = 0, j = 0;
       i < form_structure->field_count() && j < result.fields.size();
       j++) {
    size_t k = i;

    while (k < form_structure->field_count() &&
           *form_structure->field(k) != result.fields[j]) {
      k++;
    }

    if (k >= form_structure->field_count())
      continue;

    const AutoFillField* field = form_structure->field(k);
    AutoFillType autofill_type(field->type());
    if (credit_card &&
        autofill_type.group() == AutoFillType::CREDIT_CARD) {
      result.fields[i].set_value(
          credit_card->GetFieldText(autofill_type));
    } else if (credit_card &&
               autofill_type.group() == AutoFillType::ADDRESS_BILLING) {
      FillBillingFormField(credit_card, autofill_type, &result.fields[j]);
    } else if (profile) {
      FillFormField(profile, autofill_type, &result.fields[j]);
    }

    ++i;
  }

  host->AutoFillFormDataFilled(query_id, result);
  return true;
}
