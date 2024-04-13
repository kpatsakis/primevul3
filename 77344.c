void OmniboxViewWin::SetSelection(LONG start, LONG end) {
  SetSel(start, end);

  if (start <= end)
    return;

  ITextDocument* const text_object_model = GetTextObjectModel();
  if (!text_object_model)
    return;
  base::win::ScopedComPtr<ITextSelection> selection;
  const HRESULT hr = text_object_model->GetSelection(selection.Receive());
  DCHECK_EQ(S_OK, hr);
  selection->SetFlags(tomSelStartActive);
}
