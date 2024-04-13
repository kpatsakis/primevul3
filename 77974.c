string16 WindowCaptionFromPageTitle(const string16& page_title) {
#if defined(OS_MACOSX) || defined(OS_CHROMEOS)
  if (page_title.empty())
    return l10n_util::GetStringUTF16(IDS_BROWSER_WINDOW_MAC_TAB_UNTITLED);
  return page_title;
#else
  if (page_title.empty())
    return l10n_util::GetStringUTF16(IDS_PRODUCT_NAME);

  return l10n_util::GetStringFUTF16(IDS_BROWSER_WINDOW_TITLE_FORMAT,
                                    page_title);
#endif
}
