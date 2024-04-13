ssize_t ewk_frame_source_get(const Evas_Object* ewkFrame, char** frameSource)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, -1);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, -1);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->document(), -1);
    EINA_SAFETY_ON_NULL_RETURN_VAL(frameSource, -1);

    WTF::String source;
    *frameSource = 0; // Saves 0 to pointer until it's not allocated.

    if (!smartData->frame->document()->isHTMLDocument()) {
        WRN("Only HTML documents are supported");
        return -1;
    }

    WebCore::Node* documentNode = smartData->frame->document()->documentElement();
    if (documentNode)
        for (WebCore::Node* node = documentNode->firstChild(); node; node = node->parentElement()) {
            if (node->hasTagName(WebCore::HTMLNames::htmlTag)) {
                WebCore::HTMLElement* element = static_cast<WebCore::HTMLElement*>(node);
                if (element)
                    source = element->outerHTML();
                break;
            }
        }

    if (source.isEmpty()) {
        if (smartData->frame->document()->head())
            source = smartData->frame->document()->head()->outerHTML();

        if (smartData->frame->document()->body())
            source += smartData->frame->document()->body()->outerHTML();
    }

    size_t sourceLength = strlen(source.utf8().data());
    *frameSource = static_cast<char*>(malloc(sourceLength + 1));
    if (!*frameSource) {
        CRITICAL("Could not allocate memory.");
        return -1;
    }

    strncpy(*frameSource, source.utf8().data(), sourceLength);
    (*frameSource)[sourceLength] = '\0';

    return sourceLength;
}
