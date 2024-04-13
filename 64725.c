CURLcode Curl_init_userdefined(struct Curl_easy *data)
{
  struct UserDefined *set = &data->set;
  CURLcode result = CURLE_OK;

  set->out = stdout; /* default output to stdout */
  set->in_set = stdin;  /* default input from stdin */
  set->err  = stderr;  /* default stderr to stderr */

  /* use fwrite as default function to store output */
  set->fwrite_func = (curl_write_callback)fwrite;

  /* use fread as default function to read input */
  set->fread_func_set = (curl_read_callback)fread;
  set->is_fread_set = 0;
  set->is_fwrite_set = 0;

  set->seek_func = ZERO_NULL;
  set->seek_client = ZERO_NULL;

  /* conversion callbacks for non-ASCII hosts */
  set->convfromnetwork = ZERO_NULL;
  set->convtonetwork   = ZERO_NULL;
  set->convfromutf8    = ZERO_NULL;

  set->filesize = -1;        /* we don't know the size */
  set->postfieldsize = -1;   /* unknown size */
  set->maxredirs = -1;       /* allow any amount by default */

  set->httpreq = HTTPREQ_GET; /* Default HTTP request */
  set->rtspreq = RTSPREQ_OPTIONS; /* Default RTSP request */
  set->ftp_use_epsv = TRUE;   /* FTP defaults to EPSV operations */
  set->ftp_use_eprt = TRUE;   /* FTP defaults to EPRT operations */
  set->ftp_use_pret = FALSE;  /* mainly useful for drftpd servers */
  set->ftp_filemethod = FTPFILE_MULTICWD;

  set->dns_cache_timeout = 60; /* Timeout every 60 seconds by default */

  /* Set the default size of the SSL session ID cache */
  set->general_ssl.max_ssl_sessions = 5;

  set->proxyport = 0;
  set->proxytype = CURLPROXY_HTTP; /* defaults to HTTP proxy */
  set->httpauth = CURLAUTH_BASIC;  /* defaults to basic */
  set->proxyauth = CURLAUTH_BASIC; /* defaults to basic */

  /* SOCKS5 proxy auth defaults to username/password + GSS-API */
  set->socks5auth = CURLAUTH_BASIC | CURLAUTH_GSSAPI;

  /* make libcurl quiet by default: */
  set->hide_progress = TRUE;  /* CURLOPT_NOPROGRESS changes these */

  Curl_mime_initpart(&set->mimepost, data);

  /*
   * libcurl 7.10 introduced SSL verification *by default*! This needs to be
   * switched off unless wanted.
   */
  set->ssl.primary.verifypeer = TRUE;
  set->ssl.primary.verifyhost = TRUE;
#ifdef USE_TLS_SRP
  set->ssl.authtype = CURL_TLSAUTH_NONE;
#endif
  set->ssh_auth_types = CURLSSH_AUTH_DEFAULT; /* defaults to any auth
                                                      type */
  set->ssl.primary.sessionid = TRUE; /* session ID caching enabled by
                                        default */
  set->proxy_ssl = set->ssl;

  set->new_file_perms = 0644;    /* Default permissions */
  set->new_directory_perms = 0755; /* Default permissions */

  /* for the *protocols fields we don't use the CURLPROTO_ALL convenience
     define since we internally only use the lower 16 bits for the passed
     in bitmask to not conflict with the private bits */
  set->allowed_protocols = CURLPROTO_ALL;
  set->redir_protocols = CURLPROTO_ALL &  /* All except FILE, SCP and SMB */
                          ~(CURLPROTO_FILE | CURLPROTO_SCP | CURLPROTO_SMB |
                            CURLPROTO_SMBS);

#if defined(HAVE_GSSAPI) || defined(USE_WINDOWS_SSPI)
  /*
   * disallow unprotected protection negotiation NEC reference implementation
   * seem not to follow rfc1961 section 4.3/4.4
   */
  set->socks5_gssapi_nec = FALSE;
#endif

  /* Set the default CA cert bundle/path detected/specified at build time.
   *
   * If Schannel (WinSSL) is the selected SSL backend then these locations
   * are ignored. We allow setting CA location for schannel only when
   * explicitly specified by the user via CURLOPT_CAINFO / --cacert.
   */
  if(Curl_ssl_backend() != CURLSSLBACKEND_SCHANNEL) {
#if defined(CURL_CA_BUNDLE)
    result = Curl_setstropt(&set->str[STRING_SSL_CAFILE_ORIG], CURL_CA_BUNDLE);
    if(result)
      return result;

    result = Curl_setstropt(&set->str[STRING_SSL_CAFILE_PROXY],
                            CURL_CA_BUNDLE);
    if(result)
      return result;
#endif
#if defined(CURL_CA_PATH)
    result = Curl_setstropt(&set->str[STRING_SSL_CAPATH_ORIG], CURL_CA_PATH);
    if(result)
      return result;

    result = Curl_setstropt(&set->str[STRING_SSL_CAPATH_PROXY], CURL_CA_PATH);
    if(result)
      return result;
#endif
  }

  set->wildcard_enabled = FALSE;
  set->chunk_bgn      = ZERO_NULL;
  set->chunk_end      = ZERO_NULL;
  set->tcp_keepalive = FALSE;
  set->tcp_keepintvl = 60;
  set->tcp_keepidle = 60;
  set->tcp_fastopen = FALSE;
  set->tcp_nodelay = TRUE;
  set->ssl_enable_npn = TRUE;
  set->ssl_enable_alpn = TRUE;
  set->expect_100_timeout = 1000L; /* Wait for a second by default. */
  set->sep_headers = TRUE; /* separated header lists by default */
  set->buffer_size = READBUFFER_SIZE;
  set->upload_buffer_size = UPLOADBUFFER_DEFAULT;
  set->happy_eyeballs_timeout = CURL_HET_DEFAULT;
  set->fnmatch = ZERO_NULL;
  set->upkeep_interval_ms = CURL_UPKEEP_INTERVAL_DEFAULT;
  set->maxconnects = DEFAULT_CONNCACHE_SIZE; /* for easy handles */
  set->httpversion =
#ifdef USE_NGHTTP2
    CURL_HTTP_VERSION_2TLS
#else
    CURL_HTTP_VERSION_1_1
#endif
    ;
  Curl_http2_init_userset(set);
  return result;
}
