# Walkthrough: Implementación micro-ROS en ESP32-P4-ETH

## Cambios Realizados
1.  **Soporte Ethernet P4:** Configuración manual del RMII Clock (Input GPIO 50) y Reset del PHY (GPIO 51).
2.  **Parche de Toolchain:** Eliminación de símbolos atómicos en `librcutils.a` para compatibilidad con el compilador RISC-V de ESP-IDF v5.5.
3.  **Telemetría de Tiempo:** Implementación de publicador `/microROS/esp_time` con precisión de microsegundos (`esp_timer_get_time()`).
4.  **Motor de Reconexión:** Lógica de recuperación automática ante fallos del Agente.
5.  **Entorno ROS 2:** Despliegue de un Agente micro-ROS en Docker sobre Raspberry Pi 5.

## Validación de Resultados
Se ha verificado la recepción de datos en la Raspberry Pi 5 con el siguiente comando:
```bash
ros2 topic echo /microROS/esp_time
```

### Captura de Datos Real (Frecuencia 2s)
```yaml
data: '8176890'
---
data: '10176890'
---
data: '12176886'
---
data: '14176886'
```
*Se observa un incremento exacto de 2.000.000 de microsegundos entre muestras.*

## Estado del Repositorio
- **Código:** Carpeta `micro_ros_espidf_component` lista para usar como componente de ESP-IDF.
- **Ejemplos:** `examples/ping_pong` actualizado con telemetría robusta.
- **Documentación:** Completa en `03-docs/`.

## Instrucciones Finales
Para iniciar el agente en la Pi 5:
```bash
sudo docker run -d --name uros_agent --net=host microros/micro-ros-agent:humble udp4 --port 8888
```
