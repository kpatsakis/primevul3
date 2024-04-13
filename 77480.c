char* ewk_frame_plain_text_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);

    if (!smartData->frame->document())
        return 0;

    WebCore::Element* documentElement = smartData->frame->document()->documentElement();

    if (!documentElement)
        return 0;

    return strdup(documentElement->innerText().utf8().data());
}
