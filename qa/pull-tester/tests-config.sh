#!/bin/bash
# Copyright (c) 2013-2014 The FacileCoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

BUILDDIR="/opt/clean_build/facilecoin-0.10.2"
EXEEXT=""

# These will turn into comments if they were disabled when configuring.
ENABLE_WALLET=1
ENABLE_UTILS=1
ENABLE_BITCOIND=1

REAL_BITCOIND="$BUILDDIR/src/facilecoind${EXEEXT}"
REAL_BITCOINCLI="$BUILDDIR/src/facilecoin-cli${EXEEXT}"

