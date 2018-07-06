// #include <Arduino.h>
// #include "globals.h"

// trick found here (saves memory) :
// https://circuits4you.com/2016/12/16/esp8266-web-server-html/

static const char configWebPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title></title>
  <meta charset=utf8>
  <style>
    html { height: 100%; }
    body {
      font-size: 0.75em; font-family: "Courier New", Courier, monospace;
      margin: 0px; padding: 0px; width: 100%; height: 100%; max-height: 100%;
    }
    div#wrapper { width: 100%; height: 100%; text-align: center; }
    div#title {
      display: inline-block; width: 100%;
      text-align: center !important; margin-left: auto; margin-right: auto;
    }
    div#title-container { display: inline-block; width: auto; text-align: right; }
    pre { display: inline-block; width: auto; margin: 5px 0px;}
    div#settings {
      display: inline-block; max-width: 300px;
      text-align: left; padding: 0px 10px; }
    div#btn-container { text-align: center; }
    div.wide { width: 100%; }
    label { display: block; margin-bottom: 10px; }
    label.half { display: inline-block; }
    label#left { width: 51%; }
    label > label { margin-bottom: 0px; }
    input { margin: 0px; padding: 2px; border: 0; background-color: #eee; }
    input[type=text], input[type=password], input[type=number] { width: 100%; }
    .ip { width: 25px !important; }
    input[type=number] { -moz-appearance: textfield; margin: 0; }
    input[type=number]::-webkit-inner-spin-button,
    input[type=number]::-webkit-outer-spin-button {
      -webkit-appearance: none;
      margin: 0;
    }
  </style>
  <script type=text/javascript>
  window.onload = function() {
    var $ssid = document.getElementById('ssid');
    var $password = document.getElementById('password');
    var $hostip1 = document.getElementById('hostip1');
    var $hostip2 = document.getElementById('hostip2');
    var $hostip3 = document.getElementById('hostip3');
    var $hostip4 = document.getElementById('hostip4');
    var $inputPort = document.getElementById('inputPort');
    var $outputPort = document.getElementById('outputPort');
    var $accelRange = document.getElementById('accelRange');
    var $gyroRange = document.getElementById('gyroRange');
    var $useWiFi = document.getElementById('useWiFi');
    var $useSerial = document.getElementById('useSerial');
    var $sendSingleFrame = document.getElementById('sendSingleFrame');
    var $readMagPeriod = document.getElementById('readMagPeriod');
    var $outputFramePeriod = document.getElementById('outputFramePeriod');
    var $buttonHoldDuration = document.getElementById('buttonHoldDuration');

    function getRadioValue($radio) {
      var fd = new FormData($radio);
      for (var key of fd.entries()) { return key[1]; }
    }

    var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
    connection.onopen = function() { connection.send('Connect '/* + new Date()*/);};
    connection.onerror = function(error) { console.log('WebSocket Error ', error); };
    connection.onmessage = function(e) {
      var arg;
      var args = e.data.split('\n');
      console.log(e.data);
      if (args[0] === 'settings') {
        $ssid.value = args[1];
        $password.value = args[2];

        var ip = args[3].split('.');
        if (ip.length !== 4) {
          ip = [0, 0, 0, 0];
        }
        arg = parseInt(ip[0]);
        $hostip1.value = (!isNaN(arg) && arg >= 0) ? arg : 0;
        arg = parseInt(ip[1]);
        $hostip2.value = (!isNaN(arg) && arg >= 0) ? arg : 0;
        arg = parseInt(ip[2]);
        $hostip3.value = (!isNaN(arg) && arg >= 0) ? arg: 0;
        arg = parseInt(ip[3]);
        $hostip4.value = (!isNaN(arg) && arg >= 0) ? arg: 0;

        arg = parseInt(args[4]);
        $inputPort.value = (!isNaN(arg) && arg >= 0) ? arg : 0;
        arg = parseInt(args[5]);
        $outputPort.value = (!isNaN(arg) && arg >= 0) ? arg : 0;

        arg = parseInt(args[6]);
        arg = isNaN(arg) ? 0 : (arg < 0 ? 0 : (arg > 3 ? 3 : arg));
        document.querySelector('#accelRange' + arg).checked = true;

        arg = parseInt(args[7]);
        arg = isNaN(arg) ? 0 : (arg < 0 ? 0 : (arg > 3 ? 3 : arg));
        document.querySelector('#gyroRange' + arg).checked = true;

        $useWiFi.checked = parseInt(args[8]) != 0;
        $useSerial.checked = parseInt(args[9]) != 0;
        $sendSingleFrame.checked = parseInt(args[10]) != 0;

        arg = parseInt(args[11]);
        $readMagPeriod.value = (!isNaN(arg) && arg >= 0) ? arg : 0;
        arg = parseInt(args[12]);
        $outputFramePeriod.value = (!isNaN(arg) && arg >= 0) ? arg : 0;
        arg = parseInt(args[13]);
        $buttonHoldDuration.value = (!isNaN(arg) && arg >= 0) ? arg : 0;

        var fullId = `${args[15]}@${args[14]}`;
        document.getElementById('version').innerHTML = fullId;
      }
    };

    var $updateBtn = document.getElementById('updateBtn');
    $updateBtn.addEventListener('click', function() {
      var settings = 'settings\n';
      settings += `${$ssid.value}\n${$password.value}\n`;
      settings += `${$hostip1.value}.${$hostip2.value}.${$hostip3.value}.${$hostip4.value}\n`;
      settings += `${$inputPort.value}\n${$outputPort.value}\n`;
      settings += `${getRadioValue($accelRange)}\n`;
      settings += `${getRadioValue($gyroRange)}\n`;
      settings += $useWiFi.checked ? '1\n' : '0\n';
      settings += $useSerial.checked ? '1\n' : '0\n';
      settings += $sendSingleFrame.checked ? '1\n' : '0\n';
      settings += `${$readMagPeriod.value}\n`;
      settings += `${$outputFramePeriod.value}\n`;
      settings += `${$buttonHoldDuration.value}\n`;
      console.log(settings);
      connection.send(settings);
    });

    var $clearBtn = document.getElementById('clearBtn');
    $clearBtn.addEventListener('click', function() {
      connection.send('clear\n');
    });
  };
  </script>
</head>
<body>
  <div id=wrapper>

    <div id=title>
      <div id=title-container>
      <pre>
 __   __  _______  __   __  __   __  ___   __    _  _______ 
|  |_|  ||       ||  | |  ||  | |  ||   | |  |  | ||       |
|       ||   _   ||  |_|  ||  | |  ||   | |   |_| ||   _   |
|       ||  | |  ||       ||  |_|  ||   | |       ||  | |  |
|       ||  |_|  ||       ||       ||   | |  _    ||  |_|  |
| ||_|| ||       | |     | |       ||   | | | |   ||       |
|_|   |_||_______|  |___|  |_______||___| |_|  |__||_______|
      </pre>
      <br>
      <span id=version>&nbsp;</span>
      </div>
    </div>

    <div id=settings>
      <label>network ssid <input type=text id=ssid></label>
      <label>network password <input type=password id=password></label>
      <label>
        host IP
        <div class=wide>
        <input type=number id=hostip1 class=ip>.
        <input type=number id=hostip2 class=ip>.
        <input type=number id=hostip3 class=ip>.
        <input type=number id=hostip4 class=ip>
        </div>
      </label>
      <label>input UDP port <input type=number id=inputPort></label>
      <label>output UDP port <input type=number id=outputPort></label>
      <label class=half id=left>
        accelerometer range
        <form id=accelRange>
          <label><input type=radio id=accelRange0 name=accelRange value=0> +/- 2g</label>
          <label><input type=radio id=accelRange1 name=accelRange value=1> +/- 4g</label>
          <label><input type=radio id=accelRange2 name=accelRange value=2> +/- 8g</label>
          <label><input type=radio id=accelRange3 name=accelRange value=3> +/- 16g</label>
        </form>
      </label>
      <label class=half id=right>
        gyroscope range
        <form id=gyroRange>
          <label><input type=radio id=gyroRange0 name=gyroRange value=0> +/- 250 deg/sec</label>
          <label><input type=radio id=gyroRange1 name=gyroRange value=1> +/- 500 deg/sec</label>
          <label><input type=radio id=gyroRange2 name=gyroRange value=2> +/- 1000 deg/sec</label>
          <label><input type=radio id=gyroRange3 name=gyroRange value=3> +/- 2000 deg/sec</label>
        </form>
      </label>
      <label><input type=checkbox id=useWiFi> enable WiFi on boot</label>
      <label><input type=checkbox id=useSerial> use Serial</label>
      <label><input type=checkbox id=sendSingleFrame> send single frame</label>
      <label>read magnetometer period (ms) <input type=number id=readMagPeriod></label>
      <label>output frame period (ms) <input type=number id=outputFramePeriod></label>
      <label>button hold duration (ms) <input type=number id=buttonHoldDuration></label>
      <div id=btn-container>
        <button id=updateBtn>save</button>
        <button id=clearBtn>clear</button>
      </div>
    </div>
  </div>
</body>
</html>
)=====";
