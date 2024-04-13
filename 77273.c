void OmniboxViewWin::GetSelection(CHARRANGE& sel) const {
  GetSel(sel);

  ITextDocument* const text_object_model = GetTextObjectModel();
  if (!text_object_model)
    return;
  base::win::ScopedComPtr<ITextSelection> selection;
  const HRESULT hr = text_object_model->GetSelection(selection.Receive());
  DCHECK_EQ(S_OK, hr);
  long flags;
  selection->GetFlags(&flags);
  if (flags & tomSelStartActive)
    std::swap(sel.cpMin, sel.cpMax);
}
