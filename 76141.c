void ewk_view_bg_color_set(Evas_Object* ewkView, int red, int green, int blue, int alpha)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->bg_color_set);

    if (alpha < 0) {
        WRN("Alpha less than zero (%d).", alpha);
        alpha = 0;
    } else if (alpha > 255) {
        WRN("Alpha is larger than 255 (%d).", alpha);
        alpha = 255;
    }

#define CHECK_PREMUL_COLOR(color, alpha)                                        \
    if (color < 0) {                                                        \
        WRN("Color component " #color " is less than zero (%d).", color);         \
        color = 0;                                                          \
    } else if (color > alpha) {                                                 \
        WRN("Color component " #color " is greater than alpha (%d, alpha=%d).", \
            color, alpha);                                                      \
        color = alpha;                                                          \
    }
    CHECK_PREMUL_COLOR(red, alpha);
    CHECK_PREMUL_COLOR(green, alpha);
    CHECK_PREMUL_COLOR(blue, alpha);
#undef CHECK_PREMUL_COLOR

    smartData->bg_color.r = red;
    smartData->bg_color.g = green;
    smartData->bg_color.b = blue;
    smartData->bg_color.a = alpha;

    smartData->api->bg_color_set(smartData, red, green, blue, alpha);

    WebCore::FrameView* view = smartData->_priv->mainFrame->view();
    if (view) {
        WebCore::Color color;

        if (!alpha)
            color = WebCore::Color(0, 0, 0, 0);
        else if (alpha == 255)
            color = WebCore::Color(red, green, blue, alpha);
        else
            color = WebCore::Color(red * 255 / alpha, green * 255 / alpha, blue * 255 / alpha, alpha);

        view->updateBackgroundRecursively(color, !alpha);
    }
}
