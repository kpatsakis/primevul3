static int closeFunc(void* context)
{
    if (context != &globalDescriptor) {
        OffsetBuffer* data = static_cast<OffsetBuffer*>(context);
        delete data;
    }
    return 0;
}
