static void rwpng_set_gamma(png_infop info_ptr, png_structp png_ptr, double gamma, rwpng_color_transform color)
{
    if (color != RWPNG_GAMA_ONLY && color != RWPNG_NONE) {
        png_set_gAMA(png_ptr, info_ptr, gamma);
    }
    if (color == RWPNG_SRGB) {
        png_set_sRGB(png_ptr, info_ptr, 0); // 0 = Perceptual
    }
}
