bool AutoFillManager::GetAutoFillSuggestions(int query_id,
                                             bool form_autofilled,
                                             const FormField& field) {
  if (!IsAutoFillEnabled())
    return false;

  RenderViewHost* host = tab_contents_->render_view_host();
  if (!host)
    return false;

  if (personal_data_->profiles().empty() &&
      personal_data_->credit_cards().empty())
    return false;

  FormStructure* form = NULL;
  AutoFillField* autofill_field = NULL;
  for (std::vector<FormStructure*>::iterator form_iter =
           form_structures_.begin();
       form_iter != form_structures_.end(); ++form_iter) {
    form = *form_iter;

    if (!form->IsAutoFillable())
      continue;

    for (std::vector<AutoFillField*>::const_iterator iter = form->begin();
         iter != form->end(); ++iter) {
      if (!*iter)
        break;

      if ((**iter) == field) {
        autofill_field = *iter;
        break;
      }
    }
  }

  if (autofill_field == NULL)
    return false;

  std::vector<string16> values;
  std::vector<string16> labels;
  std::vector<int> unique_ids;
  AutoFillType type(autofill_field->type());

  bool handle_billing = FormIsHTTPS(form);

  if (type.group() == AutoFillType::CREDIT_CARD)
    GetCreditCardSuggestions(form, field, type, &values, &labels, &unique_ids);
  else if (type.group() == AutoFillType::ADDRESS_BILLING)
    GetBillingProfileSuggestions(
        form, field, type, &values, &labels, &unique_ids);
  else
    GetProfileSuggestions(
        form, field, type, handle_billing, &values, &labels, &unique_ids);

  DCHECK_EQ(values.size(), labels.size());
  DCHECK_EQ(values.size(), unique_ids.size());

  if (values.empty())
    return false;

  if (form_autofilled) {
    RemoveDuplicateElements(&values, &unique_ids);
    labels.resize(values.size());

    for (size_t i = 0; i < labels.size(); ++i)
      labels[i] = string16();
  }

  host->AutoFillSuggestionsReturned(query_id, values, labels, unique_ids);
  return true;
}
