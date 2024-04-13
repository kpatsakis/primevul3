GdkColor PickLuminosityContrastingColor(const GdkColor* base,
                                        const GdkColor* one,
                                        const GdkColor* two) {
  color_utils::HSL baseHSL = GdkColorToHSL(base);
  color_utils::HSL oneHSL = GdkColorToHSL(one);
  color_utils::HSL twoHSL = GdkColorToHSL(two);
  double one_difference = fabs(baseHSL.l - oneHSL.l);
  double two_difference = fabs(baseHSL.l - twoHSL.l);

  if (two_difference > one_difference + 0.1)
    return *two;
  else
    return *one;
}
