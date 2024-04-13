void OmniboxEditModel::SetInputInProgress(bool in_progress) {
  if (in_progress && !user_input_since_focus_) {
    base::TimeTicks now = base::TimeTicks::Now();
    DCHECK(last_omnibox_focus_ <= now);
    UMA_HISTOGRAM_TIMES(kFocusToEditTimeHistogram, now - last_omnibox_focus_);
    user_input_since_focus_ = true;
  }

  if (user_input_in_progress_ == in_progress)
    return;

  user_input_in_progress_ = in_progress;
  if (user_input_in_progress_) {
    time_user_first_modified_omnibox_ = base::TimeTicks::Now();
    content::RecordAction(base::UserMetricsAction("OmniboxInputInProgress"));
    autocomplete_controller()->ResetSession();
  }

  if (chrome::ShouldDisplayOriginChipV2() && in_progress)
    controller()->GetToolbarModel()->set_origin_chip_enabled(false);

  controller_->GetToolbarModel()->set_input_in_progress(in_progress);
  controller_->Update(NULL);

  if (user_input_in_progress_ || !in_revert_)
    delegate_->OnInputStateChanged();
}
