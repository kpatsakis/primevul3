SSLErrorInfo::ErrorType SSLErrorInfo::NetErrorToErrorType(int net_error) {
  switch (net_error) {
    case net::ERR_CERT_COMMON_NAME_INVALID:
      return CERT_COMMON_NAME_INVALID;
    case net::ERR_CERT_DATE_INVALID:
      return CERT_DATE_INVALID;
    case net::ERR_CERT_AUTHORITY_INVALID:
      return CERT_AUTHORITY_INVALID;
    case net::ERR_CERT_CONTAINS_ERRORS:
      return CERT_CONTAINS_ERRORS;
    case net::ERR_CERT_NO_REVOCATION_MECHANISM:
      return CERT_NO_REVOCATION_MECHANISM;
    case net::ERR_CERT_UNABLE_TO_CHECK_REVOCATION:
      return CERT_UNABLE_TO_CHECK_REVOCATION;
    case net::ERR_CERT_REVOKED:
      return CERT_REVOKED;
    case net::ERR_CERT_INVALID:
      return CERT_INVALID;
    case net::ERR_CERT_WEAK_SIGNATURE_ALGORITHM:
      return CERT_WEAK_SIGNATURE_ALGORITHM;
    case net::ERR_CERT_WEAK_KEY:
      return CERT_WEAK_KEY;
    default:
      NOTREACHED();
      return UNKNOWN;
    }
}
