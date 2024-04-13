    GetAlternativeWebContentsToNotifyForDownload() {
#if defined(OS_ANDROID)
  return NULL;
#else
  Browser* last_active = chrome::FindLastActiveWithProfile(profile_,
      chrome::GetActiveDesktop());
  return last_active ? chrome::GetActiveWebContents(last_active) : NULL;
#endif
}
