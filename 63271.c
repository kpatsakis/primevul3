void *usb_alloc_coherent(struct usb_device *dev, size_t size, gfp_t mem_flags,
			 dma_addr_t *dma)
{
	if (!dev || !dev->bus)
		return NULL;
	return hcd_buffer_alloc(dev->bus, size, mem_flags, dma);
}
