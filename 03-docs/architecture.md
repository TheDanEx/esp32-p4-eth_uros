# Arquitectura del Sistema: ESP32-P4-ETH micro-ROS

## Propósito Arquitectónico
Este proyecto implementa una capa de transporte de alta velocidad para ROS 2 sobre hardware ESP32-P4, utilizando el EMAC interno y un PHY Ethernet (IP101GRI). El objetivo es proporcionar una comunicación determinista y de baja latencia entre el microcontrolador y un agente de micro-ROS en un sistema de control robótico distribuido.

## Entorno y Dependencias
- **Framework:** ESP-IDF v5.5.2 (Soporte RISC-V P4).
- **Middleware:** micro-ROS (XRCE-DDS) rama `humble`.
- **Hardware:** Waveshare ESP32-P4-ETH (Silicon v1.3 compatible).
- **Red:** Ethernet RMII (Reloj de entrada en GPIO 50).
- **Agente:** Raspberry Pi 5 con ROS 2 Humble/Jazzy.

## Interfaces de E/S (Inputs/Outputs)
### Publicaciones (Outputs):
- `/microROS/ping` (`std_msgs/msg/Header`): Latencia y sincronización.
- `/microROS/esp_time` (`std_msgs/msg/Int64`): Timestamp de alta precisión (`esp_timer_get_time()`).
- `/microROS/pong` (`std_msgs/msg/Header`): Respuesta a subscripciones.

### Subscripciones (Inputs):
- `/microROS/ping`: Escucha mensajes de otros nodos.
- `/microROS/pong`: Confirmación de recepción.

## Flujo de Ejecución Lógico
1. **HW Reset:** Ciclo de reset físico del PHY Ethernet (GPIO 51).
2. **Netif Init:** Inicialización del stack TCP/IP (LwIP) con IP estática.
3. **uROS Transport:** Configuración del transporte UDP hacia el Agente.
4. **XRCE Session:** Negociación de sesión con el Agente micro-ROS.
5. **Entity Creation:** Registro de nodos, tópicos, publishers y suscriptores.
6. **Main Loop:** Ejecución del `rclc_executor` para gestionar eventos asíncronos.

## Funciones Principales y Parámetros
- `uros_network_interface_initialize()`: Configura el MAC y PHY específicos del P4.
- `micro_ros_task()`: Tarea de FreeRTOS que gestiona el ciclo de vida de ROS 2.
- `rmw_uros_options_set_udp_address()`: Define la ruta hacia el Agente.

## Puntos Críticos y Depuración
- **Reloj RMII:** El ESP32-P4 DEBE estar configurado como `CLK_INPUT` en el GPIO 50 para esta placa.
- **Atomic Workaround:** Es necesario parchear `librcutils.a` para evitar conflictos de símbolos atómicos de 64 bits en la arquitectura RISC-V.
- **Silicon v1.3:** Se debe activar `CONFIG_ESP32P4_SELECTS_REV_LESS_V3` para evitar excepciones de instrucción ilegal.

## Ejemplo de Uso e Instanciación
```c
void app_main(void) {
    ESP_ERROR_CHECK(uros_network_interface_initialize());
    xTaskCreate(micro_ros_task, "uros_task", 8192, NULL, 5, NULL);
}
```
