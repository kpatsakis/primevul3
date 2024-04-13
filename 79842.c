int64 SavePackage::CurrentSpeed() const {
  base::TimeDelta diff = base::TimeTicks::Now() - start_tick_;
  int64 diff_ms = diff.InMilliseconds();
  return diff_ms == 0 ? 0 : completed_count() * 1000 / diff_ms;
}
