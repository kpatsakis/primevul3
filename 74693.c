void AutoFillManager::GetCreditCardSuggestions(FormStructure* form,
                                               const FormField& field,
                                               AutoFillType type,
                                               std::vector<string16>* values,
                                               std::vector<string16>* labels,
                                               std::vector<int>* unique_ids) {
  if (!FormIsHTTPS(form))
    return;

  for (std::vector<CreditCard*>::const_iterator iter =
           personal_data_->credit_cards().begin();
       iter != personal_data_->credit_cards().end(); ++iter) {
    CreditCard* credit_card = *iter;

    string16 creditcard_field_value =
        credit_card->GetFieldText(type);
    if (!creditcard_field_value.empty() &&
        StartsWith(creditcard_field_value, field.value(), false)) {
      if (type.field_type() == CREDIT_CARD_NUMBER)
        creditcard_field_value = credit_card->ObfuscatedNumber();

      if (!form->HasNonBillingFields()) {
        values->push_back(creditcard_field_value);
        labels->push_back(credit_card->Label());
        unique_ids->push_back(credit_card->unique_id());
      } else {
        for (std::vector<AutoFillProfile*>::const_iterator iter =
                 personal_data_->profiles().begin();
             iter != personal_data_->profiles().end(); ++iter) {
          values->push_back(creditcard_field_value);

          string16 label = (*iter)->Label() +
                           ASCIIToUTF16("; ") +
                           credit_card->LastFourDigits();
          labels->push_back(label);
          unique_ids->push_back((*iter)->unique_id());
        }
      }
    }
  }
}
