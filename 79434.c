v8::Handle<v8::Value> V8DOMWindow::setIntervalMethodCustom(const v8::Arguments& args)
{
    return WindowSetTimeoutImpl(args, false);
}
