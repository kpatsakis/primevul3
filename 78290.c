void OmniboxEditModel::GetDataForURLExport(GURL* url,
                                           base::string16* title,
                                           gfx::Image* favicon) {
  *url = CurrentMatch(NULL).destination_url;
  if (*url == delegate_->GetURL()) {
    content::WebContents* web_contents = controller_->GetWebContents();
    *title = web_contents->GetTitle();
    *favicon = FaviconTabHelper::FromWebContents(web_contents)->GetFavicon();
  }
}
