MagickExport const char *GetMagickDelegates(void)
{
  return ""
#if defined(MAGICKCORE_AUTOTRACE_DELEGATE)
  "autotrace "
#endif
#if defined(MAGICKCORE_BZLIB_DELEGATE)
  "bzlib "
#endif
#if defined(MAGICKCORE_CAIRO_DELEGATE)
  "cairo "
#endif
#if defined(MAGICKCORE_DJVU_DELEGATE)
  "djvu "
#endif
#if defined(MAGICKCORE_DPS_DELEGATE)
  "dps "
#endif
#if defined(MAGICKCORE_EMF_DELEGATE)
  "emf "
#endif
#if defined(MAGICKCORE_FFTW_DELEGATE)
  "fftw "
#endif
#if defined(MAGICKCORE_FONTCONFIG_DELEGATE)
  "fontconfig "
#endif
#if defined(MAGICKCORE_FREETYPE_DELEGATE)
  "freetype "
#endif
#if defined(MAGICKCORE_FPX_DELEGATE)
  "fpx "
#endif
#if defined(MAGICKCORE_GS_DELEGATE)
  "gslib "
#endif
#if defined(MAGICKCORE_GVC_DELEGATE)
  "gvc "
#endif
#if defined(MAGICKCORE_JBIG_DELEGATE)
  "jbig "
#endif
#if defined(MAGICKCORE_JPEG_DELEGATE) && defined(MAGICKCORE_PNG_DELEGATE)
  "jng "
#endif
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
  "jp2 "
#endif
#if defined(MAGICKCORE_JPEG_DELEGATE)
  "jpeg "
#endif
#if defined(MAGICKCORE_LCMS_DELEGATE)
  "lcms "
#endif
#if defined(MAGICKCORE_LQR_DELEGATE)
  "lqr "
#endif
#if defined(MAGICKCORE_LTDL_DELEGATE)
  "ltdl "
#endif
#if defined(MAGICKCORE_LZMA_DELEGATE)
  "lzma "
#endif
#if defined(MAGICKCORE_OPENEXR_DELEGATE)
  "openexr "
#endif
#if defined(MAGICKCORE_PANGOCAIRO_DELEGATE)
  "pangocairo "
#endif
#if defined(MAGICKCORE_PNG_DELEGATE)
  "png "
#endif
#if defined(MAGICKCORE_DPS_DELEGATE) || defined(MAGICKCORE_GS_DELEGATE) || defined(WIN32)
  "ps "
#endif
#if defined(MAGICKCORE_RSVG_DELEGATE)
  "rsvg "
#endif
#if defined(MAGICKCORE_TIFF_DELEGATE)
  "tiff "
#endif
#if defined(MAGICKCORE_WEBP_DELEGATE)
  "webp "
#endif
#if defined(MAGICKCORE_WMF_DELEGATE) || defined (MAGICKCORE_WMFLITE_DELEGATE)
  "wmf "
#endif
#if defined(MAGICKCORE_X11_DELEGATE)
  "x "
#endif
#if defined(MAGICKCORE_XML_DELEGATE)
  "xml "
#endif
#if defined(MAGICKCORE_ZLIB_DELEGATE)
  "zlib"
#endif
  ;
}
