void RenderView::didSerializeDataForFrame(const WebURL& frame_url,
    const WebCString& data,
    WebPageSerializerClient::PageSerializationStatus status) {
  Send(new ViewHostMsg_SendSerializedHtmlData(
    routing_id_,
    frame_url,
    data.data(),
    static_cast<int32>(status)));
}
