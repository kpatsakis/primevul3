void BookmarkManagerView::OnKeyDown(unsigned short virtual_keycode) {
  switch (virtual_keycode) {
    case VK_RETURN: {
      std::vector<BookmarkNode*> selected_nodes = GetSelectedTableNodes();
      if (selected_nodes.size() == 1 && selected_nodes[0]->is_folder()) {
        SelectInTree(selected_nodes[0]);
      } else {
        bookmark_utils::OpenAll(
            GetWidget()->GetNativeView(), profile_, NULL, selected_nodes,
            CURRENT_TAB);
      }
      break;
    }

    case VK_BACK: {
      BookmarkNode* selected_folder = GetSelectedFolder();
      if (selected_folder != NULL &&
          selected_folder->GetParent() != GetBookmarkModel()->root_node()) {
        SelectInTree(selected_folder->GetParent());
      }
      break;
    }

    default:
      OnCutCopyPaste(KeyCodeToCutCopyPaste(virtual_keycode), true);
      break;
  }
}
