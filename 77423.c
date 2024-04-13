static Eina_Bool _ewk_frame_contents_set_internal(Ewk_Frame_Smart_Data* smartData, const char* contents, size_t contentsSize, const char* mimeType, const char* encoding, const char* baseUri, const char* unreachableUri)
{
    size_t length = strlen(contents);
    if (contentsSize < 1 || contentsSize > length)
        contentsSize = length;
    if (!mimeType)
        mimeType = "text/html";
    if (!encoding)
        encoding = "UTF-8";
    if (!baseUri)
        baseUri = "about:blank";

    WebCore::KURL baseKURL(WebCore::KURL(), WTF::String::fromUTF8(baseUri));
    WebCore::KURL unreachableKURL;
    if (unreachableUri)
        unreachableKURL = WebCore::KURL(WebCore::KURL(), WTF::String::fromUTF8(unreachableUri));
    else
        unreachableKURL = WebCore::KURL();

    WTF::RefPtr<WebCore::SharedBuffer> buffer = WebCore::SharedBuffer::create(contents, contentsSize);
    WebCore::SubstituteData substituteData
        (buffer.release(),
        WTF::String::fromUTF8(mimeType),
        WTF::String::fromUTF8(encoding),
        baseKURL, unreachableKURL);
    WebCore::ResourceRequest request(baseKURL);

    smartData->frame->loader()->load(request, substituteData, false);
    return true;
}
