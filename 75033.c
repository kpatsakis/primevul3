v8::Handle<v8::Value> V8DOMWrapper::convertCSSValueToV8Object(CSSValue* value)
{
    if (!value)
        return v8::Null();

    v8::Handle<v8::Object> wrapper = getDOMObjectMap().get(value);
    if (!wrapper.IsEmpty())
        return wrapper;

    V8ClassIndex::V8WrapperType type;

    if (value->isWebKitCSSTransformValue())
        type = V8ClassIndex::WEBKITCSSTRANSFORMVALUE;
    else if (value->isValueList())
        type = V8ClassIndex::CSSVALUELIST;
    else if (value->isPrimitiveValue())
        type = V8ClassIndex::CSSPRIMITIVEVALUE;
#if ENABLE(SVG)
    else if (value->isSVGPaint())
        type = V8ClassIndex::SVGPAINT;
    else if (value->isSVGColor())
        type = V8ClassIndex::SVGCOLOR;
#endif
    else
        type = V8ClassIndex::CSSVALUE;

    v8::Handle<v8::Object> result = instantiateV8Object(type, V8ClassIndex::CSSVALUE, value);
    if (!result.IsEmpty()) {
        value->ref();
        setJSWrapperForDOMObject(value, v8::Persistent<v8::Object>::New(result));
    }

    return result;
}
