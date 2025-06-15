#!/usr/bin/env bash
set -e
ANTLR_JAR=tools/antlr/antlr-4.13.0-complete.jar
OUTDIR=sere/fe/antlr
mkdir -p "$OUTDIR"
java -Xmx1G -cp "$ANTLR_JAR" org.antlr.v4.Tool \
  -Dlanguage=Python3 \
  -visitor \
  -o "$OUTDIR" \
  sere/fe/SereLexer.g4 sere/fe/SereParser.g4
