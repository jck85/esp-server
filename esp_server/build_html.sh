#!/bin/bash

echo "building html"

echo "const char test_html[] PROGMEM = R\"rawliteral(" > test.h
cat test.html >> test.h
echo ")rawliteral\";" >> test.h
