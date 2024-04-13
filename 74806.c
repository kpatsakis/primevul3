void RenderView::OnGetSerializedHtmlDataForCurrentPageWithLocalLinks(
    const std::vector<GURL>& links,
    const std::vector<FilePath>& local_paths,
    const FilePath& local_directory_name) {

  WebVector<WebURL> weburl_links(links);

  WebVector<WebString> webstring_paths(local_paths.size());
  for (size_t i = 0; i < local_paths.size(); i++)
    webstring_paths[i] = webkit_glue::FilePathToWebString(local_paths[i]);

  WebPageSerializer::serialize(webview()->mainFrame(),
                               true, this, weburl_links, webstring_paths,
                               webkit_glue::FilePathToWebString(
                                   local_directory_name));
}
