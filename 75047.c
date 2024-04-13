v8::Persistent<v8::FunctionTemplate> V8DOMWrapper::getTemplate(V8ClassIndex::V8WrapperType type)
{
    v8::Persistent<v8::FunctionTemplate>* cacheCell = V8ClassIndex::GetCache(type);
    if (!cacheCell->IsEmpty())
        return *cacheCell;

    FunctionTemplateFactory factory = V8ClassIndex::GetFactory(type);
    v8::Persistent<v8::FunctionTemplate> descriptor = factory();
    DEFINE_STATIC_LOCAL(v8::Persistent<v8::FunctionTemplate>, toStringTemplate, ());
    if (toStringTemplate.IsEmpty())
        toStringTemplate = v8::Persistent<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(ConstructorToString));
    descriptor->Set(GetToStringName(), toStringTemplate);
    switch (type) {
    case V8ClassIndex::CSSSTYLEDECLARATION:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler( USE_NAMED_PROPERTY_GETTER(CSSStyleDeclaration), USE_NAMED_PROPERTY_SETTER(CSSStyleDeclaration));
        setCollectionStringOrNullIndexedGetter<CSSStyleDeclaration>(descriptor);
        break;
    case V8ClassIndex::CSSRULELIST:
        setCollectionIndexedGetter<CSSRuleList, CSSRule>(descriptor,  V8ClassIndex::CSSRULE);
        break;
    case V8ClassIndex::CSSVALUELIST:
        setCollectionIndexedGetter<CSSValueList, CSSValue>(descriptor, V8ClassIndex::CSSVALUE);
        break;
    case V8ClassIndex::CSSVARIABLESDECLARATION:
        setCollectionStringOrNullIndexedGetter<CSSVariablesDeclaration>(descriptor);
        break;
    case V8ClassIndex::WEBKITCSSTRANSFORMVALUE:
        setCollectionIndexedGetter<WebKitCSSTransformValue, CSSValue>(descriptor, V8ClassIndex::CSSVALUE);
        break;
    case V8ClassIndex::HTMLALLCOLLECTION:
        descriptor->InstanceTemplate()->MarkAsUndetectable(); // fall through
    case V8ClassIndex::HTMLCOLLECTION:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(HTMLCollection));
        descriptor->InstanceTemplate()->SetCallAsFunctionHandler(USE_CALLBACK(HTMLCollectionCallAsFunction));
        setCollectionIndexedGetter<HTMLCollection, Node>(descriptor, V8ClassIndex::NODE);
        break;
    case V8ClassIndex::HTMLOPTIONSCOLLECTION:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(HTMLCollection));
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(HTMLOptionsCollection), USE_INDEXED_PROPERTY_SETTER(HTMLOptionsCollection));
        descriptor->InstanceTemplate()->SetCallAsFunctionHandler(USE_CALLBACK(HTMLCollectionCallAsFunction));
        break;
    case V8ClassIndex::HTMLSELECTELEMENT:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(HTMLSelectElementCollection));
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(nodeCollectionIndexedPropertyGetter<HTMLSelectElement>, USE_INDEXED_PROPERTY_SETTER(HTMLSelectElementCollection),
            0, 0, nodeCollectionIndexedPropertyEnumerator<HTMLSelectElement>, v8::Integer::New(V8ClassIndex::NODE));
        break;
    case V8ClassIndex::HTMLDOCUMENT: {
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(HTMLDocument), 0, 0, USE_NAMED_PROPERTY_DELETER(HTMLDocument));

        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        ASSERT(instanceTemplate->InternalFieldCount() == V8Custom::kDefaultWrapperInternalFieldCount);
        instanceTemplate->SetInternalFieldCount(V8Custom::kHTMLDocumentInternalFieldCount);
        break;
    }
#if ENABLE(SVG)
    case V8ClassIndex::SVGDOCUMENT:  // fall through
#endif
    case V8ClassIndex::DOCUMENT: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        ASSERT(instanceTemplate->InternalFieldCount() == V8Custom::kDefaultWrapperInternalFieldCount);
        instanceTemplate->SetInternalFieldCount( V8Custom::kDocumentMinimumInternalFieldCount);
        break;
    }
    case V8ClassIndex::HTMLAPPLETELEMENT:  // fall through
    case V8ClassIndex::HTMLEMBEDELEMENT:  // fall through
    case V8ClassIndex::HTMLOBJECTELEMENT:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(HTMLPlugInElement), USE_NAMED_PROPERTY_SETTER(HTMLPlugInElement));
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(HTMLPlugInElement), USE_INDEXED_PROPERTY_SETTER(HTMLPlugInElement));
        descriptor->InstanceTemplate()->SetCallAsFunctionHandler(USE_CALLBACK(HTMLPlugInElement));
        break;
    case V8ClassIndex::HTMLFRAMESETELEMENT:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(HTMLFrameSetElement));
        break;
    case V8ClassIndex::HTMLFORMELEMENT:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(HTMLFormElement));
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(HTMLFormElement), 0, 0, 0, nodeCollectionIndexedPropertyEnumerator<HTMLFormElement>, v8::Integer::New(V8ClassIndex::NODE));
        break;
    case V8ClassIndex::CANVASPIXELARRAY:
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(CanvasPixelArray), USE_INDEXED_PROPERTY_SETTER(CanvasPixelArray));
        break;
    case V8ClassIndex::STYLESHEET:  // fall through
    case V8ClassIndex::CSSSTYLESHEET: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        ASSERT(instanceTemplate->InternalFieldCount() == V8Custom::kDefaultWrapperInternalFieldCount);
        instanceTemplate->SetInternalFieldCount(V8Custom::kStyleSheetInternalFieldCount);
        break;
    }
    case V8ClassIndex::MEDIALIST:
        setCollectionStringOrNullIndexedGetter<MediaList>(descriptor);
        break;
    case V8ClassIndex::MIMETYPEARRAY:
        setCollectionIndexedAndNamedGetters<MimeTypeArray, MimeType>(descriptor, V8ClassIndex::MIMETYPE);
        break;
    case V8ClassIndex::NAMEDNODEMAP:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(NamedNodeMap));
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(NamedNodeMap), 0, 0, 0, collectionIndexedPropertyEnumerator<NamedNodeMap>, v8::Integer::New(V8ClassIndex::NODE));
        break;
#if ENABLE(DOM_STORAGE)
    case V8ClassIndex::STORAGE:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(Storage), USE_NAMED_PROPERTY_SETTER(Storage), 0, USE_NAMED_PROPERTY_DELETER(Storage), V8Custom::v8StorageNamedPropertyEnumerator);
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(Storage), USE_INDEXED_PROPERTY_SETTER(Storage), 0, USE_INDEXED_PROPERTY_DELETER(Storage));
        break;
#endif
    case V8ClassIndex::NODELIST:
        setCollectionIndexedGetter<NodeList, Node>(descriptor, V8ClassIndex::NODE);
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(NodeList));
        break;
    case V8ClassIndex::PLUGIN:
        setCollectionIndexedAndNamedGetters<Plugin, MimeType>(descriptor, V8ClassIndex::MIMETYPE);
        break;
    case V8ClassIndex::PLUGINARRAY:
        setCollectionIndexedAndNamedGetters<PluginArray, Plugin>(descriptor, V8ClassIndex::PLUGIN);
        break;
    case V8ClassIndex::STYLESHEETLIST:
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(StyleSheetList));
        setCollectionIndexedGetter<StyleSheetList, StyleSheet>(descriptor, V8ClassIndex::STYLESHEET);
        break;
    case V8ClassIndex::DOMWINDOW: {
        v8::Local<v8::Signature> defaultSignature = v8::Signature::New(descriptor);

        descriptor->PrototypeTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(DOMWindow));
        descriptor->PrototypeTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(DOMWindow));

        descriptor->SetHiddenPrototype(true);

        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kDOMWindowInternalFieldCount);

        instanceTemplate->SetAccessCheckCallbacks(V8Custom::v8DOMWindowNamedSecurityCheck, V8Custom::v8DOMWindowIndexedSecurityCheck, v8::Integer::New(V8ClassIndex::DOMWINDOW), false);
        break;
    }
    case V8ClassIndex::LOCATION: {
        v8::Local<v8::ObjectTemplate> instance = descriptor->InstanceTemplate();
        instance->SetAccessor(v8::String::New("reload"), V8Custom::v8LocationReloadAccessorGetter, 0, v8::Handle<v8::Value>(), v8::ALL_CAN_READ, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly));
        instance->SetAccessor(v8::String::New("replace"), V8Custom::v8LocationReplaceAccessorGetter, 0, v8::Handle<v8::Value>(), v8::ALL_CAN_READ, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly));
        instance->SetAccessor(v8::String::New("assign"), V8Custom::v8LocationAssignAccessorGetter, 0, v8::Handle<v8::Value>(), v8::ALL_CAN_READ, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly));
        break;
    }
    case V8ClassIndex::HISTORY:
        break;

    case V8ClassIndex::MESSAGECHANNEL: {
        descriptor->SetCallHandler(USE_CALLBACK(MessageChannelConstructor));
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kMessageChannelInternalFieldCount);
        break;
    }

    case V8ClassIndex::MESSAGEPORT: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kMessagePortInternalFieldCount);
        break;
    }

#if ENABLE(WORKERS)
    case V8ClassIndex::ABSTRACTWORKER: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kAbstractWorkerInternalFieldCount);
        break;
    }

    case V8ClassIndex::WORKER: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kWorkerInternalFieldCount);
        descriptor->SetCallHandler(USE_CALLBACK(WorkerConstructor));
        break;
    }

    case V8ClassIndex::WORKERCONTEXT: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kWorkerContextInternalFieldCount);
        break;
    }
#endif // WORKERS

    case V8ClassIndex::DOMPARSER:
        descriptor->SetCallHandler(USE_CALLBACK(DOMParserConstructor));
        break;
#if ENABLE(VIDEO)
    case V8ClassIndex::HTMLAUDIOELEMENT:
        descriptor->SetCallHandler(USE_CALLBACK(HTMLAudioElementConstructor));
        break;
#endif
    case V8ClassIndex::HTMLIMAGEELEMENT:
        descriptor->SetCallHandler(USE_CALLBACK(HTMLImageElementConstructor));
        break;
    case V8ClassIndex::HTMLOPTIONELEMENT:
        descriptor->SetCallHandler(USE_CALLBACK(HTMLOptionElementConstructor));
        break;
    case V8ClassIndex::WEBKITCSSMATRIX:
        descriptor->SetCallHandler(USE_CALLBACK(WebKitCSSMatrixConstructor));
        break;
    case V8ClassIndex::WEBKITPOINT:
        descriptor->SetCallHandler(USE_CALLBACK(WebKitPointConstructor));
        break;
    case V8ClassIndex::XMLSERIALIZER:
        descriptor->SetCallHandler(USE_CALLBACK(XMLSerializerConstructor));
        break;
    case V8ClassIndex::XMLHTTPREQUEST: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kXMLHttpRequestInternalFieldCount);
        descriptor->SetCallHandler(USE_CALLBACK(XMLHttpRequestConstructor));
        break;
    }
    case V8ClassIndex::XMLHTTPREQUESTUPLOAD: {
        v8::Local<v8::ObjectTemplate> instanceTemplate = descriptor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(V8Custom::kXMLHttpRequestInternalFieldCount);
        break;
    }
    case V8ClassIndex::XPATHEVALUATOR:
        descriptor->SetCallHandler(USE_CALLBACK(XPathEvaluatorConstructor));
        break;
    case V8ClassIndex::XSLTPROCESSOR:
        descriptor->SetCallHandler(USE_CALLBACK(XSLTProcessorConstructor));
        break;
    case V8ClassIndex::CLIENTRECTLIST:
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(ClientRectList));
        break;
#if ENABLE(DATAGRID)
    case V8ClassIndex::DATAGRIDCOLUMNLIST:
        descriptor->InstanceTemplate()->SetIndexedPropertyHandler(USE_INDEXED_PROPERTY_GETTER(DataGridColumnList));
        descriptor->InstanceTemplate()->SetNamedPropertyHandler(USE_NAMED_PROPERTY_GETTER(DataGridColumnList));
        break;
#endif
    default:
        break;
    }

    *cacheCell = descriptor;
    return descriptor;
}
