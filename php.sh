#!/bin/bash

file="$(readlink -e "${0}")"
dir="$(dirname "${file}")"

php -d extension="${dir}/modules/cpptest.so" "${@}"
