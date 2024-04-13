static std::string selectionAsString(WebFrame* frame)
{
    return std::string(frame->selectionAsText().utf8().data());
}
