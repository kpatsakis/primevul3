void ewk_view_scroll(Evas_Object* ewkView, Evas_Coord deltaX, Evas_Coord deltaY, Evas_Coord scrollX, Evas_Coord scrollY, Evas_Coord scrollWidth, Evas_Coord scrollHeight, Evas_Coord centerX, Evas_Coord centerY, Evas_Coord centerWidth, Evas_Coord centerHeight, bool mainFrame)
{
    DBG("ewkView=%p, delta: %d,%d, scroll: %d,%d+%dx%d, clip: %d,%d+%dx%d",
        ewkView, deltaX, deltaY, scrollX, scrollY, scrollWidth, scrollHeight, centerX, centerY, centerWidth, centerHeight);

    if ((scrollX != centerX) || (scrollY != centerY) || (scrollWidth != centerWidth) || (scrollHeight != centerHeight))
        WRN("scroll region and clip are different! %d,%d+%dx%d and %d,%d+%dx%d",
            scrollX, scrollY, scrollWidth, scrollHeight, centerX, centerY, centerWidth, centerHeight);

    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    EINA_SAFETY_ON_TRUE_RETURN(!deltaX && !deltaY);

    _ewk_view_scroll_add(priv, deltaX, deltaY, scrollX, scrollY, scrollWidth, scrollHeight, mainFrame);

    _ewk_view_smart_changed(smartData);
}
