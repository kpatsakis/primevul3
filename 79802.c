void ChromeDownloadManagerDelegate::OpenWithWebIntent(
    const DownloadItem* item) {
  webkit_glue::WebIntentData intent_data(
      ASCIIToUTF16("http://webintents.org/view"),
      ASCIIToUTF16(item->GetMimeType()),
      item->GetFullPath(),
      item->GetReceivedBytes());

  intent_data.extra_data.insert(make_pair(
      ASCIIToUTF16("url"), ASCIIToUTF16(item->GetURL().spec())));

  intent_data.extra_data.insert(
      make_pair(ASCIIToUTF16("filename"),
                item->GetFileNameToReportUser().LossyDisplayName()));

  content::WebIntentsDispatcher* dispatcher =
      content::WebIntentsDispatcher::Create(intent_data);
  dispatcher->RegisterReplyNotification(
      base::Bind(&OnWebIntentDispatchCompleted, item->GetFullPath()));

  content::WebContentsDelegate* delegate = NULL;
#if !defined(OS_ANDROID)
  if (item->GetWebContents() && item->GetWebContents()->GetDelegate()) {
    delegate = item->GetWebContents()->GetDelegate();
  } else {
    delegate = web_intents::GetBrowserForBackgroundWebIntentDelivery(profile_);
  }
#endif  // !defined(OS_ANDROID)
  DCHECK(delegate);
  delegate->WebIntentDispatch(NULL, dispatcher);
}
