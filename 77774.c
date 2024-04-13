void LogOneClickHistogramValue(int action) {
  UMA_HISTOGRAM_ENUMERATION("Signin.OneClickActions", action,
                            one_click_signin::HISTOGRAM_MAX);
  UMA_HISTOGRAM_ENUMERATION("Signin.AllAccessPointActions", action,
                            one_click_signin::HISTOGRAM_MAX);
}
