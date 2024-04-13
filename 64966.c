void Writef(SAVESTREAM* f, const char* frm, ...)
{
    char Buffer[4096];
    va_list args;

    va_start(args, frm);
    vsnprintf(Buffer, 4095, frm, args);
    Buffer[4095] = 0;
    WriteStr(f, Buffer);
    va_end(args);

}
