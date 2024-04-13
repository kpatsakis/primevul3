string16 OmniboxViewWin::GetLabelForCommandId(int command_id) const {
  DCHECK_EQ(IDS_PASTE_AND_GO, command_id);
  return l10n_util::GetStringUTF16(model_->is_paste_and_search() ?
      IDS_PASTE_AND_SEARCH : IDS_PASTE_AND_GO);
}
