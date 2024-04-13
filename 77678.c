void BookmarkManagerView::ContentsChanged(views::TextField* sender,
                                          const std::wstring& new_contents) {
  search_factory_.RevokeAll();
  MessageLoop::current()->PostDelayedTask(FROM_HERE,
      search_factory_.NewRunnableMethod(&BookmarkManagerView::PerformSearch),
      kSearchDelayMS);
}
