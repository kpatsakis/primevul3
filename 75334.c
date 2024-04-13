void PrepareFrameAndViewForPrint::StartPrinting(
    const gfx::Size& print_canvas_size) {
  print_canvas_size_ = print_canvas_size;

  gfx::Size print_layout_size(print_canvas_size_);
  print_layout_size.set_height(static_cast<int>(
      static_cast<double>(print_layout_size.height()) * 1.25));

  web_view_->resize(print_layout_size);

  expected_pages_count_ = frame_->printBegin(print_canvas_size_, node_to_print_,
                                             dpi_, &use_browser_overlays_);
}
