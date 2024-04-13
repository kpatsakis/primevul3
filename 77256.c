void OmniboxViewWin::EmphasizeURLComponents() {
  ITextDocument* const text_object_model = GetTextObjectModel();
  ScopedFreeze freeze(this, text_object_model);
  ScopedSuspendUndo suspend_undo(text_object_model);

  CHARRANGE saved_sel;
  GetSelection(saved_sel);

  url_parse::Component scheme, host;
  AutocompleteInput::ParseForEmphasizeComponents(
      GetText(), model_->GetDesiredTLD(), &scheme, &host);
  const bool emphasize = model_->CurrentTextIsURL() && (host.len > 0);

  CHARFORMAT cf = {0};
  cf.dwMask = CFM_COLOR;
  cf.crTextColor = skia::SkColorToCOLORREF(LocationBarView::GetColor(
      security_level_,
      emphasize ? LocationBarView::DEEMPHASIZED_TEXT : LocationBarView::TEXT));
  SelectAll(false);
  SetSelectionCharFormat(cf);

  if (emphasize) {
    cf.crTextColor = skia::SkColorToCOLORREF(LocationBarView::GetColor(
        security_level_, LocationBarView::TEXT));
    SetSelection(host.begin, host.end());
    SetSelectionCharFormat(cf);
  }

  insecure_scheme_component_.reset();
  if (!model_->user_input_in_progress() && scheme.is_nonempty() &&
      (security_level_ != ToolbarModel::NONE)) {
    if (security_level_ == ToolbarModel::SECURITY_ERROR) {
      insecure_scheme_component_.begin = scheme.begin;
      insecure_scheme_component_.len = scheme.len;
    }
    cf.crTextColor = skia::SkColorToCOLORREF(LocationBarView::GetColor(
        security_level_, LocationBarView::SECURITY_TEXT));
    SetSelection(scheme.begin, scheme.end());
    SetSelectionCharFormat(cf);
  }

  SetSelectionRange(saved_sel);
}
