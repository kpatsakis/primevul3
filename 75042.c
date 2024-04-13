v8::Handle<v8::Value> V8DOMWrapper::convertToV8Object(V8ClassIndex::V8WrapperType type, void* impl)
{
    ASSERT(type != V8ClassIndex::EVENTLISTENER);
    ASSERT(type != V8ClassIndex::EVENTTARGET);
    ASSERT(type != V8ClassIndex::EVENT);

    bool isActiveDomObject = false;
    switch (type) {
#define MAKE_CASE(TYPE, NAME) case V8ClassIndex::TYPE:
        DOM_NODE_TYPES(MAKE_CASE)
#if ENABLE(SVG)
        SVG_NODE_TYPES(MAKE_CASE)
#endif
        return convertNodeToV8Object(static_cast<Node*>(impl));
    case V8ClassIndex::CSSVALUE:
        return convertCSSValueToV8Object(static_cast<CSSValue*>(impl));
    case V8ClassIndex::CSSRULE:
        return convertCSSRuleToV8Object(static_cast<CSSRule*>(impl));
    case V8ClassIndex::STYLESHEET:
        return convertStyleSheetToV8Object(static_cast<StyleSheet*>(impl));
    case V8ClassIndex::DOMWINDOW:
        return convertWindowToV8Object(static_cast<DOMWindow*>(impl));
#if ENABLE(SVG)
        SVG_NONNODE_TYPES(MAKE_CASE)
        if (type == V8ClassIndex::SVGELEMENTINSTANCE)
            return convertSVGElementInstanceToV8Object(static_cast<SVGElementInstance*>(impl));
        return convertSVGObjectWithContextToV8Object(type, impl);
#endif

        ACTIVE_DOM_OBJECT_TYPES(MAKE_CASE)
        isActiveDomObject = true;
        break;
    default:
        break;
  }

#undef MAKE_CASE

    if (!impl)
        return v8::Null();

    v8::Persistent<v8::Object> result = isActiveDomObject ? getActiveDOMObjectMap().get(impl) : getDOMObjectMap().get(impl);
    if (result.IsEmpty()) {
        v8::Local<v8::Object> v8Object = instantiateV8Object(type, type, impl);
        if (!v8Object.IsEmpty()) {
            switch (type) {
#define MAKE_CASE(TYPE, NAME) \
            case V8ClassIndex::TYPE: static_cast<NAME*>(impl)->ref(); break;
                DOM_OBJECT_TYPES(MAKE_CASE)
#undef MAKE_CASE
            default:
                ASSERT_NOT_REACHED();
            }
            result = v8::Persistent<v8::Object>::New(v8Object);
            if (isActiveDomObject)
                setJSWrapperForActiveDOMObject(impl, result);
            else
                setJSWrapperForDOMObject(impl, result);

            switch (type) {
            case V8ClassIndex::CONSOLE:
                setHiddenWindowReference(static_cast<Console*>(impl)->frame(), V8Custom::kDOMWindowConsoleIndex, result);
                break;
            case V8ClassIndex::HISTORY:
                setHiddenWindowReference(static_cast<History*>(impl)->frame(), V8Custom::kDOMWindowHistoryIndex, result);
                break;
            case V8ClassIndex::NAVIGATOR:
                setHiddenWindowReference(static_cast<Navigator*>(impl)->frame(), V8Custom::kDOMWindowNavigatorIndex, result);
                break;
            case V8ClassIndex::SCREEN:
                setHiddenWindowReference(static_cast<Screen*>(impl)->frame(), V8Custom::kDOMWindowScreenIndex, result);
                break;
            case V8ClassIndex::LOCATION:
                setHiddenWindowReference(static_cast<Location*>(impl)->frame(), V8Custom::kDOMWindowLocationIndex, result);
                break;
            case V8ClassIndex::DOMSELECTION:
                setHiddenWindowReference(static_cast<DOMSelection*>(impl)->frame(), V8Custom::kDOMWindowDOMSelectionIndex, result);
                break;
            case V8ClassIndex::BARINFO: {
                BarInfo* barInfo = static_cast<BarInfo*>(impl);
                Frame* frame = barInfo->frame();
                switch (barInfo->type()) {
                case BarInfo::Locationbar:
                    setHiddenWindowReference(frame, V8Custom::kDOMWindowLocationbarIndex, result);
                    break;
                case BarInfo::Menubar:
                    setHiddenWindowReference(frame, V8Custom::kDOMWindowMenubarIndex, result);
                    break;
                case BarInfo::Personalbar:
                    setHiddenWindowReference(frame, V8Custom::kDOMWindowPersonalbarIndex, result);
                    break;
                case BarInfo::Scrollbars:
                    setHiddenWindowReference(frame, V8Custom::kDOMWindowScrollbarsIndex, result);
                    break;
                case BarInfo::Statusbar:
                    setHiddenWindowReference(frame, V8Custom::kDOMWindowStatusbarIndex, result);
                    break;
                case BarInfo::Toolbar:
                    setHiddenWindowReference(frame, V8Custom::kDOMWindowToolbarIndex, result);
                    break;
                }
                break;
            }
            default:
                break;
            }
        }
    }
    return result;
}
