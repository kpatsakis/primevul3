static std::string TempFileName() {
#if defined(OS_MACOSX)
  return StringPrintf(".%s.XXXXXX", base::mac::BaseBundleID());
#endif

#if defined(GOOGLE_CHROME_BUILD)
  return std::string(".com.google.Chrome.XXXXXX");
#else
  return std::string(".org.chromium.Chromium.XXXXXX");
#endif
}
