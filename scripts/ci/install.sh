#!/usr/bin/env bash
set -e

# Determine the OF_ROOT based on the location of this file.
OF_ROOT=$( cd "$(dirname "$0")/../../../.." ; pwd -P )

# Default addon github info.
GH_USERNAME='bakercp'
GH_BRANCH='master'
GH_DEPTH=1




TRAVIS=true
APPVEYOR=True

# ${TRAVIS_BRANCH}
# ${APPVEYOR_REPO_BRANCH}
#
# ${TRAVIS_REPO_SLUG}
# ${APPVEYOR_REPO_NAME}


# An array of required addons that will be gathered below.
REQUIRED_ADDONS=()

# Extract ADDON_DEPENDENCIES from addon_config.mk file.
if [ -f ${OF_ROOT}/addons/${OF_ADDON_NAME}/addon_config.mk ]; then
  while read line; do
    if [[ $line == ADDON_DEPENDENCIES* ]] ;
    then
      line=${line#*=}
      IFS=' ' read -ra ADDR <<< "$line"
      for i in "${ADDR[@]}"; do
          REQUIRED_ADDONS+=($i)
      done
    fi
  done < ${OF_ROOT}/addons/${OF_ADDON_NAME}/addon_config.mk
fi

# Gather addons from all examples.
for addons_make in ${OF_ROOT}/addons/${OF_ADDON_NAME}/example*/addons.make; do
  while read addon; do
    if [ ${addon} != ${OF_ADDON_NAME} ] ;
    then
      REQUIRED_ADDONS+=($addon)
    fi
  done < $addons_make
done

# We aren't de-duplicating array to keep it pure bash.
# If the addon already exists, it will be skipped.
for addon in "${REQUIRED_ADDONS[@]}"
do
  if [ ! -d ${OF_ROOT}/addons/${addon} ]; then
    git clone --depth=$GH_DEPTH https://github.com/$GH_USERNAME/$addon.git ${OF_ROOT}/addons/${addon}

    # Initialize any other addons.
    if [ -d ${OF_ROOT}/addons/${addon}/scripts/ci/install.sh ]; then
        ${OF_ROOT}/addons/${addon}/scripts/ci/install.sh
    fi
  fi
done
