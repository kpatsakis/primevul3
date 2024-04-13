void ewk_view_repaint_add(Ewk_View_Private_Data* priv, Evas_Coord x, Evas_Coord y, Evas_Coord width, Evas_Coord height)
{
    EINA_SAFETY_ON_NULL_RETURN(priv);
    _ewk_view_repaint_add(priv, x, y, width, height);
}
