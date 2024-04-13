static int key_change_login(pam_handle_t *pamh, int flags, PKCS11_SLOT *slot, const char *pin_regex)
{
	char *old = NULL, *new = NULL, *retyped = NULL;
	int ok;

	if (0 == slot->token->loginRequired) {
		/* we can't change a PIN */
		ok = 0;
		goto err;
	}
	ok = 0;

	/* We need a R/W public session to change the PIN via PUK or
	 * a R/W user session to change the PIN via PIN */
	if (0 != PKCS11_open_session(slot, 1)
			|| (0 == slot->token->userPinLocked
				&& 1 != key_login(pamh, flags, slot, pin_regex))) {
		goto err;
	}

	/* prompt for new PIN (and PUK if needed) */
	if (0 != slot->token->secureLogin) {
		if (0 != slot->token->userPinLocked) {
			prompt(flags, pamh, PAM_TEXT_INFO, NULL,
					_("Change PIN with PUK on PIN pad for %s"),
					slot->token->label);
		} else {
			prompt(flags, pamh, PAM_TEXT_INFO, NULL,
					_("Change PIN on PIN pad for %s"),
					slot->token->label);
		}
	} else {
		if (0 != slot->token->userPinLocked) {
			if (PAM_SUCCESS == prompt(flags, pamh,
						PAM_PROMPT_ECHO_OFF, &old,
						_("PUK for %s: "),
						slot->token->label)) {
				goto err;
			}
		} else {
#ifdef TEST
			/* In the tests we're calling pam_sm_chauthtok() directly, so
			 * pam_get_item(PAM_AUTHTOK) will return PAM_BAD_ITEM. As
			 * workaround, we simply enter the current PIN twice. */
			if (PAM_SUCCESS != prompt(flags, pamh,
						PAM_PROMPT_ECHO_OFF, &old,
						_("Current PIN: "))) {
				goto err;
			}
#else
			if (PAM_SUCCESS != pam_get_item(pamh, PAM_AUTHTOK, (void *)&old)
					|| NULL == old) {
				goto err;
			}
			old = strdup(old);
			if (NULL == old) {
				pam_syslog(pamh, LOG_CRIT, "strdup() failed: %s",
						strerror(errno));
				goto err;
			}
#endif
		}
		if (PAM_SUCCESS != prompt(flags, pamh,
					PAM_PROMPT_ECHO_OFF, &new,
					_("Enter new PIN: "))
				|| PAM_SUCCESS != prompt(flags, pamh,
					PAM_PROMPT_ECHO_OFF, &retyped,
					_("Retype new PIN: "))) {
			goto err;
		}
		if (0 != strcmp(new, retyped)) {
			prompt(flags, pamh, PAM_ERROR_MSG, NULL, _("PINs don't match"));
			goto err;
		}
	}

	if (0 != PKCS11_change_pin(slot, old, new)) {
		if (slot->token->userPinLocked) {
			prompt(flags, pamh, PAM_ERROR_MSG, NULL, _("PIN not changed; PIN locked"));
		} else if (slot->token->userPinFinalTry) {
			prompt(flags, pamh, PAM_ERROR_MSG, NULL, _("PIN not changed; one try remaining"));
		} else {
			prompt(flags, pamh, PAM_ERROR_MSG, NULL, _("PIN not changed"));
		}
		goto err;
	}

	pam_set_item(pamh, PAM_AUTHTOK, new);

	ok = 1;

err:
	if (NULL != retyped) {
		OPENSSL_cleanse(retyped, strlen(retyped));
		free(retyped);
	}
	if (NULL != new) {
		OPENSSL_cleanse(new, strlen(new));
		free(new);
	}
	if (NULL != old) {
		OPENSSL_cleanse(old, strlen(old));
		free(old);
	}

	return ok;
}
