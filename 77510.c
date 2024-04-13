void ewk_frame_title_set(Evas_Object* ewkFrame, const char* title)
{
    DBG("ewkFrame=%p, title=%s", ewkFrame, title ? title : "(null)");
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    if (!eina_stringshare_replace(&smartData->title, title))
        return;
    evas_object_smart_callback_call(ewkFrame, "title,changed", (void*)smartData->title);
}
