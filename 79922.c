int64 FileEnumerator::GetFilesize(const FindInfo& find_info) {
  return find_info.stat.st_size;
}
