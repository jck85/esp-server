const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: monospace;
        }

        body {
            margin: 0;
            background-color: grey;
        }

        header {
            display: flex;
            align-items: center;
        }

        header > h2 {
            font-size: 3.0rem;
        }

        p {
            font-size: 3.0rem;
        }

        .switch {
            position: relative;
            display: inline-block;
            width: 120px;
            height: 68px
        }

        .switch input {
            display: none
        }

        .slider {
            position: absolute;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            border-radius: 6px
        }

        .slider:before {
            position: absolute;
            content: "";
            height: 52px;
            width: 52px;
            left: 8px;
            bottom: 8px;
            background-color: white;
            -webkit-transition: .4s;
            transition: .4s;
            border-radius: 3px
        }

        input:checked+.slider {
            background-color: green;
        }

        input:checked+.slider:before {
            -webkit-transform: translateX(52px);
            -ms-transform: translateX(52px);
            transform: translateX(52px)
        }

    </style>
</head>

<body>
    <header>
        <h2>ESP Web Server</h2>
    </header>

    <div id="button-template">
        %BUTTON_TEMPALTE%
    <div>

    <div id="gpio">
        <label class="switch">
            GPIO 4
            <input type="checkbox" onchange="toggleCheckbox(this)" id="4">
            <span class="slider"></span>
        </label>

        <label class="switch">
        GPIO 5
            <input type="checkbox" onchange="toggleCheckbox(this)" id="5">
            <span class="slider"></span>
        </label>

        <label class="switch">
            GPIO 6
            <input type="checkbox" onchange="toggleCheckbox(this)" id="6">
            <span class="slider"></span>
        </label>"
    </div>

    <script>
        function toggleCheckbox(element) {
            console.log("checkbox: ", element.id, element.checked);

            var xhr = new XMLHttpRequest();
            
            let state = element.checked ? "1" : "0";

            xhr.open("GET", "/update?gpio=" + element.id + "&state=" + state, true);

            xhr.onload = () => {
                if (xhr.status === 200) {
                    const res = xhr.responseText;
                    const data = JSON.parse(res);
                    console.log("response: ", res);
                } else {
                    console.log("request failed");
                }
            }

            xhr.send();

        }
    </script>
</body>

</html>
)rawliteral";