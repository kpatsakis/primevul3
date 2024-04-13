void AutoFillManager::FormsSeen(const std::vector<FormData>& forms) {
  if (!IsAutoFillEnabled())
    return;

  if (personal_data_->profiles().empty() &&
      personal_data_->credit_cards().empty())
    return;

  ParseForms(forms);
}
