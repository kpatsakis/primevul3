void rwpng_version_info(FILE *fp)
{
    const char *pngver = png_get_header_ver(NULL);

#if USE_COCOA
    fprintf(fp, "   Color profiles are supported via Cocoa. Using libpng %s.\n", pngver);
#elif USE_LCMS
    fprintf(fp, "   Color profiles are supported via Little CMS. Using libpng %s.\n", pngver);
#else
    fprintf(fp, "   Compiled with no support for color profiles. Using libpng %s.\n", pngver);
#endif

#if PNG_LIBPNG_VER < 10600
    if (strcmp(pngver, "1.3.") < 0) {
        fputs("\nWARNING: Your version of libpng is outdated and may produce corrupted files.\n"
              "Please recompile pngquant with the current version of libpng (1.6 or later).\n", fp);
    } else if (strcmp(pngver, "1.6.") < 0) {
        #if defined(PNG_UNKNOWN_CHUNKS_SUPPORTED)
        fputs("\nWARNING: Your version of libpng is old and has buggy support for custom chunks.\n"
              "Please recompile pngquant with the current version of libpng (1.6 or later).\n", fp);
        #endif
    }
#endif
}
