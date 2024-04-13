void OmniboxViewWin::MoveSelectedText(int new_position) {
  const string16 selected_text(GetSelectedText());
  CHARRANGE sel;
  GetSel(sel);
  DCHECK((sel.cpMax != sel.cpMin) && (new_position >= 0) &&
         (new_position <= GetTextLength()));

  ScopedFreeze freeze(this, GetTextObjectModel());
  OnBeforePossibleChange();

  ReplaceSel(L"", TRUE);

  if (new_position >= sel.cpMin)
    new_position -= (sel.cpMax - sel.cpMin);
  PlaceCaretAt(new_position);
  ReplaceSel(selected_text.c_str(), TRUE);

  OnAfterPossibleChange();
}
