int CopyOrLinkDragOperation(int drag_operation) {
  if (drag_operation & ui::DragDropTypes::DRAG_COPY)
    return ui::DragDropTypes::DRAG_COPY;
  if (drag_operation & ui::DragDropTypes::DRAG_LINK)
    return ui::DragDropTypes::DRAG_LINK;
  return ui::DragDropTypes::DRAG_NONE;
}
