static float lite_font_stringwidth( wmfAPI* API, wmfFont* font, char* str)
{
#if 0
  wmf_magick_t
    *ddata = WMF_MAGICK_GetData(API);

  Image
    *image = ddata->image;

  DrawInfo
    *draw_info;

  TypeMetric
    metrics;

  float
    stringwidth = 0;

  double
    orig_x_resolution,
    orig_y_resolution;

  ResolutionType
    orig_resolution_units;

  orig_x_resolution = image->x_resolution;
  orig_y_resolution = image->y_resolution;
  orig_resolution_units = image->units;

  draw_info=ddata->draw_info;
  if (draw_info == (const DrawInfo *) NULL)
    return 0;

  draw_info->font=WMF_FONT_PSNAME(font);
  draw_info->pointsize=12;
  draw_info->text=str;

  image->x_resolution = 72;
  image->y_resolution = 72;
  image->units = PixelsPerInchResolution;

  if (GetTypeMetrics(image, draw_info, &metrics) != MagickFalse)
    stringwidth = ((metrics.width * 72)/(image->x_resolution * draw_info->pointsize)); /* *0.916348; */

  draw_info->font=NULL;
  draw_info->text=NULL;

#if 0
  printf("\nlite_font_stringwidth\n");
  printf("string                  = \"%s\"\n", str);
  printf("WMF_FONT_NAME           = \"%s\"\n", WMF_FONT_NAME(font));
  printf("WMF_FONT_PSNAME         = \"%s\"\n", WMF_FONT_PSNAME(font));
  printf("stringwidth             = %g\n", stringwidth);
  /* printf("WMF_FONT_HEIGHT         = %i\n", (int)WMF_FONT_HEIGHT(font)); */
  /* printf("WMF_FONT_WIDTH          = %i\n", (int)WMF_FONT_WIDTH(font)); */
  fflush(stdout);
#endif

  image->x_resolution = orig_x_resolution;
  image->y_resolution = orig_y_resolution;
  image->units = orig_resolution_units;

  return stringwidth;
#else
  (void) API;
  (void) font;
  (void) str;

  return 0;
#endif
}
