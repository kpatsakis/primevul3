ProfileImplIOData::InitializeAppRequestContext(
    scoped_refptr<ChromeURLRequestContext> main_context,
    const std::string& app_id) const {
  AppRequestContext* context = new AppRequestContext;

  context->CopyFrom(main_context);

  FilePath app_path = app_path_.AppendASCII(app_id);
  FilePath cookie_path = app_path.Append(chrome::kCookieFilename);
  FilePath cache_path = app_path.Append(chrome::kCacheDirname);
  int cache_max_size = 0;

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  bool record_mode = chrome::kRecordModeEnabled &&
                     command_line.HasSwitch(switches::kRecordMode);
  bool playback_mode = command_line.HasSwitch(switches::kPlaybackMode);

  net::HttpCache::DefaultBackend* app_backend =
      new net::HttpCache::DefaultBackend(
          net::DISK_CACHE,
          cache_path,
          cache_max_size,
          BrowserThread::GetMessageLoopProxyForThread(BrowserThread::CACHE));
  net::HttpNetworkSession* main_network_session =
      main_http_factory_->GetSession();
  net::HttpCache* app_http_cache =
      new net::HttpCache(main_network_session, app_backend);

  scoped_refptr<net::CookieStore> cookie_store = NULL;
  if (record_mode || playback_mode) {
    cookie_store = new net::CookieMonster(NULL, NULL);
    app_http_cache->set_mode(
        record_mode ? net::HttpCache::RECORD : net::HttpCache::PLAYBACK);
  }

  if (!cookie_store) {
    DCHECK(!cookie_path.empty());

    scoped_refptr<SQLitePersistentCookieStore> cookie_db =
        new SQLitePersistentCookieStore(cookie_path, false);
    cookie_db->SetClearLocalStateOnExit(clear_local_state_on_exit_);
    cookie_store = new net::CookieMonster(cookie_db.get(), NULL);
  }

  context->SetCookieStore(cookie_store);
  context->SetHttpTransactionFactory(app_http_cache);

  return context;
}
