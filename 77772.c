void OneClickSigninHelper::LogConfirmHistogramValue(int action) {
  UMA_HISTOGRAM_ENUMERATION("Signin.OneClickConfirmation", action,
                            one_click_signin::HISTOGRAM_CONFIRM_MAX);
}
