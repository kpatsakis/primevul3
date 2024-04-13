void ewk_frame_load_finished(Evas_Object* ewkFrame, const char* errorDomain, int errorCode, bool isCancellation, const char* errorDescription, const char* failingUrl)
{
    Ewk_Frame_Load_Error buffer, *error;
    if (!errorDomain) {
        DBG("ewkFrame=%p, success.", ewkFrame);
        error = 0;
    } else {
        DBG("ewkFrame=%p, error=%s (%d, cancellation=%hhu) \"%s\", url=%s",
            ewkFrame, errorDomain, errorCode, isCancellation,
            errorDescription, failingUrl);

        buffer.domain = errorDomain;
        buffer.code = errorCode;
        buffer.is_cancellation = isCancellation;
        buffer.description = errorDescription;
        buffer.failing_url = failingUrl;
        buffer.resource_identifier = 0;
        buffer.frame = ewkFrame;
        error = &buffer;
    }
    evas_object_smart_callback_call(ewkFrame, "load,finished", error);
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    ewk_view_load_finished(smartData->view, error);
}
