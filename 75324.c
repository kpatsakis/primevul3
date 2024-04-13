bool PrintMsg_Print_Params_IsEqual(
    const PrintMsg_PrintPages_Params& oldParams,
    const PrintMsg_PrintPages_Params& newParams) {
  return PageLayoutIsEqual(oldParams, newParams) &&
         oldParams.params.max_shrink == newParams.params.max_shrink &&
         oldParams.params.min_shrink == newParams.params.min_shrink &&
         oldParams.params.selection_only == newParams.params.selection_only &&
         oldParams.params.supports_alpha_blend ==
             newParams.params.supports_alpha_blend &&
         oldParams.pages.size() == newParams.pages.size() &&
         oldParams.params.display_header_footer ==
             newParams.params.display_header_footer &&
         oldParams.params.date == newParams.params.date &&
         oldParams.params.title == newParams.params.title &&
         oldParams.params.url == newParams.params.url &&
         std::equal(oldParams.pages.begin(), oldParams.pages.end(),
             newParams.pages.begin());
}
