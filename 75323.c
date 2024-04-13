bool PrintMsg_Print_Params_IsEmpty(const PrintMsg_Print_Params& params) {
  return !params.document_cookie && !params.desired_dpi && !params.max_shrink &&
         !params.min_shrink && !params.dpi && params.printable_size.IsEmpty() &&
         !params.selection_only && params.page_size.IsEmpty() &&
         !params.margin_top && !params.margin_left &&
         !params.supports_alpha_blend;
}
