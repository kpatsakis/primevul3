void WebContentsImpl::ClearAllPowerSaveBlockers() {
  for (PowerSaveBlockerMap::iterator i(power_save_blockers_.begin());
       i != power_save_blockers_.end(); ++i)
    STLDeleteValues(&power_save_blockers_[i->first]);
  power_save_blockers_.clear();
}
