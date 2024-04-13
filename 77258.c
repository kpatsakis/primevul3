void OmniboxViewWin::EraseTopOfSelection(CDC* dc,
                                         const CRect& client_rect,
                                         const CRect& paint_clip_rect) {
  CRect erase_rect(client_rect.left, client_rect.top, client_rect.right,
                   client_rect.top + font_y_adjustment_);
  erase_rect.IntersectRect(erase_rect, paint_clip_rect);

  if (!erase_rect.IsRectNull())
    dc->FillSolidRect(&erase_rect, background_color_);
}
