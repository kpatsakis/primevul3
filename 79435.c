v8::Handle<v8::Value> V8DOMWindow::showModalDialogMethodCustom(const v8::Arguments& args)
{
    DOMWindow* impl = V8DOMWindow::toNative(args.Holder());
    if (!BindingSecurity::shouldAllowAccessToFrame(impl->frame()))
        return v8::Undefined();

    String urlString = toWebCoreStringWithUndefinedOrNullCheck(args[0]);
    DialogHandler handler(args[1]);
    String dialogFeaturesString = toWebCoreStringWithUndefinedOrNullCheck(args[2]);

    impl->showModalDialog(urlString, dialogFeaturesString, activeDOMWindow(), firstDOMWindow(), setUpDialog, &handler);

    return handler.returnValue();
}
