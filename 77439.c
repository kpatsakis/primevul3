Eina_Bool ewk_frame_contents_alternate_set(Evas_Object* ewkFrame, const char* contents, size_t contentsSize, const char* mimeType, const char* encoding, const char* baseUri, const char* unreachableUri)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(contents, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(unreachableUri, false);
    return _ewk_frame_contents_set_internal
               (smartData, contents, contentsSize, mimeType, encoding, baseUri,
               unreachableUri);
}
