evutil_gai_strerror(int err)
{
	/* As a sneaky side-benefit, this case statement will get most
	 * compilers to tell us if any of the error codes we defined
	 * conflict with the platform's native error codes. */
	switch (err) {
	case EVUTIL_EAI_CANCEL:
		return "Request canceled";
	case 0:
		return "No error";

	case EVUTIL_EAI_ADDRFAMILY:
		return "address family for nodename not supported";
	case EVUTIL_EAI_AGAIN:
		return "temporary failure in name resolution";
	case EVUTIL_EAI_BADFLAGS:
		return "invalid value for ai_flags";
	case EVUTIL_EAI_FAIL:
		return "non-recoverable failure in name resolution";
	case EVUTIL_EAI_FAMILY:
		return "ai_family not supported";
	case EVUTIL_EAI_MEMORY:
		return "memory allocation failure";
	case EVUTIL_EAI_NODATA:
		return "no address associated with nodename";
	case EVUTIL_EAI_NONAME:
		return "nodename nor servname provided, or not known";
	case EVUTIL_EAI_SERVICE:
		return "servname not supported for ai_socktype";
	case EVUTIL_EAI_SOCKTYPE:
		return "ai_socktype not supported";
	case EVUTIL_EAI_SYSTEM:
		return "system error";
	default:
#if defined(USE_NATIVE_GETADDRINFO) && defined(_WIN32)
		return gai_strerrorA(err);
#elif defined(USE_NATIVE_GETADDRINFO)
		return gai_strerror(err);
#else
		return "Unknown error code";
#endif
	}
}
