void ewk_view_repaint(Evas_Object* ewkView, Evas_Coord x, Evas_Coord y, Evas_Coord width, Evas_Coord height)
{
    DBG("ewkView=%p, region=%d,%d + %dx%d", ewkView, x, y, width, height);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);

    _ewk_view_repaint_add(priv, x, y, width, height);
    _ewk_view_smart_changed(smartData);
}
