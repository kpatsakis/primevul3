static inline bool hackAroundLibXMLEntityBug(void* closure)
{
#if LIBXML_VERSION >= 20627
    UNUSED_PARAM(closure);

    return false;
#else
    return static_cast<xmlParserCtxtPtr>(closure)->node;
#endif
}
