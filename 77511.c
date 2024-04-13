bool ewk_frame_uri_changed(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    WTF::CString uri(smartData->frame->document()->url().string().utf8());

    INF("uri=%s", uri.data());
    if (!uri.data()) {
        ERR("no uri");
        return false;
    }

    eina_stringshare_replace(&smartData->uri, uri.data());
    evas_object_smart_callback_call(ewkFrame, "uri,changed", (void*)smartData->uri);
    return true;
}
