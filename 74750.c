void RenderView::DumpLoadHistograms() const {
  static const TimeDelta kPLTMin(TimeDelta::FromMilliseconds(10));
  static const TimeDelta kPLTMax(TimeDelta::FromMinutes(10));
  static const size_t kPLTCount(100);
  #define PLT_HISTOGRAM(name, sample) \
      UMA_HISTOGRAM_CUSTOM_TIMES(name, sample, kPLTMin, kPLTMax, kPLTCount);

  WebFrame* main_frame = webview()->mainFrame();
  NavigationState* navigation_state =
      NavigationState::FromDataSource(main_frame->dataSource());

  if (navigation_state->load_histograms_recorded())
    return;

  Time start = navigation_state->start_load_time();
  if (start.is_null())
    return;  // Probably very premature abandonment of page.
  Time commit = navigation_state->commit_load_time();
  if (commit.is_null())
    return;  // Probably very premature abandonment of page.

  Time request = navigation_state->request_time();
  Time first_paint = navigation_state->first_paint_time();
  Time first_paint_after_load = navigation_state->first_paint_after_load_time();
  Time finish = navigation_state->finish_load_time();
  Time finish_doc = navigation_state->finish_document_load_time();

  bool abandoned_page = finish.is_null();
  if (abandoned_page) {
    finish = Time::Now();
    navigation_state->set_finish_load_time(finish);
  }

  if (finish_doc.is_null()) {
    DCHECK(abandoned_page);  // how can the doc have finished but not the page?
    if (!abandoned_page)
      return;  // Don't try to record a stat which is broken.
    finish_doc = finish;
    navigation_state->set_finish_document_load_time(finish_doc);
  }

  Time begin = request.is_null() ? start : request;
  TimeDelta begin_to_finish_doc = finish_doc - begin;
  TimeDelta begin_to_finish = finish - begin;
  TimeDelta start_to_finish = finish - start;
  TimeDelta start_to_commit = commit - start;

  NavigationState::LoadType load_type = navigation_state->load_type();

  UMA_HISTOGRAM_ENUMERATION("PLT.Abandoned", abandoned_page ? 1 : 0, 2);
  UMA_HISTOGRAM_ENUMERATION("PLT.LoadType", load_type,
      NavigationState::kLoadTypeMax);
  PLT_HISTOGRAM("PLT.StartToCommit", start_to_commit);
  PLT_HISTOGRAM("PLT.CommitToFinishDoc", finish_doc - commit);
  PLT_HISTOGRAM("PLT.FinishDocToFinish", finish - finish_doc);
  PLT_HISTOGRAM("PLT.BeginToCommit", commit - begin);
  PLT_HISTOGRAM("PLT.StartToFinish", start_to_finish);
  if (!request.is_null()) {
    PLT_HISTOGRAM("PLT.RequestToStart", start - request);
    PLT_HISTOGRAM("PLT.RequestToFinish", finish - request);
  }
  PLT_HISTOGRAM("PLT.CommitToFinish", finish - commit);
  if (!first_paint.is_null()) {
    DCHECK(begin <= first_paint);
    PLT_HISTOGRAM("PLT.BeginToFirstPaint", first_paint - begin);
    DCHECK(commit <= first_paint);
    PLT_HISTOGRAM("PLT.CommitToFirstPaint", first_paint - commit);
  }
  if (!first_paint_after_load.is_null()) {
    DCHECK(begin <= first_paint_after_load);
    PLT_HISTOGRAM("PLT.BeginToFirstPaintAfterLoad",
      first_paint_after_load - begin);
    DCHECK(commit <= first_paint_after_load);
    PLT_HISTOGRAM("PLT.CommitToFirstPaintAfterLoad",
        first_paint_after_load - commit);
    DCHECK(finish <= first_paint_after_load);
    PLT_HISTOGRAM("PLT.FinishToFirstPaintAfterLoad",
        first_paint_after_load - finish);
  }
  PLT_HISTOGRAM("PLT.BeginToFinishDoc", begin_to_finish_doc);
  PLT_HISTOGRAM("PLT.BeginToFinish", begin_to_finish);

  switch (load_type) {
    case NavigationState::UNDEFINED_LOAD:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_UndefLoad", begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_UndefLoad", begin_to_finish);
      break;
    case NavigationState::RELOAD:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_Reload", begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_Reload", begin_to_finish);
      break;
    case NavigationState::HISTORY_LOAD:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_HistoryLoad", begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_HistoryLoad", begin_to_finish);
      break;
    case NavigationState::NORMAL_LOAD:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_NormalLoad", begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_NormalLoad", begin_to_finish);
      break;
    case NavigationState::LINK_LOAD_NORMAL:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_LinkLoadNormal",
          begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_LinkLoadNormal", begin_to_finish);
      break;
    case NavigationState::LINK_LOAD_RELOAD:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_LinkLoadReload",
           begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_LinkLoadReload", begin_to_finish);
      break;
    case NavigationState::LINK_LOAD_CACHE_STALE_OK:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_LinkLoadStaleOk",
           begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_LinkLoadStaleOk", begin_to_finish);
      break;
    case NavigationState::LINK_LOAD_CACHE_ONLY:
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_LinkLoadCacheOnly",
           begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_LinkLoadCacheOnly", begin_to_finish);
      break;
    default:
      break;
  }

  static bool use_dns_histogram(FieldTrialList::Find("DnsImpact") &&
      !FieldTrialList::Find("DnsImpact")->group_name().empty());
  if (use_dns_histogram) {
    UMA_HISTOGRAM_ENUMERATION(
        FieldTrial::MakeName("PLT.Abandoned", "DnsImpact"),
        abandoned_page ? 1 : 0, 2);
    UMA_HISTOGRAM_ENUMERATION(
        FieldTrial::MakeName("PLT.LoadType", "DnsImpact"),
        load_type, NavigationState::kLoadTypeMax);
    switch (load_type) {
      case NavigationState::NORMAL_LOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_NormalLoad", "DnsImpact"), begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_NORMAL:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadNormal", "DnsImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_RELOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadReload", "DnsImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_CACHE_STALE_OK:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadStaleOk", "DnsImpact"),
            begin_to_finish);
        break;
      default:
        break;
    }
  }

  static const bool prefetching_fieldtrial =
      FieldTrialList::Find("Prefetch") &&
      !FieldTrialList::Find("Prefetch")->group_name().empty();
  static const bool prefetching_explicitly_disabled =
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableContentPrefetch);
  if (navigation_state->was_prefetcher()) {
    if (!prefetching_explicitly_disabled) {
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_ContentPrefetcher",
                    begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_ContentPrefetcher",
                    begin_to_finish);
    }
    if (prefetching_fieldtrial) {
      PLT_HISTOGRAM(
          FieldTrial::MakeName("PLT.BeginToFinishDoc_ContentPrefetcher",
                               "Prefetch"),
          begin_to_finish_doc);
      PLT_HISTOGRAM(
          FieldTrial::MakeName("PLT.BeginToFinish_ContentPrefetcher",
                               "Prefetch"),
          begin_to_finish);
    }
  }
  if (navigation_state->was_referred_by_prefetcher()) {
    if (!prefetching_explicitly_disabled) {
      PLT_HISTOGRAM("PLT.BeginToFinishDoc_ContentPrefetcherReferrer",
                    begin_to_finish_doc);
      PLT_HISTOGRAM("PLT.BeginToFinish_ContentPrefetcherReferrer",
                    begin_to_finish);
    }
    if (prefetching_fieldtrial) {
      PLT_HISTOGRAM(
          FieldTrial::MakeName("PLT.BeginToFinishDoc_ContentPrefetcherReferrer",
                               "Prefetch"),
          begin_to_finish_doc);
      PLT_HISTOGRAM(
          FieldTrial::MakeName("PLT.BeginToFinish_ContentPrefetcherReferrer",
                               "Prefetch"),
          begin_to_finish);
    }
  }
  if (prefetching_fieldtrial) {
    UMA_HISTOGRAM_ENUMERATION(FieldTrial::MakeName("PLT.Abandoned", "Prefetch"),
                              abandoned_page ? 1 : 0, 2);
    PLT_HISTOGRAM(FieldTrial::MakeName("PLT.BeginToFinishDoc", "Prefetch"),
                  begin_to_finish_doc);
    PLT_HISTOGRAM(FieldTrial::MakeName("PLT.BeginToFinish", "Prefetch"),
                  begin_to_finish);
  }

  static bool use_connection_impact_histogram(
      FieldTrialList::Find("ConnCountImpact") &&
      !FieldTrialList::Find("ConnCountImpact")->group_name().empty());
  if (use_connection_impact_histogram) {
    UMA_HISTOGRAM_ENUMERATION(
        FieldTrial::MakeName("PLT.Abandoned", "ConnCountImpact"),
        abandoned_page ? 1 : 0, 2);
    switch (load_type) {
      case NavigationState::NORMAL_LOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_NormalLoad", "ConnCountImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_NORMAL:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadNormal", "ConnCountImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_RELOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadReload", "ConnCountImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_CACHE_STALE_OK:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadStaleOk", "ConnCountImpact"),
            begin_to_finish);
        break;
      default:
        break;
    }
  }

  static bool use_idle_socket_timeout_histogram(
      FieldTrialList::Find("IdleSktToImpact") &&
      !FieldTrialList::Find("IdleSktToImpact")->group_name().empty());
  if (use_idle_socket_timeout_histogram) {
    UMA_HISTOGRAM_ENUMERATION(
        FieldTrial::MakeName("PLT.Abandoned", "IdleSktToImpact"),
        abandoned_page ? 1 : 0, 2);
    switch (load_type) {
      case NavigationState::NORMAL_LOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_NormalLoad", "IdleSktToImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_NORMAL:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadNormal", "IdleSktToImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_RELOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadReload", "IdleSktToImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_CACHE_STALE_OK:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadStaleOk", "IdleSktToImpact"),
            begin_to_finish);
        break;
      default:
        break;
    }
  }

  static bool use_proxy_connection_impact_histogram(
      FieldTrialList::Find("ProxyConnectionImpact") &&
      !FieldTrialList::Find("ProxyConnectionImpact")->group_name().empty());
  if (use_proxy_connection_impact_histogram) {
    UMA_HISTOGRAM_ENUMERATION(
        FieldTrial::MakeName("PLT.Abandoned", "ProxyConnectionImpact"),
        abandoned_page ? 1 : 0, 2);
    switch (load_type) {
      case NavigationState::NORMAL_LOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_NormalLoad", "ProxyConnectionImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_NORMAL:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadNormal", "ProxyConnectionImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_RELOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadReload", "ProxyConnectionImpact"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_CACHE_STALE_OK:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadStaleOk", "ProxyConnectionImpact"),
            begin_to_finish);
        break;
      default:
        break;
    }
  }

  static bool use_sdch_histogram(FieldTrialList::Find("GlobalSdch") &&
      !FieldTrialList::Find("GlobalSdch")->group_name().empty());
  if (use_sdch_histogram) {
    UMA_HISTOGRAM_ENUMERATION(
        FieldTrial::MakeName("PLT.LoadType", "GlobalSdch"),
        load_type, NavigationState::kLoadTypeMax);
    switch (load_type) {
      case NavigationState::NORMAL_LOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_NormalLoad", "GlobalSdch"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_NORMAL:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadNormal", "GlobalSdch"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_RELOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadReload", "GlobalSdch"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_CACHE_STALE_OK:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadStaleOk", "GlobalSdch"),
            begin_to_finish);
        break;
      case NavigationState::LINK_LOAD_CACHE_ONLY:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadCacheOnly", "GlobalSdch"),
            begin_to_finish);
        break;
      default:
        break;
    }
  }

  static bool use_cache_histogram1(FieldTrialList::Find("CacheSize") &&
      !FieldTrialList::Find("CacheSize")->group_name().empty());
  if (use_cache_histogram1 && NavigationState::LINK_LOAD_NORMAL <= load_type &&
      NavigationState::LINK_LOAD_CACHE_ONLY >= load_type) {
    PLT_HISTOGRAM(FieldTrial::MakeName(
        "PLT.BeginToFinishDoc_LinkLoad", "CacheSize"), begin_to_finish_doc);
  }


  GURL url = GURL(main_frame->url());

  static bool use_spdy_histogram(FieldTrialList::Find("SpdyImpact") &&
      !FieldTrialList::Find("SpdyImpact")->group_name().empty());
  if (use_spdy_histogram && url.SchemeIs("https") &&
      navigation_state->was_npn_negotiated()) {
    UMA_HISTOGRAM_ENUMERATION(
        FieldTrial::MakeName("PLT.Abandoned", "SpdyImpact"),
        abandoned_page ? 1 : 0, 2);
    switch (load_type) {
      case NavigationState::LINK_LOAD_NORMAL:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_LinkLoadNormal_SpdyTrial", "SpdyImpact"),
            begin_to_finish);
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.StartToFinish_LinkLoadNormal_SpdyTrial", "SpdyImpact"),
            start_to_finish);
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.StartToCommit_LinkLoadNormal_SpdyTrial", "SpdyImpact"),
            start_to_commit);
        break;
      case NavigationState::NORMAL_LOAD:
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.BeginToFinish_NormalLoad_SpdyTrial", "SpdyImpact"),
            begin_to_finish);
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.StartToFinish_NormalLoad_SpdyTrial", "SpdyImpact"),
            start_to_finish);
        PLT_HISTOGRAM(FieldTrial::MakeName(
            "PLT.StartToCommit_NormalLoad_SpdyTrial", "SpdyImpact"),
            start_to_commit);
        break;
      default:
        break;
    }
  }

  if (url.SchemeIs("http") &&
      navigation_state->was_alternate_protocol_available()) {
    if (!navigation_state->was_npn_negotiated()) {
      switch (load_type) {
        case NavigationState::LINK_LOAD_NORMAL:
          PLT_HISTOGRAM(
              "PLT.StartToFinish_LinkLoadNormal_AlternateProtocol_http",
              start_to_finish);
          PLT_HISTOGRAM(
              "PLT.StartToCommit_LinkLoadNormal_AlternateProtocol_http",
              start_to_commit);
          break;
        case NavigationState::NORMAL_LOAD:
          PLT_HISTOGRAM(
              "PLT.StartToFinish_NormalLoad_AlternateProtocol_http",
              start_to_finish);
          PLT_HISTOGRAM(
              "PLT.StartToCommit_NormalLoad_AlternateProtocol_http",
              start_to_commit);
          break;
        default:
          break;
      }
    } else if (navigation_state->was_fetched_via_spdy()) {
      switch (load_type) {
        case NavigationState::LINK_LOAD_NORMAL:
          PLT_HISTOGRAM(
              "PLT.StartToFinish_LinkLoadNormal_AlternateProtocol_spdy",
              start_to_finish);
          PLT_HISTOGRAM(
              "PLT.StartToCommit_LinkLoadNormal_AlternateProtocol_spdy",
              start_to_commit);
          break;
        case NavigationState::NORMAL_LOAD:
          PLT_HISTOGRAM(
              "PLT.StartToFinish_NormalLoad_AlternateProtocol_spdy",
              start_to_finish);
          PLT_HISTOGRAM(
              "PLT.StartToCommit_NormalLoad_AlternateProtocol_spdy",
              start_to_commit);
          break;
        default:
          break;
      }
    }
  }

  if (navigation_state->was_fetched_via_proxy()) {
    if (url.SchemeIs("https")) {
      PLT_HISTOGRAM("PLT.StartToFinish.Proxy.https", start_to_finish);
      UMA_HISTOGRAM_ENUMERATION("PLT.Abandoned.Proxy.https",
          abandoned_page ? 1 : 0, 2);
    } else {
      PLT_HISTOGRAM("PLT.StartToFinish.Proxy.http", start_to_finish);
      UMA_HISTOGRAM_ENUMERATION("PLT.Abandoned.Proxy.http",
          abandoned_page ? 1 : 0, 2);
    }
  } else {
    if (url.SchemeIs("https")) {
      PLT_HISTOGRAM("PLT.StartToFinish.NoProxy.https", start_to_finish);
      UMA_HISTOGRAM_ENUMERATION("PLT.Abandoned.NoProxy.https",
          abandoned_page ? 1 : 0, 2);
    } else {
      PLT_HISTOGRAM("PLT.StartToFinish.NoProxy.http", start_to_finish);
      UMA_HISTOGRAM_ENUMERATION("PLT.Abandoned.NoProxy.http",
          abandoned_page ? 1 : 0, 2);
    }
  }

  UMA_HISTOGRAM_COUNTS("SiteIsolation.PageLoadsWithCrossSiteFrameAccess",
                       cross_origin_access_count_);
  UMA_HISTOGRAM_COUNTS("SiteIsolation.PageLoadsWithSameSiteFrameAccess",
                       same_origin_access_count_);

  LogNavigationState(navigation_state, main_frame->dataSource());

  navigation_state->set_load_histograms_recorded(true);

  if (RenderThread::current()) {
    RenderThread::current()->SendHistograms(
        chrome::kHistogramSynchronizerReservedSequenceNumber);
  }
}
