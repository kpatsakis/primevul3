  void RunTest(const FilePath& url, GpuResultFlags expectations) {
    using trace_analyzer::Query;
    using trace_analyzer::TraceAnalyzer;

    ASSERT_TRUE(tracing::BeginTracing("test_gpu"));

    RunTest(url, NULL, true);

    std::string json_events;
    ASSERT_TRUE(tracing::EndTracing(&json_events));

    scoped_ptr<TraceAnalyzer> analyzer(TraceAnalyzer::Create(json_events));
    analyzer->AssociateBeginEndEvents();
    trace_analyzer::TraceEventVector events;

    if (expectations & EXPECT_NO_GPU_PROCESS) {
      EXPECT_EQ(0u, analyzer->FindEvents(
          Query::MatchBeginName("OnGraphicsInfoCollected"), &events));
    }

    if (expectations & EXPECT_GPU_SWAP_BUFFERS) {
      EXPECT_GT(analyzer->FindEvents(Query::EventName() ==
                                     Query::String("SwapBuffers"), &events),
                size_t(0));
    }
  }
