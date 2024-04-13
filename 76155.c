const char* ewk_view_font_family_name_get(const Evas_Object* ewkView, Ewk_Font_Family fontFamily)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, 0);

    switch (fontFamily) {
    case EWK_FONT_FAMILY_STANDARD:
        return priv->settings.fontStandard;
    case EWK_FONT_FAMILY_CURSIVE:
        return priv->settings.fontCursive;
    case EWK_FONT_FAMILY_FANTASY:
        return priv->settings.fontFantasy;
    case EWK_FONT_FAMILY_MONOSPACE:
        return priv->settings.fontMonospace;
    case EWK_FONT_FAMILY_SERIF:
        return priv->settings.fontSerif;
    case EWK_FONT_FAMILY_SANS_SERIF:
        return priv->settings.fontSansSerif;
    }
    return 0;
}
