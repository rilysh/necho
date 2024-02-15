/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 */

#include <stdio.h>

static void necho_posix_echo(char **argv)
{
	unsigned char *arg, c;

	while (*argv) {
		/* Get the first argument. */
		arg = (unsigned char *)argv[0];
	        do {
			/* Loop through it, and if it ends,
			   exit from this child loop. */
			c = *arg++;
			if (c == '\0')
			        break;

			if (c == '\\' && *arg) {
				c = *arg++;
			        switch (c) {
				case 'a':
					/* Write an <alert>. */
					c = '\a';
					break;

				case 'b':
					/* Write a <backspace>. */
					c = '\b';
					break;

				case 'c':
					/* Supress the <newline> as according to
					   POSIX, by default echo shall print a
					   newline, and, however, "\c" means, end-
					   ing a statement. */
					return;

				case 'f':
					/* Write a <form-feed>. */
					c = '\f';
					break;

				case 'n':
					/* Write a <newline>. */
					c = '\n';
					break;

				case 'r':
					/* Write a <carriage-return>. */
					c = '\r';
					break;

				case 't':
					/* Write a <tab>. */
					c = '\t';
					break;
				
				case 'v':
					/* Write a <vertical-tab>. */
					c = '\v';
					break;

				case '0':
					/* Convert a octal number to a,
					   character, and write it. */
				        if (*arg >= '0' && *arg <= '7') {
						/* Keep the next value. */
						c = *arg++;
						if (c >= '0' && c <= '7') {
							c -= '0';
							c = (c * 8 + (*arg++ - '0'));
						}
						if (*arg >= '0' && *arg <= '7')
							/* Next check and convert from octal. */
							c = (c * 8 + (*arg++ - '0'));
					}
				        break;

				case '\\':
					/* Write a <backslash> character. */
				default:
					fputc('\\', stdout);
				        break;
				}
			}

			fputc(c, stdout);
		} while (*arg);

		/* Next argument. */
		argv++;

		/* If we have an argument (next argument) that is
		   non-empty, add a space in between the previous
		   argument and the next argument. */
	        if (*argv)
			fputc(' ', stdout);
	}
}

int main(int argc, char **argv)
{
	int has_args;

        has_args = 0;
	/* If we only pass "-", ignore it, and print
	   a newline. */
	if (argc > 1) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'n' && argv[1][2] == '\0') {
			        has_args = 1;
			        goto posix_echo;
			}
		}
	}

posix_echo:
	/* If has_args is true, cut 2 arguments
	   (e.g. necho -n; # and cut these two),
	   otherwise the first argument (e.g.
	   necho Hello world; # and cut the first
	   one). */
	argv += has_args ? 2 : 1;
	necho_posix_echo(argv);

	if (has_args == 0)
		fputc('\n', stdout);
}
