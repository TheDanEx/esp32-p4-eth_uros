# Guía de Uso y Despliegue

## Propósito Arquitectónico
Este documento detalla los pasos para compilar, flashear y ejecutar el proyecto de micro-ROS en el ESP32-P4 y la Raspberry Pi 5.

## Entorno y Dependencias
- Raspberry Pi 5 conectada vía USB (para flash) y Ethernet (para ROS).
- ESP-IDF v5.5 instalado en la Pi 5.
- Docker instalado en la Pi 5 para el Agente.

## Interfaces de E/S (Inputs/Outputs)
- **Serial Port:** `/dev/ttyACM0` (ESP32-P4).
- **Network Interface:** `eth0` (IP 192.168.5.1).

## Flujo de Ejecución Lógico
1. **Compilación:** Uso del sistema `idf.py` en la Raspberry Pi 5.
2. **Flash:** Despliegue mediante el script `pi5_rebuild_flash.py`.
3. **Agente:** Inicio del contenedor Docker de micro-ROS.

## Funciones Principales y Parámetros
### Comandos de Compilación:
```bash
idf.py build
```

### Comandos de Despliegue (desde la Pi 5):
```bash
# Reiniciar el agente
sudo docker run -d --name uros_agent --net=host microros/micro-ros-agent:humble udp4 --port 8888
```

## Puntos Críticos y Depuración
- **IP Estática:** Asegúrate de que la interfaz Ethernet de la Pi 5 tenga la IP `192.168.5.1/24`.
- **Logs:** Si la conexión falla, verifica los logs del agente con `docker logs uros_agent`.
- **Reset:** El ESP32-P4 puede requerir un reset físico si el puerto USB-ACM se bloquea tras un fallo de red.

## Ejemplo de Uso e Instanciación
Para visualizar el timestamp del ESP desde la Pi 5:
```bash
ros2 topic echo /microROS/esp_time
```
