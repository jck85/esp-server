#!/bin/bash

echo "building html"

INDEX_HTML="esp_server/public/index.html"
INDEX_HEADER="esp_server/server/index.h"

echo "const char index_html[] PROGMEM = R\"rawliteral(" > $INDEX_HEADER
cat $INDEX_HTML >> $INDEX_HEADER
echo ")rawliteral\";" >> $INDEX_HEADER