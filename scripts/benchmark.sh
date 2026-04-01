#!/bin/bash

echo "========================================"
echo "   BENCHMARK MQTT - Latence et Débit"
echo "========================================"

echo ""
echo "--- TEST 1 : Latence SANS TLS (port 1883) ---"
for i in $(seq 1 10); do
  { time mosquitto_pub -h localhost -p 1883 \
    -u esp32 -P esp32pass \
    -t "iot/bench" \
    -m '{"temp":24.5,"hum":62.3}'; } 2>&1 | grep real
done

echo ""
echo "--- TEST 2 : Latence AVEC TLS (port 8883) ---"
for i in $(seq 1 10); do
  { time mosquitto_pub -h localhost -p 8883 \
    --cafile /etc/mosquitto/certs/ca.crt \
    --insecure \
    -u esp32 -P esp32pass \
    -t "iot/bench" \
    -m '{"temp":24.5,"hum":62.3}'; } 2>&1 | grep real
done

echo ""
echo "--- TEST 3 : Débit SANS TLS (100 messages) ---"
time for i in $(seq 1 100); do
  mosquitto_pub -h localhost -p 1883 \
    -u esp32 -P esp32pass \
    -t "iot/bench" \
    -m '{"temp":24.5,"hum":62.3}'
done

echo ""
echo "--- TEST 4 : Débit AVEC TLS (100 messages) ---"
time for i in $(seq 1 100); do
  mosquitto_pub -h localhost -p 8883 \
    --cafile /etc/mosquitto/certs/ca.crt \
    --insecure \
    -u esp32 -P esp32pass \
    -t "iot/bench" \
    -m '{"temp":24.5,"hum":62.3}'
done

echo ""
echo "========================================"
echo "   BENCHMARK TERMINÉ"
echo "========================================"
