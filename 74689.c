bool FormIsHTTPS(FormStructure* form) {
  return form->ConvertToFormData().origin.SchemeIs(chrome::kHttpsScheme);
}
