static v8::Handle<v8::Value> ConstructorToString(const v8::Arguments& args)
{
    v8::Handle<v8::Value> value = args.Callee()->Get(GetToStringName());
    if (!value->IsFunction()) 
        return v8::String::New("");
    return v8::Handle<v8::Function>::Cast(value)->Call(args.This(), 0, 0);
}
