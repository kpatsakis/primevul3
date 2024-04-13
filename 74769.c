void RenderView::LogNavigationState(const NavigationState* state,
                                    const WebDataSource* ds) const {
  if (logging::LOG_INFO < logging::GetMinLogLevel())
    return;

  DCHECK(state);
  DCHECK(ds);
  GURL url(ds->request().url());
  Time start = state->start_load_time();
  Time finish = state->finish_load_time();
  LOG(INFO) << "PLT: "
            << (finish - start).InMilliseconds()
            << "ms "
            << url.spec();
}
