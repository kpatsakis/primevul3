static ImageEventSender& loadEventSender()
{
    DEFINE_STATIC_LOCAL(ImageEventSender, sender, (eventNames().loadEvent));
    return sender;
}
