static void rwpng_warning_stderr_handler(png_structp png_ptr, png_const_charp msg) {
    fprintf(stderr, "  libpng warning: %s\n", msg);
}
