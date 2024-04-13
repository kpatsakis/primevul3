void CalculatePrintCanvasSize(const PrintMsg_Print_Params& print_params,
                              gfx::Size* result) {
  int dpi = GetDPI(&print_params);
  result->set_width(ConvertUnit(print_params.printable_size.width(), dpi,
                                print_params.desired_dpi));

  result->set_height(ConvertUnit(print_params.printable_size.height(), dpi,
                                 print_params.desired_dpi));
}
