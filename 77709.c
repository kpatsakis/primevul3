void BookmarkManagerView::Show(Profile* profile) {
  if (!profile->GetBookmarkModel())
    return;

  if (open_window != NULL) {
    open_window->Activate();
    return;
  }

  manager = new BookmarkManagerView(profile);

  open_window = views::Window::CreateChromeWindow(NULL, gfx::Rect(), manager);
  manager->PrepareForShow();
  open_window->Show();

  manager->search_tf_->RequestFocus();
}
