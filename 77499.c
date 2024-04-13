Eina_Bool ewk_frame_stop(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    smartData->frame->loader()->stopAllLoaders();
    return true;
}
