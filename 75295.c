SkPoint GetHeaderFooterPosition(
    float webkit_scale_factor,
    const PageSizeMargins& page_layout,
    printing::HorizontalHeaderFooterPosition horizontal_position,
    printing::VerticalHeaderFooterPosition vertical_position,
    double offset_to_baseline,
    double text_width_in_points) {
  SkScalar x = 0;
  switch (horizontal_position) {
    case printing::LEFT: {
      x = printing::kSettingHeaderFooterInterstice - page_layout.margin_left;
      break;
    }
    case printing::RIGHT: {
      x = page_layout.content_width + page_layout.margin_right -
          printing::kSettingHeaderFooterInterstice - text_width_in_points;
      break;
    }
    case printing::CENTER: {
      SkScalar available_width = GetHeaderFooterSegmentWidth(
          page_layout.margin_left + page_layout.margin_right +
              page_layout.content_width);
      x = available_width - page_layout.margin_left +
          (available_width - text_width_in_points) / 2;
      break;
    }
    default: {
      NOTREACHED();
    }
  }

  SkScalar y = 0;
  switch (vertical_position) {
    case printing::TOP:
      y = printing::kSettingHeaderFooterInterstice -
          page_layout.margin_top - offset_to_baseline;
      break;
    case printing::BOTTOM:
      y = page_layout.margin_bottom + page_layout.content_height -
          printing::kSettingHeaderFooterInterstice - offset_to_baseline;
      break;
    default:
      NOTREACHED();
  }

  SkPoint point = SkPoint::Make(x / webkit_scale_factor,
                                y / webkit_scale_factor);
  return point;
}
