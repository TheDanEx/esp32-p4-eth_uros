# Desarrollo de Nodos y Tópicos

## Propósito Arquitectónico
Guía de referencia para la creación de lógica de control y telemetría utilizando la API RCLC.

## Entorno y Dependencias
- Librerías `rcl`, `rclc` y `std_msgs`.

## Interfaces de E/S (Inputs/Outputs)
- **Tópicos:** Uso de tipos de mensaje estándar de ROS 2.
- **Servicios:** Implementación de callbacks para peticiones/respuestas.

## Flujo de Ejecución Lógico
1. Crear `init_options` y configurar el transporte.
2. Inicializar `support` con las opciones.
3. Crear el `node`.
4. Definir y crear `publishers` / `subscribers`.
5. Inicializar el `executor`.
6. Añadir los tópicos al `executor`.

## Funciones Principales y Parámetros
### Ejemplo de Creación de un Nodo:
```c
rcl_node_t node = rcl_get_zero_initialized_node();
RCCHECK(rclc_node_init_default(&node, "robot_sensor_node", "", &support));
```

### Ejemplo de Publisher de Telemetría:
```c
RCCHECK(rclc_publisher_init_default(&my_pub, &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32), "/robot/battery"));
```

## Puntos Críticos y Depuración
- **Stack Size:** Las tareas de micro-ROS requieren al menos 8KB de stack (`CONFIG_MICRO_ROS_APP_STACK=8192`).
- **Executor handles:** Asegúrate de que el tamaño del executor coincida con el número de suscriptores y timers añadidos.

## Ejemplo de Uso e Instanciación
Consulta el archivo `examples/ping_pong/main/main.c` para una implementación completa de referencia con 4 handles activos.
