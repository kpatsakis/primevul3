v8::Handle<v8::Value> WindowSetTimeoutImpl(const v8::Arguments& args, bool singleShot)
{
    int argumentCount = args.Length();

    if (argumentCount < 1)
        return v8::Undefined();

    DOMWindow* imp = V8DOMWindow::toNative(args.Holder());
    ScriptExecutionContext* scriptContext = static_cast<ScriptExecutionContext*>(imp->document());

    if (!scriptContext)
        return setDOMException(INVALID_ACCESS_ERR, args.GetIsolate());

    v8::Handle<v8::Value> function = args[0];
    WTF::String functionString;
    if (!function->IsFunction()) {
        if (function->IsString())
            functionString = toWebCoreString(function);
        else {
            v8::Handle<v8::Value> v8String = function->ToString();

            if (v8String.IsEmpty())
                return v8::Undefined();

            functionString = toWebCoreString(v8String);
        }

        if (functionString.length() == 0)
            return v8::Undefined();
    }

    int32_t timeout = 0;
    if (argumentCount >= 2)
        timeout = args[1]->Int32Value();

    if (!BindingSecurity::shouldAllowAccessToFrame(imp->frame()))
        return v8::Undefined();

    int id;
    if (function->IsFunction()) {
        int paramCount = argumentCount >= 2 ? argumentCount - 2 : 0;
        v8::Local<v8::Value>* params = 0;
        if (paramCount > 0) {
            params = new v8::Local<v8::Value>[paramCount];
            for (int i = 0; i < paramCount; i++)
                params[i] = args[i+2];
        }

        ASSERT(imp->frame());
        OwnPtr<ScheduledAction> action = adoptPtr(new ScheduledAction(imp->frame()->script()->currentWorldContext(), v8::Handle<v8::Function>::Cast(function), paramCount, params, args.GetIsolate()));

        delete[] params;

        id = DOMTimer::install(scriptContext, action.release(), timeout, singleShot);
    } else {
        if (imp->document() && !imp->document()->contentSecurityPolicy()->allowEval())
            return v8Integer(0, args.GetIsolate());
        ASSERT(imp->frame());
        id = DOMTimer::install(scriptContext, adoptPtr(new ScheduledAction(imp->frame()->script()->currentWorldContext(), functionString, KURL(), args.GetIsolate())), timeout, singleShot);
    }

    double maximumFireInterval = static_cast<double>(timeout) / 1000 / 2;
    V8GCForContextDispose::instance().notifyIdleSooner(maximumFireInterval);

    return v8Integer(id, args.GetIsolate());
}
