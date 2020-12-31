#!/bin/bash
# DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)"
# rlwrap ${QBIN} ${DIR}/debug.q -appdir ${DIR} -p 5000

DIR=/home/ghlian/CODE_LIAN/code_kdb/QInteractiveBrokers/app
q ${DIR}/debug.q -appdir ${DIR} -p 5000
