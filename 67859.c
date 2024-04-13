static int mac80211_hwsim_add_chanctx(struct ieee80211_hw *hw,
				      struct ieee80211_chanctx_conf *ctx)
{
	hwsim_set_chanctx_magic(ctx);
	wiphy_dbg(hw->wiphy,
		  "add channel context control: %d MHz/width: %d/cfreqs:%d/%d MHz\n",
		  ctx->def.chan->center_freq, ctx->def.width,
		  ctx->def.center_freq1, ctx->def.center_freq2);
	return 0;
}
