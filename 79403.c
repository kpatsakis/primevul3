static ImageEventSender& beforeLoadEventSender()
{
    DEFINE_STATIC_LOCAL(ImageEventSender, sender, (eventNames().beforeloadEvent));
    return sender;
}
