void RenderView::OnGetAllSavableResourceLinksForCurrentPage(
    const GURL& page_url) {
  std::vector<GURL> resources_list;
  std::vector<GURL> referrers_list;
  std::vector<GURL> frames_list;
  webkit_glue::SavableResourcesResult result(&resources_list,
                                             &referrers_list,
                                             &frames_list);

  if (!webkit_glue::GetAllSavableResourceLinksForCurrentPage(
          webview(),
          page_url,
          &result,
          chrome::kSavableSchemes)) {
    referrers_list.clear();
    resources_list.clear();
    frames_list.clear();
  }

  Send(new ViewHostMsg_SendCurrentPageAllSavableResourceLinks(routing_id_,
                                                              resources_list,
                                                              referrers_list,
                                                              frames_list));
}
