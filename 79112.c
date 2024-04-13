  void CloseInspectedTab() {
    browser()->tab_strip_model()->CloseWebContentsAt(0,
        TabStripModel::CLOSE_NONE);
  }
