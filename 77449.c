Eina_Bool ewk_frame_feed_key_up(Evas_Object* ewkFrame, const Evas_Event_Key_Up* upEvent)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(upEvent, false);

    DBG("ewkFrame=%p keyname=%s (key=%s, string=%s)",
        ewkFrame, upEvent->keyname, upEvent->key ? upEvent->key : "", upEvent->string ? upEvent->string : "");

    WebCore::PlatformKeyboardEvent event(upEvent);
    return smartData->frame->eventHandler()->keyEvent(event);
}
