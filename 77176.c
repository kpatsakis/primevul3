  void SetupBlacklist(const std::string& json_blacklist) {
    GpuBlacklist* blacklist = GpuBlacklist::GetInstance();
    ASSERT_TRUE(blacklist->LoadGpuBlacklist(
        "1.0", json_blacklist, GpuBlacklist::kAllOs));
    blacklist->UpdateGpuDataManager();
  }
