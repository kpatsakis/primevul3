static inline struct usb_request *hidg_alloc_ep_req(struct usb_ep *ep,
						    unsigned length)
{
	return alloc_ep_req(ep, length);
}
