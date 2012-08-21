#!/bin/bash

file="$(readlink -e "${0}")"
dir="$(dirname "${file}")"

"${dir}/../../php-install2/bin/php" -d extension="${dir}/modules/cpptest.so" "${@}"
