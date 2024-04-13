Ewk_Certificate_Status ewk_frame_certificate_status_get(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, EWK_CERTIFICATE_STATUS_NO_CERTIFICATE);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, EWK_CERTIFICATE_STATUS_NO_CERTIFICATE);

    const WebCore::FrameLoader* frameLoader = smartData->frame->loader();
    const WebCore::DocumentLoader* documentLoader = frameLoader->documentLoader();
    const WebCore::KURL documentURL = documentLoader->requestURL();

    if (!documentURL.protocolIs("https"))
        return EWK_CERTIFICATE_STATUS_NO_CERTIFICATE;

    if (frameLoader->subframeIsLoading())
        return EWK_CERTIFICATE_STATUS_NO_CERTIFICATE;

    SoupMessage* soupMessage = documentLoader->request().toSoupMessage();

    if (soupMessage && (soup_message_get_flags(soupMessage) & SOUP_MESSAGE_CERTIFICATE_TRUSTED))
        return EWK_CERTIFICATE_STATUS_TRUSTED;

    return EWK_CERTIFICATE_STATUS_UNTRUSTED;
}
