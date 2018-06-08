start  /c/MongoDB/Server/3.6/bin/mongod.exe --dbpath /c/Users/Administrator/Desktop/node_red/mongodb
sleep 0.8
start  node-red
sleep 0.8
start /c/mosquitto/mosquitto.exe
sleep 0.2
start /c/mosquitto/mosquitto_sub.exe -d -t test_data/hum
start /c/mosquitto/mosquitto_sub.exe -d -t test_data/temp
start /c/mosquitto/mosquitto_sub.exe -d -t test_data/LEDchange