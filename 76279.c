Eina_Bool ewk_view_text_search(const Evas_Object* ewkView, const char* string, Eina_Bool caseSensitive, Eina_Bool forward, Eina_Bool wrap)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(string, false);
    WTF::TextCaseSensitivity sensitive;
    WebCore::FindDirection direction;

    if (caseSensitive)
        sensitive = WTF::TextCaseSensitive;
    else
        sensitive = WTF::TextCaseInsensitive;

    if (forward)
        direction = WebCore::FindDirectionForward;
    else
        direction = WebCore::FindDirectionBackward;

    return priv->page->findString(String::fromUTF8(string), sensitive, direction, wrap);
}
