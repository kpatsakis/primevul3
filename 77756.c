OneClickSigninHelper::Offer OneClickSigninHelper::CanOfferOnIOThread(
    net::URLRequest* request,
    ProfileIOData* io_data) {
  return CanOfferOnIOThreadImpl(request->url(), request->referrer(),
                                request, io_data);
}
