OmniboxViewWin::EditDropTarget::EditDropTarget(OmniboxViewWin* edit)
    : ui::DropTarget(edit->m_hWnd),
      edit_(edit),
      drag_has_url_(false),
      drag_has_string_(false) {
}
