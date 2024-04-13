void ewk_frame_view_create_for_view(Evas_Object* ewkFrame, Evas_Object* view)
{
    DBG("ewkFrame=%p, view=%p", ewkFrame, view);
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->frame);
    Evas_Coord width, height;

    evas_object_geometry_get(view, 0, 0, &width, &height);

    WebCore::IntSize size(width, height);
    int red, green, blue, alpha;
    WebCore::Color background;

    ewk_view_bg_color_get(view, &red, &green, &blue, &alpha);
    if (!alpha)
        background = WebCore::Color(0, 0, 0, 0);
    else if (alpha == 255)
        background = WebCore::Color(red, green, blue, alpha);
    else
        background = WebCore::Color(red * 255 / alpha, green * 255 / alpha, blue * 255 / alpha, alpha);

    smartData->frame->createView(size, background, !alpha, WebCore::IntSize(), false);
    if (!smartData->frame->view())
        return;

    const char* theme = ewk_view_theme_get(view);
    smartData->frame->view()->setEdjeTheme(theme);
    smartData->frame->view()->setEvasObject(ewkFrame);

    ewk_frame_mixed_content_displayed_set(ewkFrame, false);
    ewk_frame_mixed_content_run_set(ewkFrame, false);
}
