static void nfc_urelease_event_work(struct work_struct *work)
{
	struct urelease_work *w = container_of(work, struct urelease_work, w);
	struct class_dev_iter iter;
	struct nfc_dev *dev;

	pr_debug("portid %d\n", w->portid);

	mutex_lock(&nfc_devlist_mutex);

	nfc_device_iter_init(&iter);
	dev = nfc_device_iter_next(&iter);

	while (dev) {
		mutex_lock(&dev->genl_data.genl_data_mutex);

		if (dev->genl_data.poll_req_portid == w->portid) {
			nfc_stop_poll(dev);
			dev->genl_data.poll_req_portid = 0;
		}

		mutex_unlock(&dev->genl_data.genl_data_mutex);

		dev = nfc_device_iter_next(&iter);
	}

	nfc_device_iter_exit(&iter);

	mutex_unlock(&nfc_devlist_mutex);

	kfree(w);
}
