Eina_Bool ewk_view_js_object_add(Evas_Object* ewkView, Ewk_JS_Object* object, const char* objectName)
{
#if ENABLE(NETSCAPE_PLUGIN_API)
    if (object->view) // object has already been added to another ewk_view
        return false;
    object->name = eina_stringshare_add(objectName);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    JSC::JSLock lock(JSC::SilenceAssertionsOnly);
    WebCore::JSDOMWindow* window = toJSDOMWindow(priv->mainFrame, WebCore::mainThreadNormalWorld());
    JSC::Bindings::RootObject* root;
    root = priv->mainFrame->script()->bindingRootObject();

    if (!window) {
        ERR("Warning: couldn't get window object");
        return false;
    }

    JSC::ExecState* executeState = window->globalExec();

    object->view = ewkView;
    JSC::JSObject* runtimeObject = (JSC::JSObject*)JSC::Bindings::CInstance::create((NPObject*)object, root)->createRuntimeObject(executeState);
    JSC::Identifier id = JSC::Identifier(executeState, objectName);

    JSC::PutPropertySlot slot;
    window->methodTable()->put(window, executeState, id, runtimeObject, slot);
    return true;
#else
    return false;
#endif // ENABLE(NETSCAPE_PLUGIN_API)
}
