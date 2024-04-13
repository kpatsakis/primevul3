void ewk_view_bg_color_get(const Evas_Object* ewkView, int* red, int* green, int* blue, int* alpha)
{
    if (red)
        *red = 0;
    if (green)
        *green = 0;
    if (blue)
        *blue = 0;
    if (alpha)
        *alpha = 0;
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    if (red)
        *red = smartData->bg_color.r;
    if (green)
        *green = smartData->bg_color.g;
    if (blue)
        *blue = smartData->bg_color.b;
    if (alpha)
        *alpha = smartData->bg_color.a;
}
