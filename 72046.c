static inline struct usb_request *midi_alloc_ep_req(struct usb_ep *ep,
						    unsigned length)
{
	return alloc_ep_req(ep, length);
}
