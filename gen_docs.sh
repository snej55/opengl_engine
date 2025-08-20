#!/usr/bin/bash
cd media/docs || exit; perl -ne 's/^\[(.+)\].*/`cat $1`/e;print' README.md > ../../README.md