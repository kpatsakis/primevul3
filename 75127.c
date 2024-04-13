static void IPv6SupportResults(IPv6SupportStatus result) {
  static bool run_once = false;
  if (run_once)
    return;
  run_once = true;
  UMA_HISTOGRAM_ENUMERATION("Net.IPv6Status", result, IPV6_SUPPORT_MAX);
}
