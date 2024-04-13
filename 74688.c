void AutoFillManager::FillSelectOneField(const AutoFillProfile* profile,
                                         AutoFillType type,
                                         webkit_glue::FormField* field) {
  DCHECK(profile);
  DCHECK(field);
  DCHECK(field->form_control_type() == ASCIIToUTF16("select-one"));
  string16 selected_string = profile->GetFieldText(type);
  std::string ascii_value = UTF16ToASCII(selected_string);
  for (size_t i = 0; i < field->option_strings().size(); ++i) {
    if (profile->GetFieldText(type) == field->option_strings()[i]) {
      selected_string = profile->GetFieldText(type);
      break;
    }
    if (!base::strcasecmp(UTF16ToASCII(field->option_strings()[i]).c_str(),
                          ascii_value.c_str())) {
      selected_string = field->option_strings()[i];
    }
  }
  field->set_value(selected_string);
}
