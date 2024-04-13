v8::Handle<v8::Value> V8DOMWrapper::convertCSSRuleToV8Object(CSSRule* rule)
{
    if (!rule) 
        return v8::Null();

    v8::Handle<v8::Object> wrapper = getDOMObjectMap().get(rule);
    if (!wrapper.IsEmpty())
        return wrapper;

    V8ClassIndex::V8WrapperType type;

    switch (rule->type()) {
    case CSSRule::STYLE_RULE:
        type = V8ClassIndex::CSSSTYLERULE;
        break;
    case CSSRule::CHARSET_RULE:
        type = V8ClassIndex::CSSCHARSETRULE;
        break;
    case CSSRule::IMPORT_RULE:
        type = V8ClassIndex::CSSIMPORTRULE;
        break;
    case CSSRule::MEDIA_RULE:
        type = V8ClassIndex::CSSMEDIARULE;
        break;
    case CSSRule::FONT_FACE_RULE:
        type = V8ClassIndex::CSSFONTFACERULE;
        break;
    case CSSRule::PAGE_RULE:
        type = V8ClassIndex::CSSPAGERULE;
        break;
    case CSSRule::VARIABLES_RULE:
        type = V8ClassIndex::CSSVARIABLESRULE;
        break;
    case CSSRule::WEBKIT_KEYFRAME_RULE:
        type = V8ClassIndex::WEBKITCSSKEYFRAMERULE;
        break;
    case CSSRule::WEBKIT_KEYFRAMES_RULE:
        type = V8ClassIndex::WEBKITCSSKEYFRAMESRULE;
        break;
    default:  // CSSRule::UNKNOWN_RULE
        type = V8ClassIndex::CSSRULE;
        break;
    }

    v8::Handle<v8::Object> result = instantiateV8Object(type, V8ClassIndex::CSSRULE, rule);
    if (!result.IsEmpty()) {
        rule->ref();
        setJSWrapperForDOMObject(rule, v8::Persistent<v8::Object>::New(result));
    }
    return result;
}
