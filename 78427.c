        Notification(WebFrame* frame, v8::Handle<v8::Context> context, int worldId)
            : frame(frame)
            , context(context->GetIsolate(), context)
            , worldId(worldId)
        {
        }
