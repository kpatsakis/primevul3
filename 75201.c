color_utils::HSL GdkColorToHSL(const GdkColor* color) {
  color_utils::HSL hsl;
  color_utils::SkColorToHSL(SkColorSetRGB(color->red >> 8,
                                          color->green >> 8,
                                          color->blue >> 8), &hsl);
  return hsl;
}
