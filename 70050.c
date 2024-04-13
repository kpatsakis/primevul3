static void print_tags_recursive(const u8 * buf0, const u8 * buf,
				 size_t buflen, size_t depth)
{
	int r;
	size_t i;
	size_t bytesleft = buflen;
	const char *classes[4] = {
		"Universal",
		"Application",
		"Context",
		"Private"
	};
	const u8 *p = buf;

	while (bytesleft >= 2) {
		unsigned int cla = 0, tag = 0, hlen;
		const u8 *tagp = p;
		size_t len;

		r = sc_asn1_read_tag(&tagp, bytesleft, &cla, &tag, &len);
		if (r != SC_SUCCESS || tagp == NULL) {
			printf("Error in decoding.\n");
			return;
		}
		hlen = tagp - p;
		if (cla == 0 && tag == 0) {
			printf("Zero tag, finishing\n");
			break;
		}
		print_indent(depth);
		/* let i be the length of the tag in bytes */
		for (i = 1; i < sizeof tag - 1; i++) {
			if (!(tag >> 8*i))
				break;
		}
		printf("%02X", cla<<(i-1)*8 | tag);

		if ((cla & SC_ASN1_TAG_CLASS) == SC_ASN1_TAG_UNIVERSAL) {
			printf(" %s", tag2str(tag));
		} else {
			printf(" %s %-2u",
					classes[cla >> 6],
					i == 1 ? tag & SC_ASN1_TAG_PRIMITIVE : tag & (((unsigned int) ~0) >> (i-1)*8));
		}
		if (!((cla & SC_ASN1_TAG_CLASS) == SC_ASN1_TAG_UNIVERSAL
					&& tag == SC_ASN1_TAG_NULL && len == 0)) {
			printf(" (%"SC_FORMAT_LEN_SIZE_T"u byte%s)",
					len,
					len != 1 ? "s" : "");
		}

		if (len + hlen > bytesleft) {
			printf(" Illegal length!\n");
			return;
		}
		p += hlen + len;
		bytesleft -= hlen + len;

		if (cla & SC_ASN1_TAG_CONSTRUCTED) {
			putchar('\n');
			print_tags_recursive(buf0, tagp, len, depth + 2*i + 1);
			continue;
		}

		switch (tag) {
			case SC_ASN1_TAG_BIT_STRING:
				printf(": ");
				sc_asn1_print_bit_string(tagp, len, depth + 2*i + 1);
				break;
			case SC_ASN1_TAG_OCTET_STRING:
				sc_asn1_print_octet_string(tagp, len, depth + 2*i + 1);
				break;
			case SC_ASN1_TAG_OBJECT:
				printf(": ");
				sc_asn1_print_object_id(tagp, len);
				break;
			case SC_ASN1_TAG_INTEGER:
			case SC_ASN1_TAG_ENUMERATED:
				printf(": ");
				sc_asn1_print_integer(tagp, len);
				break;
			case SC_ASN1_TAG_IA5STRING:
			case SC_ASN1_TAG_PRINTABLESTRING:
			case SC_ASN1_TAG_T61STRING:
			case SC_ASN1_TAG_UTF8STRING:
				printf(": ");
				sc_asn1_print_utf8string(tagp, len);
				break;
			case SC_ASN1_TAG_BOOLEAN:
				printf(": ");
				sc_asn1_print_boolean(tagp, len);
				break;
			case SC_ASN1_GENERALIZEDTIME:
				printf(": ");
				sc_asn1_print_generalizedtime(tagp, len);
				break;
			case SC_ASN1_UTCTIME:
				printf(": ");
				sc_asn1_print_utctime(tagp, len);
				break;
		}

		if ((cla & SC_ASN1_TAG_CLASS) == SC_ASN1_TAG_APPLICATION) {
			print_hex(tagp, len, depth + 2*i + 1);
		}

		if ((cla & SC_ASN1_TAG_CLASS) == SC_ASN1_TAG_CONTEXT) {
			print_hex(tagp, len, depth + 2*i + 1);
		}

		putchar('\n');
	}
}
