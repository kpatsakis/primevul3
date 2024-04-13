usbnet_deferred_kevent (struct work_struct *work)
{
	struct usbnet		*dev =
		container_of(work, struct usbnet, kevent);
	int			status;

	/* usb_clear_halt() needs a thread context */
	if (test_bit (EVENT_TX_HALT, &dev->flags)) {
		unlink_urbs (dev, &dev->txq);
		status = usb_autopm_get_interface(dev->intf);
		if (status < 0)
			goto fail_pipe;
		status = usb_clear_halt (dev->udev, dev->out);
		usb_autopm_put_interface(dev->intf);
		if (status < 0 &&
		    status != -EPIPE &&
		    status != -ESHUTDOWN) {
			if (netif_msg_tx_err (dev))
fail_pipe:
				netdev_err(dev->net, "can't clear tx halt, status %d\n",
					   status);
		} else {
			clear_bit (EVENT_TX_HALT, &dev->flags);
			if (status != -ESHUTDOWN)
				netif_wake_queue (dev->net);
		}
	}
	if (test_bit (EVENT_RX_HALT, &dev->flags)) {
		unlink_urbs (dev, &dev->rxq);
		status = usb_autopm_get_interface(dev->intf);
		if (status < 0)
			goto fail_halt;
		status = usb_clear_halt (dev->udev, dev->in);
		usb_autopm_put_interface(dev->intf);
		if (status < 0 &&
		    status != -EPIPE &&
		    status != -ESHUTDOWN) {
			if (netif_msg_rx_err (dev))
fail_halt:
				netdev_err(dev->net, "can't clear rx halt, status %d\n",
					   status);
		} else {
			clear_bit (EVENT_RX_HALT, &dev->flags);
			tasklet_schedule (&dev->bh);
		}
	}

	/* tasklet could resubmit itself forever if memory is tight */
	if (test_bit (EVENT_RX_MEMORY, &dev->flags)) {
		struct urb	*urb = NULL;
		int resched = 1;

		if (netif_running (dev->net))
			urb = usb_alloc_urb (0, GFP_KERNEL);
		else
			clear_bit (EVENT_RX_MEMORY, &dev->flags);
		if (urb != NULL) {
			clear_bit (EVENT_RX_MEMORY, &dev->flags);
			status = usb_autopm_get_interface(dev->intf);
			if (status < 0) {
				usb_free_urb(urb);
				goto fail_lowmem;
			}
			if (rx_submit (dev, urb, GFP_KERNEL) == -ENOLINK)
				resched = 0;
			usb_autopm_put_interface(dev->intf);
fail_lowmem:
			if (resched)
				tasklet_schedule (&dev->bh);
		}
	}

	if (test_bit (EVENT_LINK_RESET, &dev->flags)) {
		struct driver_info	*info = dev->driver_info;
		int			retval = 0;

		clear_bit (EVENT_LINK_RESET, &dev->flags);
		status = usb_autopm_get_interface(dev->intf);
		if (status < 0)
			goto skip_reset;
		if(info->link_reset && (retval = info->link_reset(dev)) < 0) {
			usb_autopm_put_interface(dev->intf);
skip_reset:
			netdev_info(dev->net, "link reset failed (%d) usbnet usb-%s-%s, %s\n",
				    retval,
				    dev->udev->bus->bus_name,
				    dev->udev->devpath,
				    info->description);
		} else {
			usb_autopm_put_interface(dev->intf);
		}

		/* handle link change from link resetting */
		__handle_link_change(dev);
	}

	if (test_bit (EVENT_LINK_CHANGE, &dev->flags))
		__handle_link_change(dev);

	if (test_bit (EVENT_SET_RX_MODE, &dev->flags))
		__handle_set_rx_mode(dev);


	if (dev->flags)
		netdev_dbg(dev->net, "kevent done, flags = 0x%lx\n", dev->flags);
}
