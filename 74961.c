void RenderView::startDragging(const WebDragData& data,
                               WebDragOperationsMask mask,
                               const WebImage& image,
                               const WebPoint& imageOffset) {
#if WEBKIT_USING_SKIA
  SkBitmap bitmap(image.getSkBitmap());
#elif WEBKIT_USING_CG
  SkBitmap bitmap = gfx::CGImageToSkBitmap(image.getCGImageRef());
#endif

  Send(new ViewHostMsg_StartDragging(routing_id_,
                                     WebDropData(data),
                                     mask,
                                     bitmap,
                                     imageOffset));
}
