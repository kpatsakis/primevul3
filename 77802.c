  AffiliationFetcherTest()
      : request_context_getter_(new net::TestURLRequestContextGetter(
            make_scoped_refptr(new base::NullTaskRunner))) {}
