/*
 * Copyright (c) 2013 William Pitcock <nenolod@dereferenced.org>.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "libecdsaauth/keypair.h"
#include "tool-applet.h"

static int tool_keygen(int argc, const char *argv[])
{
	FILE *pubout;
	libecdsaauth_key_t *key;
	char *pubkey;

	if (argv[1] == NULL)
	{
		fprintf(stderr, "usage: ecdsatool keygen privatekey.pem\n");
		return EXIT_FAILURE;
	}

	key = libecdsaauth_key_new();

	pubout = fopen(argv[1], "w");
	PEM_write_ECPrivateKey(pubout, key->eckey, NULL, NULL, 0, NULL);
	fclose(pubout);

	pubkey = libecdsaauth_key_public_key_base64(key);
	printf("%s\n", pubkey);
	free(pubkey);

	return EXIT_SUCCESS;
}

static int tool_pubkey(int argc, const char *argv[])
{
	libecdsaauth_key_t *key;
	char *pubkey;

	if (argv[1] == NULL)
	{
		fprintf(stderr, "usage: ecdsatool pubkey privatekey.pem\n");
		return EXIT_FAILURE;
	}

	key = libecdsaauth_key_load(argv[1]);

	pubkey = libecdsaauth_key_public_key_base64(key);
	printf("%s\n", pubkey);
	free(pubkey);

	libecdsaauth_key_free(key);

	return EXIT_SUCCESS;
}

/**************************************************************************************/

static tool_applet_t tool_applets[] = {
	{"keygen", tool_keygen},
	{"pubkey", tool_pubkey},
	{NULL, NULL}
};

static inline tool_applet_t *tool_applet_find(const char *name)
{
	tool_applet_t *iter;

	for (iter = tool_applets; iter->name != NULL; iter++)
	{
		if (!strcmp(iter->name, name))
			return iter;
	}

	return NULL;
}

int main(int argc, const char *argv[])
{
	tool_applet_t *tool;

	if (argv[1] == NULL)
		argv[1] = "usage";

	tool = tool_applet_find(argv[1]);
	if (tool == NULL)
	{
		fprintf(stderr, "%s: not implemented\n", argv[1]);
		return EXIT_FAILURE;
	}

	return tool->main(argc - 1, argv + 1);
}
