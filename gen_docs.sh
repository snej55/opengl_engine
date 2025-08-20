#!/usr/bin/bash
cd docs || exit; perl -ne 's/^\[(.+)\].*/`cat $1`/e;print' README.md > ../README.md