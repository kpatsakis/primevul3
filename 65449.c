static const char *cache_id(const char *id)
{
  static char clean[SHORT_STRING];
  mutt_str_strfcpy(clean, id, sizeof(clean));
  mutt_file_sanitize_filename(clean, true);
  return clean;
}
