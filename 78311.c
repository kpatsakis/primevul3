void OmniboxEditModel::Revert() {
  SetInputInProgress(false);
  paste_state_ = NONE;
  InternalSetUserText(base::string16());
  keyword_.clear();
  is_keyword_hint_ = false;
  has_temporary_text_ = false;
  view_->SetWindowTextAndCaretPos(permanent_text_,
                                  has_focus() ? permanent_text_.length() : 0,
                                  false, true);
  AutocompleteActionPredictor* action_predictor =
      predictors::AutocompleteActionPredictorFactory::GetForProfile(profile_);
  action_predictor->ClearTransitionalMatches();
  action_predictor->CancelPrerender();
}
