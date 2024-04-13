read_colormap(bmp_source_ptr sinfo, int cmaplen, int mapentrysize)
/* Read the colormap from a BMP file */
{
  int i, gray = 1;

  switch (mapentrysize) {
  case 3:
    /* BGR format (occurs in OS/2 files) */
    for (i = 0; i < cmaplen; i++) {
      sinfo->colormap[2][i] = (JSAMPLE)read_byte(sinfo);
      sinfo->colormap[1][i] = (JSAMPLE)read_byte(sinfo);
      sinfo->colormap[0][i] = (JSAMPLE)read_byte(sinfo);
      if (sinfo->colormap[2][i] != sinfo->colormap[1][i] ||
          sinfo->colormap[1][i] != sinfo->colormap[0][i])
        gray = 0;
    }
    break;
  case 4:
    /* BGR0 format (occurs in MS Windows files) */
    for (i = 0; i < cmaplen; i++) {
      sinfo->colormap[2][i] = (JSAMPLE)read_byte(sinfo);
      sinfo->colormap[1][i] = (JSAMPLE)read_byte(sinfo);
      sinfo->colormap[0][i] = (JSAMPLE)read_byte(sinfo);
      (void)read_byte(sinfo);
      if (sinfo->colormap[2][i] != sinfo->colormap[1][i] ||
          sinfo->colormap[1][i] != sinfo->colormap[0][i])
        gray = 0;
    }
    break;
  default:
    ERREXIT(sinfo->cinfo, JERR_BMP_BADCMAP);
    break;
  }

  if (sinfo->cinfo->in_color_space == JCS_UNKNOWN && gray)
    sinfo->cinfo->in_color_space = JCS_GRAYSCALE;

  if (sinfo->cinfo->in_color_space == JCS_GRAYSCALE && !gray)
    ERREXIT(sinfo->cinfo, JERR_BAD_IN_COLORSPACE);
}
