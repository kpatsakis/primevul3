bool AreURLsInPageNavigation(const GURL& existing_url, const GURL& new_url) {
  if (existing_url == new_url || !new_url.has_ref()) {
    return false;
  }

  url_canon::Replacements<char> replacements;
  replacements.ClearRef();
  return existing_url.ReplaceComponents(replacements) ==
      new_url.ReplaceComponents(replacements);
}
