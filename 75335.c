void PrepareFrameAndViewForPrint::UpdatePrintParams(
    const PrintMsg_Print_Params& print_params) {
  DCHECK(!finished_);
  gfx::Size canvas_size;
  CalculatePrintCanvasSize(print_params, &canvas_size);
  if (canvas_size == print_canvas_size_)
    return;

  frame_->printEnd();
  dpi_ = static_cast<int>(print_params.dpi);
  StartPrinting(canvas_size);
}
