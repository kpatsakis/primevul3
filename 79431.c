v8::Handle<v8::Value> V8DOMWindow::openMethodCustom(const v8::Arguments& args)
{
    DOMWindow* impl = V8DOMWindow::toNative(args.Holder());
    if (!BindingSecurity::shouldAllowAccessToFrame(impl->frame()))
        return v8::Undefined();

    String urlString = toWebCoreStringWithUndefinedOrNullCheck(args[0]);
    AtomicString frameName = (args[1]->IsUndefined() || args[1]->IsNull()) ? "_blank" : AtomicString(toWebCoreString(args[1]));
    String windowFeaturesString = toWebCoreStringWithUndefinedOrNullCheck(args[2]);

    RefPtr<DOMWindow> openedWindow = impl->open(urlString, frameName, windowFeaturesString, activeDOMWindow(), firstDOMWindow());
    if (!openedWindow)
        return v8::Undefined();

    return toV8Fast(openedWindow.release(), args, impl);
}
