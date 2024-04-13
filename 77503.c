Eina_Bool ewk_frame_text_matches_nth_pos_get(const Evas_Object* ewkFrame, size_t number, int* x, int* y)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);

    Vector<WebCore::IntRect> intRects = smartData->frame->document()->markers()->renderedRectsForMarkers(WebCore::DocumentMarker::TextMatch);

    /* remove useless values */
    std::remove_if(intRects.begin(), intRects.end(), _ewk_frame_rect_is_negative_value);

    if (intRects.isEmpty() || number > intRects.size())
        return false;

    std::sort(intRects.begin(), intRects.end(), _ewk_frame_rect_cmp_less_than);

    if (x)
        *x = intRects[number - 1].x();
    if (y)
        *y = intRects[number - 1].y();
    return true;
}
