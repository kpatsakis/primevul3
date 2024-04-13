static int key_find(pam_handle_t *pamh, int flags, const char *user,
		PKCS11_CTX *ctx, PKCS11_SLOT *slots, unsigned int nslots,
		PKCS11_SLOT **authslot, PKCS11_KEY **authkey)
{
	int token_found = 0;

	if (NULL == authslot || NULL == authkey) {
		return 0;
	}

	*authkey = NULL;
	*authslot = NULL;

	/* search all valuable slots for a key that is authorized by the user */
	while (0 < nslots) {
		PKCS11_SLOT *slot = NULL;
		PKCS11_CERT *certs = NULL;
#ifdef HAVE_PKCS11_ENUMERATE_PUBLIC_KEYS
		PKCS11_KEY *keys = NULL;
#endif
		unsigned int count = 0;

		slot = PKCS11_find_token(ctx, slots, nslots);
		if (NULL == slot || NULL == slot->token) {
			break;
		}
		token_found = 1;

		if (slot->token->loginRequired && slot->token->userPinLocked) {
			pam_syslog(pamh, LOG_DEBUG, "%s: PIN locked",
					slot->token->label);
			continue;
		}
		pam_syslog(pamh, LOG_DEBUG, "Searching %s for keys",
				slot->token->label);

#ifdef HAVE_PKCS11_ENUMERATE_PUBLIC_KEYS
		/* First, search all available public keys to allow using tokens
		 * without certificates (e.g. OpenPGP card) */
		if (0 == PKCS11_enumerate_public_keys(slot->token, &keys, &count)) {
			while (0 < count && NULL != keys) {
				EVP_PKEY *pubkey = PKCS11_get_public_key(keys);
				int r = match_user_opensc(pubkey, user);
				if (1 != r) {
					r = match_user_openssh(pubkey, user);
				}
				if (NULL != pubkey) {
					EVP_PKEY_free(pubkey);
				}
				if (1 == r) {
					*authkey = keys;
					*authslot = slot;
					pam_syslog(pamh, LOG_DEBUG, "Found %s",
							keys->label);
					return 1;
				}

				/* Try the next possible public key */
				keys++;
				count--;
			}
		}
#endif

		/* Next, search all certificates */
		if (0 == PKCS11_enumerate_certs(slot->token, &certs, &count)) {
			while (0 < count && NULL != certs) {
				EVP_PKEY *pubkey = X509_get_pubkey(certs->x509);
				int r = match_user_opensc(pubkey, user);
				if (1 != r) {
					r = match_user_openssh(pubkey, user);
				}
				if (NULL != pubkey) {
					EVP_PKEY_free(pubkey);
				}
				if (1 == r) {
					*authkey = PKCS11_find_key(certs);
					if (NULL == *authkey) {
						continue;
					}
					*authslot = slot;
					pam_syslog(pamh, LOG_DEBUG, "Found %s",
							certs->label);
					return 1;
				}

				/* Try the next possible certificate */
				certs++;
				count--;
			}
		}

		/* Try the next possible slot: PKCS11 slots are implemented as array,
		 * so starting to look at slot++ and decrementing nslots accordingly
		 * will search the rest of slots. */
		slot++;
		nslots -= (slot - slots);
		slots = slot;
		pam_syslog(pamh, LOG_DEBUG, "No authorized key found");
	}

	if (0 == token_found) {
		prompt(flags, pamh, PAM_ERROR_MSG , NULL, _("No token found"));
	} else {
		prompt(flags, pamh, PAM_ERROR_MSG , NULL, _("No authorized keys on token"));
	}

	return 0;
}
