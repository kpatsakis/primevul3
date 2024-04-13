bool SavePackage::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(SavePackage, message)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SendCurrentPageAllSavableResourceLinks,
                        OnReceivedSavableResourceLinksForCurrentPage)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SendSerializedHtmlData,
                        OnReceivedSerializedHtmlData)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}
