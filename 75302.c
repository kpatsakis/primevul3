void PrintWebViewHelper::IncrementScriptedPrintCount() {
  ++user_cancelled_scripted_print_count_;
  last_cancelled_script_print_ = base::Time::Now();
}
