v8::Handle<v8::Value> V8DOMWrapper::convertSVGObjectWithContextToV8Object(V8ClassIndex::V8WrapperType type, void* object)
{
    if (!object)
        return v8::Null();

    v8::Persistent<v8::Object> result = getDOMSVGObjectWithContextMap().get(object);
    if (!result.IsEmpty())
        return result;

    if (type == V8ClassIndex::SVGPATHSEG)
        type = V8Custom::DowncastSVGPathSeg(object);

    v8::Local<v8::Object> v8Object = instantiateV8Object(type, type, object);
    if (!v8Object.IsEmpty()) {
        result = v8::Persistent<v8::Object>::New(v8Object);
        switch (type) {
#define MAKE_CASE(TYPE, NAME)     \
        case V8ClassIndex::TYPE: static_cast<NAME*>(object)->ref(); break;
        SVG_OBJECT_TYPES(MAKE_CASE)
#undef MAKE_CASE
#define MAKE_CASE(TYPE, NAME)     \
        case V8ClassIndex::TYPE:    \
            static_cast<V8SVGPODTypeWrapper<NAME>*>(object)->ref(); break;
        SVG_POD_NATIVE_TYPES(MAKE_CASE)
#undef MAKE_CASE
        default:
            ASSERT_NOT_REACHED();
        }
        getDOMSVGObjectWithContextMap().set(object, result);
    }

    return result;
}
