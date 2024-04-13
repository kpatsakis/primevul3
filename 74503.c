void FrameLoader::addHTTPOriginIfNeeded(ResourceRequest& request, String origin)
{
    if (!request.httpOrigin().isEmpty())
        return;  // Request already has an Origin header.

    if (request.httpMethod() == "GET" || request.httpMethod() == "HEAD")
        return;


    if (origin.isEmpty()) {
        origin = SecurityOrigin::createEmpty()->toString();
    }

    request.setHTTPOrigin(origin);
}
