void FrameLoader::addHTTPOriginIfNeeded(ResourceRequest& request, const String& origin)
{
    if (!request.httpOrigin().isEmpty())
        return;  // Request already has an Origin header.

    if (request.httpMethod() == "GET" || request.httpMethod() == "HEAD")
        return;


    if (origin.isEmpty()) {
        request.setHTTPOrigin(SecurityOrigin::createUnique()->toString());
        return;
    }

    request.setHTTPOrigin(origin);
}
