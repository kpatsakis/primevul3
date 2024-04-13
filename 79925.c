base::Time FileEnumerator::GetLastModifiedTime(const FindInfo& find_info) {
  return base::Time::FromTimeT(find_info.stat.st_mtime);
}
