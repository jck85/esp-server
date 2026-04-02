const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
  <head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="icon" href="data:," />

    <style>
      /* Default Styles */
      html {
        box-sizing: border-box;
        font-size: 100%;
        font-family: monospace;
        color: black;
      }

      *,
      *:before,
      *:after {
        box-sizing: inherit;
        font-family: inherit;
      }

      body {
        margin: 0;
      }

      h1,
      h2,
      h3,
      h4,
      h5,
      h6,
      p {
        margin: 0;
      }

      a {
        text-decoration: none;
        color: inherit;
      }

      button {
        border: 2px solid black;
        background: white;
      }

      a:hover,
      button:hover,
      input:hover {
        cursor: pointer;
      }

      button:active {
        background-color: green;
        transform: scale(0.95);
      }

      nav {
        border: 2px solid black;
        background-color: darkgrey;
        display: flex;
        padding-left: 2rem;
      }

      /* App Styling*/
      .app {
        border: 4px solid black;
        margin: auto;
        margin-top: 2rem;
        max-width: 60rem;
        display: flex;
        flex-direction: column;
      }

      /* Esp Gui */
      .esp-gui {
        border: 2px solid black;
        padding: 1rem;
        display: flex;
        flex-direction: column;
        gap: 1rem;
      }

      .esp-ws-control {
        border: 2px solid black;
        display: flex;
        flex-direction: column;
        padding: 1rem;
      }

      .esp-ws-button {
        border: 2px solid black;
        background-color: white;
        width: 6rem;
        height: 2rem;
      }

      .esp-gpio-control {
        border: 2px solid black;
        display: flex;
        flex-direction: column;
        gap: 1rem;
        padding: 1rem;
      }

      .esp-gpio-row {
        display: flex;
        align-items: center;
        gap: 1rem;
      }

      .gpio-button {
        border: 2px solid black;
        width: 2rem;
        height: 2rem;
      }

      .switch {
        border: 2px solid black;
        width: 3rem;
        height: 2rem;
        position: relative;
        display: inline-block;
      }

      .switch > label {
        position: absolute;
        margin: auto;
        padding-left: 25%;
        width: 100%;
        height: 100%;
        display: flex;
        align-items: center;
        z-index: 1;
      }

      .switch > input {
        display: none;
        position: absolute;
        left: 2rem;
      }

      .switch > span {
        position: absolute;
        content: "";
        left: -1.5px;
        top: -1.5px;
        border: 2px solid black;
        background-color: white;
        width: calc(50% + 3px);
        height: calc(100% + 3px);
        -webkit-transition: 0.3s;
        transition: 0.3s;
        display: flex;
        align-items: center;
        justify-content: center;
      }

      .switch:has(input[type="checkbox"]:checked) {
        background-color: green;
      }

      .switch-input:checked + span {
        transform: translateX(calc(75% + 3px));
        -webkit-transform: translateX(calc(75% + 3px));
        -ms-transform: translateX(calc(75% + 3px));
      }

      .switch:hover * {
        cursor: pointer;
      }
    </style>
  </head>

  <body>
    <main class="app">
      <nav>
        <h2>ESP Web Server</h2>
      </nav>

      <div id="esp-gui" class="esp-gui">
        <div class="esp-ws-control">
          <p>Web Socket Status: <span id="ws-status">Closed</span></p>
          <button id="ws-send" class="esp-ws-button" onclick="ws_send(this)">
            TOGGLE
          </button>
        </div>

        <div class="esp-gpio-control">
          <p>GPIO Controls</p>

          <div class="esp-gpio-row">
            <p>GPIO 4</p>
            <button
              id="button-4"
              class="gpio-button"
              onclick="button_pressed(this)"
            >
              4
            </button>

            <div class="switch gpio-switch">
              <label for="switch-4" class="switch-label"></label>
              <input
                id="switch-4"
                class="switch-input"
                type="checkbox"
                onchange="switch_toggle(this)"
              />
              <span class="slider">4</span>
            </div>
          </div>

          <div class="esp-gpio-row">
            <p>GPIO 5</p>
            <button
              id="button-5"
              class="gpio-button"
              onclick="button_pressed(this)"
            >
              5
            </button>

            <div class="switch gpio-switch">
              <label for="switch-5" class="switch-label"></label>
              <input
                id="switch-5"
                class="switch-input"
                type="checkbox"
                onchange="switch_toggle(this)"
              />
              <span class="slider">5</span>
            </div>
          </div>
        </div>
      </div>
    </main>

    <script>
      // params is an object in which each key/value pair
      // will be used for the query params in the xml request
      function esp_request(endpoint, params = {}) {
        var xhr = new XMLHttpRequest();

        let url_string = "/" + endpoint;

        let keys = Object.keys(params);

        if (keys.length !== 0) {
          url_string += "?";
          for (let i = 0; i < keys.length; i++) {
            let key = keys[i];
            let value = params[key];

            url_string += key + "=" + value;

            if (i !== keys.length - 1) {
              url_string += "&";
            }
          }
        }

        xhr.open("GET", url_string, true);

        xhr.onload = () => {
          if (xhr.status === 200) {
            const res = xhr.responseText;
            const data = JSON.parse(res);
            console.log("response: ", res);
          } else {
            console.log("request failed");
          }
        };

        xhr.send();
      }

      function button_pressed(element) {
        let params = {
          id: element.id,
          gpio: element.id.split("-")[1],
          state: "pressed",
        };

        esp_request("update", params);
      }

      function switch_toggle(element) {
        let state = element.checked ? "1" : "0";
        let params = {
          id: element.id,
          gpio: element.id.split("-")[1],
          state: state,
        };

        esp_request("update", params);
      }

      // Web Sockets
      // http://10.227.22.89:8080/
      // const ws_gateway = `ws://192.168.1.101:8080/ws`;
      const ws_gateway = `ws://10.227.22.89:8080/ws`;

      let ws;

      function ws_init() {
        console.log("Initializing Web Socket");
        ws = new WebSocket(ws_gateway);
        ws.onopen = wsOnOpen;
        ws.onclose = wsOnClose;
        ws.onmessage = wsOnMessage;
      }

      function wsOnOpen() {
        console.log("Web Socket Connection Open");
      }

      function wsOnClose() {
        console.log("Web Socket Connection Closed");
      }

      function wsOnMessage(event) {
        console.log("ws event: ", event.data);
        let state = "";
        if (event.data == "1") {
          state = "Open";
        } else {
          state = "Closed";
        }
        document.getElementById("ws-status").innerHTML = state;
      }

      function ws_send() {
        ws.send("toggle");
      }

      window.onload = (event) => {
        ws_init();
      };
    </script>
  </body>
</html>
)rawliteral";
