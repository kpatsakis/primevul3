V8ClassIndex::V8WrapperType V8DOMWrapper::htmlElementType(HTMLElement* element)
{
    typedef HashMap<String, V8ClassIndex::V8WrapperType> WrapperTypeMap;
    DEFINE_STATIC_LOCAL(WrapperTypeMap, wrapperTypeMap, ());
    if (wrapperTypeMap.isEmpty()) {
#define ADD_TO_HASH_MAP(tag, name) \
        wrapperTypeMap.set(#tag, V8ClassIndex::HTML##name##ELEMENT);
        FOR_EACH_TAG(ADD_TO_HASH_MAP)
#if ENABLE(VIDEO)
        if (MediaPlayer::isAvailable()) {
            FOR_EACH_VIDEO_TAG(ADD_TO_HASH_MAP)
        }
#endif
#undef ADD_TO_HASH_MAP
    }

    V8ClassIndex::V8WrapperType type = wrapperTypeMap.get(element->localName().impl());
    if (!type)
        return V8ClassIndex::HTMLELEMENT;
    return type;
}
