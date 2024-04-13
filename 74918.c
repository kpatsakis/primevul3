void RenderView::didNavigateWithinPage(
    WebFrame* frame, bool is_new_navigation) {

  NavigationState* state =
    NavigationState::FromDataSource(frame->dataSource());
  bool idle_scheduler_ran = state->user_script_idle_scheduler()->has_run();

  didCreateDataSource(frame, frame->dataSource());

  if (idle_scheduler_ran) {
    NavigationState* new_state =
        NavigationState::FromDataSource(frame->dataSource());
    new_state->user_script_idle_scheduler()->set_has_run(true);
  }

  didCommitProvisionalLoad(frame, is_new_navigation);

  UpdateTitle(frame, frame->view()->mainFrame()->dataSource()->pageTitle());
}
