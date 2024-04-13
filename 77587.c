void ProfileImplIOData::Handle::Init(
      const FilePath& cookie_path,
      const FilePath& origin_bound_cert_path,
      const FilePath& cache_path,
      int cache_max_size,
      const FilePath& media_cache_path,
      int media_cache_max_size,
      const FilePath& extensions_cookie_path,
      const FilePath& app_path,
      chrome_browser_net::Predictor* predictor,
      PrefService* local_state,
      IOThread* io_thread,
      bool restore_old_session_cookies) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!io_data_->lazy_params_.get());
  DCHECK(predictor);

  LazyParams* lazy_params = new LazyParams;

  lazy_params->cookie_path = cookie_path;
  lazy_params->origin_bound_cert_path = origin_bound_cert_path;
  lazy_params->cache_path = cache_path;
  lazy_params->cache_max_size = cache_max_size;
  lazy_params->media_cache_path = media_cache_path;
  lazy_params->media_cache_max_size = media_cache_max_size;
  lazy_params->extensions_cookie_path = extensions_cookie_path;
  lazy_params->restore_old_session_cookies = restore_old_session_cookies;

  io_data_->lazy_params_.reset(lazy_params);

  io_data_->app_path_ = app_path;

  io_data_->predictor_.reset(predictor);

  if (!main_request_context_getter_) {
    main_request_context_getter_ =
        ChromeURLRequestContextGetter::CreateOriginal(
            profile_, io_data_);
  }
  io_data_->predictor_->InitNetworkPredictor(profile_->GetPrefs(),
                                             local_state,
                                             io_thread,
                                             main_request_context_getter_);
}
