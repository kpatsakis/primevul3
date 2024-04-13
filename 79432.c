void V8DOMWindow::openerAttrSetterCustom(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    DOMWindow* imp = V8DOMWindow::toNative(info.Holder());

    if (!BindingSecurity::shouldAllowAccessToFrame(imp->frame()))
        return;

    if (value->IsNull()) {
        ASSERT(imp->frame());
        imp->frame()->loader()->setOpener(0);
    }

    info.Holder()->Delete(name);

    info.This()->Set(name, value);
}
