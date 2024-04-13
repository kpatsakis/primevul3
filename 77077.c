int SSLErrorInfo::GetErrorsForCertStatus(int cert_id,
                                         net::CertStatus cert_status,
                                         const GURL& url,
                                         std::vector<SSLErrorInfo>* errors) {
  const net::CertStatus kErrorFlags[] = {
    net::CERT_STATUS_COMMON_NAME_INVALID,
    net::CERT_STATUS_DATE_INVALID,
    net::CERT_STATUS_AUTHORITY_INVALID,
    net::CERT_STATUS_NO_REVOCATION_MECHANISM,
    net::CERT_STATUS_UNABLE_TO_CHECK_REVOCATION,
    net::CERT_STATUS_REVOKED,
    net::CERT_STATUS_INVALID,
    net::CERT_STATUS_WEAK_SIGNATURE_ALGORITHM,
    net::CERT_STATUS_WEAK_KEY
  };

  const ErrorType kErrorTypes[] = {
    CERT_COMMON_NAME_INVALID,
    CERT_DATE_INVALID,
    CERT_AUTHORITY_INVALID,
    CERT_NO_REVOCATION_MECHANISM,
    CERT_UNABLE_TO_CHECK_REVOCATION,
    CERT_REVOKED,
    CERT_INVALID,
    CERT_WEAK_SIGNATURE_ALGORITHM,
    CERT_WEAK_KEY
  };
  DCHECK(arraysize(kErrorFlags) == arraysize(kErrorTypes));

  scoped_refptr<net::X509Certificate> cert = NULL;
  int count = 0;
  for (size_t i = 0; i < arraysize(kErrorFlags); ++i) {
    if (cert_status & kErrorFlags[i]) {
      count++;
      if (!cert.get()) {
        bool r = content::CertStore::GetInstance()->RetrieveCert(
            cert_id, &cert);
        DCHECK(r);
      }
      if (errors)
        errors->push_back(SSLErrorInfo::CreateError(kErrorTypes[i], cert, url));
    }
  }
  return count;
}
