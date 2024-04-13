v8::Handle<v8::Value> V8DOMWindow::namedPropertyGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{

    DOMWindow* window = V8DOMWindow::toNative(info.Holder());
    if (!window)
        return v8Undefined();

    Frame* frame = window->frame();
    if (!frame)
        return v8Undefined();

    AtomicString propName = toWebCoreAtomicString(name);
    Frame* child = frame->tree()->scopedChild(propName);
    if (child)
        return toV8Fast(child->document()->domWindow(), info, window);

    if (!info.Holder()->GetRealNamedProperty(name).IsEmpty())
        return v8Undefined();

    Document* doc = frame->document();

    if (doc && doc->isHTMLDocument()) {
        if (toHTMLDocument(doc)->hasNamedItem(propName.impl()) || doc->hasElementWithId(propName.impl())) {
            RefPtr<HTMLCollection> items = doc->windowNamedItems(propName);
            if (!items->isEmpty()) {
                if (items->hasExactlyOneItem())
                    return toV8Fast(items->item(0), info, window);
                return toV8Fast(items.release(), info, window);
            }
        }
    }

    return v8Undefined();
}
