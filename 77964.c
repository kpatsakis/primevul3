  string16 LocaleWindowCaptionFromPageTitle(const string16& expected_title) {
    string16 page_title = WindowCaptionFromPageTitle(expected_title);
#if defined(OS_WIN)
    std::string locale = g_browser_process->GetApplicationLocale();
    if (base::i18n::GetTextDirectionForLocale(locale.c_str()) ==
        base::i18n::RIGHT_TO_LEFT) {
      base::i18n::WrapStringWithLTRFormatting(&page_title);
    }

    return page_title;
#else
    return page_title;
#endif
  }
