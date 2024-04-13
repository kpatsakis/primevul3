bool PageLayoutIsEqual(const PrintMsg_PrintPages_Params& oldParams,
                       const PrintMsg_PrintPages_Params& newParams) {
  return oldParams.params.printable_size == newParams.params.printable_size &&
         oldParams.params.page_size == newParams.params.page_size &&
         oldParams.params.margin_top == newParams.params.margin_top &&
         oldParams.params.margin_left == newParams.params.margin_left &&
         oldParams.params.desired_dpi == newParams.params.desired_dpi &&
         oldParams.params.dpi == newParams.params.dpi;
}
