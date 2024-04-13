Eina_Bool ewk_view_text_matches_unmark_all(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    priv->page->unmarkAllTextMatches();
    return true;
}
