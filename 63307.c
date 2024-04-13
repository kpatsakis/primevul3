static int __hwahc_op_set_gtk(struct wusbhc *wusbhc, u32 tkid,
			      const void *key, size_t key_size)
{
	u8 key_idx = wusb_key_index(0, WUSB_KEY_INDEX_TYPE_GTK,
				    WUSB_KEY_INDEX_ORIGINATOR_HOST);

	return __hwahc_dev_set_key(wusbhc, 0, tkid, key, key_size, key_idx);
}
