static void mac80211_hwsim_remove_chanctx(struct ieee80211_hw *hw,
					  struct ieee80211_chanctx_conf *ctx)
{
	wiphy_dbg(hw->wiphy,
		  "remove channel context control: %d MHz/width: %d/cfreqs:%d/%d MHz\n",
		  ctx->def.chan->center_freq, ctx->def.width,
		  ctx->def.center_freq1, ctx->def.center_freq2);
	hwsim_check_chanctx_magic(ctx);
	hwsim_clear_chanctx_magic(ctx);
}
