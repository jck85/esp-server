const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
    <title>esp_server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style> 
        html {
            font-family: monospace;
            font-size: 100%;
        }

        body {
            margin: 0;
        }
    </style>
</head>

<body>
    <h2>ESP Server</h2>

    <script>
        console.log("esp_server")
    </script>
</body>

</html>
)rawliteral";