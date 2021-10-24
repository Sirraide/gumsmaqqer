#!/usr/bin/env bash

info() {
  echo -e "\033[33m$1\033[0m"
}

die() {
	echo -e "\033[31m$1\033[0m"
	echo -e "\033[31mABORTED AND SUNKEN INTO DESPAIR\033[0m"
	exit 1
}

ensure_dir() {
  if [[ -d $1 ]]; then
  	info "FOUND OLD DIRECTORY $1. ERASING..."
  	rm -rf "$1"
  fi

  mkdir "$1"
}

set -eu

if [[ $# -gt 0 ]]; then
	if [[ "$1" == "install" ]]; then
	  	if [[ $EUID -ne 0 ]]; then 
			die 'ERROR: MUST NEEDS EXECUTE SCRIPT AS ROOT WHEN USING \033[33minstall\033[31m!'
		fi
	  	if [ ! -f 'gumsmaqqer' ]; then 
			die 'ERROR: MUST NEEDS COMPILE THE GUMSMAQQER BEFORE INSTALLATION.\nDO RUN \033[33m./build.sh\033[31m WITHOUT ANY ARGUMENTS!'
		fi

	  	chmod +x gumsmaqqer
	  	cp gumsmaqqer /bin/gumsmaqqer
		cp libgumsmaq.so /lib/libgumsmaq.so
	  	info 'IMPORTANT: MUST NEEDS CREATE ENVIRONMENT VARIABLE \033[32mGUMSMAQQER_ASSETS_DIR\033[33m POINTING TO ASSETS FOLDER CONTAINING ASSETS!'
	  	exit
  	fi

	die "UNKNOWN OPTION '$1'"
fi

ensure_dir ./build
cd ./build


info 'GENERATING BUILD FILES...'
cmake -DCMAKE_BUILD_TYPE=Release ..

info 'COMPILING...'
make

info 'CAN NOW USE THE GUMSMAQQER \033[32m./gumsmaqqer\033[33m!'
info 'MAY NOW RUN \033[32m./build.sh install\033[33m TO INSTALL THE GUMSMAQQER!'
