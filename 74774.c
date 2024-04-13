void RenderView::OnAutoFillFormDataFilled(int query_id,
                                          const webkit_glue::FormData& form) {
  autofill_helper_.FormDataFilled(query_id, form);
}
