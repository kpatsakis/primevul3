static void mac80211_hwsim_change_chanctx(struct ieee80211_hw *hw,
					  struct ieee80211_chanctx_conf *ctx,
					  u32 changed)
{
	hwsim_check_chanctx_magic(ctx);
	wiphy_dbg(hw->wiphy,
		  "change channel context control: %d MHz/width: %d/cfreqs:%d/%d MHz\n",
		  ctx->def.chan->center_freq, ctx->def.width,
		  ctx->def.center_freq1, ctx->def.center_freq2);
}
