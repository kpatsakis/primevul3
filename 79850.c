void SavePackage::GetAllSavableResourceLinksForCurrentPage() {
  if (wait_state_ != START_PROCESS)
    return;

  wait_state_ = RESOURCES_LIST;
  Send(new ViewMsg_GetAllSavableResourceLinksForCurrentPage(routing_id(),
                                                            page_url_));
}
