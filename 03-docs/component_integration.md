# Integración como Componente ESP-IDF

## Propósito Arquitectónico
Instrucciones para desacoplar y reutilizar el driver de Ethernet micro-ROS en otros proyectos de robótica.

## Entorno y Dependencias
- Proyecto base ESP-IDF v5.x.
- Acceso a los archivos de `micro_ros_espidf_component`.

## Interfaces de E/S (Inputs/Outputs)
- **API C:** `uros_network_interface_initialize()`
- **Kconfig:** Configuración de pines SMI y Reloj RMII.

## Flujo de Ejecución Lógico
1. Copiar la carpeta del componente al directorio `components/` de tu proyecto.
2. Añadir las dependencias al `CMakeLists.txt` del componente.
3. Configurar los pines mediante `menuconfig`.
4. Llamar a la inicialización antes de la tarea de ROS 2.

## Funciones Principales y Parámetros
### Estructura de archivos necesaria:
- `micro_ros_espidf_component/`
  - `network_interfaces/uros_ethernet_netif.c`
  - `esp32_toolchain.cmake.in` (Crucial para RISC-V/P4)

## Puntos Críticos y Depuración
- **CMake Toolchain:** Es VITAL incluir el `.cmake.in` modificado para que el compilador use el ABI de hardware floating point (`ilp32f`) del P4, o el enlazado de micro-ROS fallará.
- **Kconfig Integration:** Asegúrate de que tu `sdkconfig` tenga habilitado el soporte Ethernet global (`CONFIG_ETH_ENABLED`).

## Ejemplo de Uso e Instanciación
En tu `main/CMakeLists.txt`:
```cmake
idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES micro_ros_espidf_component esp_netif esp_eth)
```
