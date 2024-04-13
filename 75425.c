static void ExtentToStringSet(const URLPatternSet& host_extent,
                              std::set<std::string>* result) {
  URLPatternList patterns = host_extent.patterns();
  URLPatternList::const_iterator i;

  for (i = patterns.begin(); i != patterns.end(); ++i)
    result->insert(i->GetAsString());
}
