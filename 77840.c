NOINLINE static void MaybeTriggerAsanError(const GURL& url) {
  static const char kCrashDomain[] = "crash";
  static const char kHeapOverflow[] = "/heap-overflow";
  static const char kHeapUnderflow[] = "/heap-underflow";
  static const char kUseAfterFree[] = "/use-after-free";
  static const int kArraySize = 5;

  if (!url.DomainIs(kCrashDomain, sizeof(kCrashDomain) - 1))
    return;

  if (!url.has_path())
    return;

  scoped_ptr<int[]> array(new int[kArraySize]);
  std::string crash_type(url.path());
  int dummy = 0;
  if (crash_type == kHeapOverflow) {
    dummy = array[kArraySize];
  } else if (crash_type == kHeapUnderflow ) {
    dummy = array[-1];
  } else if (crash_type == kUseAfterFree) {
    int* dangling = array.get();
    array.reset();
    dummy = dangling[kArraySize / 2];
  }

  base::debug::Alias(&dummy);
}
