void AutoFillManager::FillPhoneNumberField(const AutoFillProfile* profile,
                                           webkit_glue::FormField* field) {
  string16 number = profile->GetFieldText(AutoFillType(PHONE_HOME_NUMBER));
  bool has_valid_suffix_and_prefix = (number.length() ==
      (kAutoFillPhoneNumberPrefixCount + kAutoFillPhoneNumberSuffixCount));
  if (has_valid_suffix_and_prefix &&
      field->size() == kAutoFillPhoneNumberPrefixCount) {
    number = number.substr(kAutoFillPhoneNumberPrefixOffset,
                           kAutoFillPhoneNumberPrefixCount);
    field->set_value(number);
  } else if (has_valid_suffix_and_prefix &&
             field->size() == kAutoFillPhoneNumberSuffixCount) {
    number = number.substr(kAutoFillPhoneNumberSuffixOffset,
                           kAutoFillPhoneNumberSuffixCount);
    field->set_value(number);
  } else {
    field->set_value(number);
  }
}
