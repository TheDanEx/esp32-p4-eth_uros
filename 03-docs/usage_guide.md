# Guía de Uso y Despliegue

## Propósito Arquitectónico
Este documento detalla los pasos para compilar, flashear y ejecutar el proyecto de micro-ROS en el ESP32-P4 y la Raspberry Pi 5.

## Entorno y Dependencias
- Raspberry Pi 5 conectada vía USB (para flash) y Ethernet (para ROS).
- ESP-IDF v5.4 (Estable) instalado en la Pi 5.
- Docker instalado en la Pi 5 para el Agente.

## Interfaces de E/S (Inputs/Outputs)
- **Serial Port:** `/dev/ttyACM0` (ESP32-P4).
- **Network Interface:** `eth0` (IP 192.168.5.1).

## Ejemplos Disponibles

### 1. Telemetría Base (`esp_p4_telemetry`)
Ejemplo ligero que envía el tiempo del sistema cada 2 segundos. Ideal para verificar conectividad básica.
```bash
cd examples/esp_p4_telemetry
idf.py build
idf.py flash
```

### 2. Sincronización y Diagnóstico (`esp_p4_ptp_telemetry`)
Ejemplo avanzado que sincroniza el reloj del ESP32 con la Pi 5 y mide el rendimiento de red.
```bash
cd examples/esp_p4_ptp_telemetry
idf.py build
idf.py flash
```
**Métricas en tiempo real:**
- **Latency:** Tiempo de viaje de ida y vuelta (RTT/2).
- **Offset:** Desfase temporal respecto a la Pi 5 (precisión microsegundos).
- **Jitter:** Estabilidad y varianza de la red Ethernet.

## Comandos de Verificación (desde la Pi 5)

Para ver el diagnóstico avanzado:
```bash
sudo docker run -it --rm --net=host ros:humble-ros-base ros2 topic echo /microROS/esp_diag_time
```

## Puntos Críticos y Depuración
- **IP Estática:** Asegúrate de que la interfaz Ethernet de la Pi 5 tenga la IP `192.168.5.1/24`.
- **Sincronización:** El nodo de diagnóstico requiere unos segundos tras el arranque para estabilizar el Offset.
- **Reset:** El ESP32-P4 puede requerir un reset físico si el puerto USB-ACM se bloquea tras un fallo de red.

---
*Desarrollado para sistemas robóticos de alta precisión.*
