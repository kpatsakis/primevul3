static int append_radio_msg(struct sk_buff *skb, int id,
			    struct hwsim_new_radio_params *param)
{
	int ret;

	ret = nla_put_u32(skb, HWSIM_ATTR_RADIO_ID, id);
	if (ret < 0)
		return ret;

	if (param->channels) {
		ret = nla_put_u32(skb, HWSIM_ATTR_CHANNELS, param->channels);
		if (ret < 0)
			return ret;
	}

	if (param->reg_alpha2) {
		ret = nla_put(skb, HWSIM_ATTR_REG_HINT_ALPHA2, 2,
			      param->reg_alpha2);
		if (ret < 0)
			return ret;
	}

	if (param->regd) {
		int i;

		for (i = 0; i < ARRAY_SIZE(hwsim_world_regdom_custom); i++) {
			if (hwsim_world_regdom_custom[i] != param->regd)
				continue;

			ret = nla_put_u32(skb, HWSIM_ATTR_REG_CUSTOM_REG, i);
			if (ret < 0)
				return ret;
			break;
		}
	}

	if (param->reg_strict) {
		ret = nla_put_flag(skb, HWSIM_ATTR_REG_STRICT_REG);
		if (ret < 0)
			return ret;
	}

	if (param->p2p_device) {
		ret = nla_put_flag(skb, HWSIM_ATTR_SUPPORT_P2P_DEVICE);
		if (ret < 0)
			return ret;
	}

	if (param->use_chanctx) {
		ret = nla_put_flag(skb, HWSIM_ATTR_USE_CHANCTX);
		if (ret < 0)
			return ret;
	}

	if (param->hwname) {
		ret = nla_put(skb, HWSIM_ATTR_RADIO_NAME,
			      strlen(param->hwname), param->hwname);
		if (ret < 0)
			return ret;
	}

	return 0;
}
