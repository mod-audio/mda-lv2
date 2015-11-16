#!/bin/bash

set -e
cd bundles
for i in $(ls); do
  cd $i;
  if [ -f manifest.ttl.in ]; then
    ln -s manifest.ttl.in manifest.ttl;
  fi;
  cd ..;
done
