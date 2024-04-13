Ewk_Security_Origin* ewk_frame_security_origin_get(const Evas_Object *ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->document(), 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->document()->securityOrigin(), 0);

    return ewk_security_origin_new(smartData->frame->document()->securityOrigin());
}
