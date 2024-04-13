Bool gf_fs_check_filter_register_cap_ex(const GF_FilterRegister *f_reg, u32 incode, GF_PropertyValue *cap_input, u32 outcode, GF_PropertyValue *cap_output, Bool exact_match_only, Bool out_cap_excluded)
{
	u32 j;
	u32 has_raw_in = 0;
	u32 has_cid_match = 0;
	u32 exclude_cid_out = 0;
	u32 has_exclude_cid_out = 0;
	for (j=0; j<f_reg->nb_caps; j++) {
		const GF_FilterCapability *cap = &f_reg->caps[j];
		if (!(cap->flags & GF_CAPFLAG_IN_BUNDLE)) {
			//CID not excluded, raw in present and CID explicit match or not included in excluded set
			if (!exclude_cid_out && has_raw_in && (has_cid_match || (!exact_match_only && has_exclude_cid_out) ) ) {
				return GF_TRUE;
			}

			if (has_raw_in != 2) has_raw_in = 0;
			if (has_cid_match != 2) has_cid_match = 0;
			if (exclude_cid_out != 2) exclude_cid_out = 0;
			if (has_exclude_cid_out != 2) has_exclude_cid_out = 0;

			continue;
		}

		if ( (cap->flags & GF_CAPFLAG_INPUT) && (cap->code == incode) ) {
			if (! (cap->flags & GF_CAPFLAG_EXCLUDED) && gf_props_equal(&cap->val, cap_input) ) {
				has_raw_in = (cap->flags & GF_CAPS_INPUT_STATIC) ? 2 : 1;
			}
		}
		if ( (cap->flags & GF_CAPFLAG_OUTPUT) && (cap->code == outcode) ) {
			if (! (cap->flags & GF_CAPFLAG_EXCLUDED)) {
				if (gf_props_equal(&cap->val, cap_output) ) {
					if (out_cap_excluded) {
						exclude_cid_out = (cap->flags & GF_CAPS_OUTPUT_STATIC) ? 2 : 1;
					} else {
						has_cid_match = (cap->flags & GF_CAPS_OUTPUT_STATIC) ? 2 : 1;
					}
				}
			} else {
				Bool prop_equal = gf_props_equal(&cap->val, cap_output);
				if (out_cap_excluded)
					prop_equal = !prop_equal;

				if (prop_equal) {
					exclude_cid_out = (cap->flags & GF_CAPS_OUTPUT_STATIC) ? 2 : 1;
				} else {
					has_exclude_cid_out = (cap->flags & GF_CAPS_OUTPUT_STATIC) ? 2 : 1;
				}
			}
		}
	}
	//CID not excluded, raw in present and CID explicit match or not included in excluded set
	if (!exclude_cid_out && has_raw_in && (has_cid_match || (!exact_match_only && has_exclude_cid_out) ) ) {
		return GF_TRUE;
	}
	return GF_FALSE;
}