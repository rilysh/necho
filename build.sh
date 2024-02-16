#!/usr/bin/env sh

# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.

# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# For more information, please refer to <https://unlicense.org>

PROGRAM=necho
CFLAGS="-O2 -Wall -Wextra -s"

check_file() {
    test -f $PROGRAM
    if [ $? = 0 ]
    then
	while true
	do
	      printf "re-build? (Yy/Nn): "
	      read yn
	      case $yn in
		  Y|y) build && exit 1 ;;
		  N|n) exit 1 ;;
		  *) ;;
	      esac
	done
    fi
}

build() {
    command -v cc 1>/dev/null
    if [ $? = 0 ]
    then
	cc $CFLAGS $PROGRAM.c -o $PROGRAM
    else
	printf "error: no C compiler was found.\n"
    fi
}

clean() {
    rm -f $PROGRAM
}

do_tests() {
    test -f $PROGRAM
    if [ $? = 1 ]
    then
	printf "error: $PROGRAM does not exists.\n"
	exit 1
    fi

    ./$PROGRAM Hello world
    ./$PROGRAM -n Hello world
    ./$PROGRAM " "with no newline
    ./$PROGRAM Hello wo\\crld
    ./$PROGRAM Hello \\thorizontal tab
    ./$PROGRAM Hello \\vverticle tab
    ./$PROGRAM Hello octal \\0123
}

main() {
    if [ "$1" = '' ]; then
	printf "error: no command was provided. (e.g. build, clean, etc.)\n"
    else
	case "$1" in
	    build) check_file && build ;;
	    clean) clean ;;
	    tests) do_tests ;;
	    *) printf "error: unknown command.\n"
	esac
    fi
}

main "$1"
