static int mac80211_hwsim_vendor_cmd_test(struct wiphy *wiphy,
					  struct wireless_dev *wdev,
					  const void *data, int data_len)
{
	struct sk_buff *skb;
	struct nlattr *tb[QCA_WLAN_VENDOR_ATTR_MAX + 1];
	int err;
	u32 val;

	err = nla_parse(tb, QCA_WLAN_VENDOR_ATTR_MAX, data, data_len,
			hwsim_vendor_test_policy, NULL);
	if (err)
		return err;
	if (!tb[QCA_WLAN_VENDOR_ATTR_TEST])
		return -EINVAL;
	val = nla_get_u32(tb[QCA_WLAN_VENDOR_ATTR_TEST]);
	wiphy_dbg(wiphy, "%s: test=%u\n", __func__, val);

	/* Send a vendor event as a test. Note that this would not normally be
	 * done within a command handler, but rather, based on some other
	 * trigger. For simplicity, this command is used to trigger the event
	 * here.
	 *
	 * event_idx = 0 (index in mac80211_hwsim_vendor_commands)
	 */
	skb = cfg80211_vendor_event_alloc(wiphy, wdev, 100, 0, GFP_KERNEL);
	if (skb) {
		/* skb_put() or nla_put() will fill up data within
		 * NL80211_ATTR_VENDOR_DATA.
		 */

		/* Add vendor data */
		nla_put_u32(skb, QCA_WLAN_VENDOR_ATTR_TEST, val + 1);

		/* Send the event - this will call nla_nest_end() */
		cfg80211_vendor_event(skb, GFP_KERNEL);
	}

	/* Send a response to the command */
	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, 10);
	if (!skb)
		return -ENOMEM;

	/* skb_put() or nla_put() will fill up data within
	 * NL80211_ATTR_VENDOR_DATA
	 */
	nla_put_u32(skb, QCA_WLAN_VENDOR_ATTR_TEST, val + 2);

	return cfg80211_vendor_cmd_reply(skb);
}
