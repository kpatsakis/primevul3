V8ClassIndex::V8WrapperType V8DOMWrapper::svgElementType(SVGElement* element)
{
    typedef HashMap<String, V8ClassIndex::V8WrapperType> WrapperTypeMap;
    DEFINE_STATIC_LOCAL(WrapperTypeMap, wrapperTypeMap, ());
    if (wrapperTypeMap.isEmpty()) {
#define ADD_TO_HASH_MAP(tag, name) \
        wrapperTypeMap.set(#tag, V8ClassIndex::SVG##name##ELEMENT);
        FOR_EACH_TAG(ADD_TO_HASH_MAP)
#undef ADD_TO_HASH_MAP
    }

    V8ClassIndex::V8WrapperType type = wrapperTypeMap.get(element->localName().impl());
    if (!type)
        return V8ClassIndex::SVGELEMENT;
    return type;
}
