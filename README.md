# ESP32-P4 Ethernet micro-ROS Bridge

Este repositorio contiene un componente de **micro-ROS** altamente optimizado para el **ESP32-P4**, permitiendo la comunicación nativa con **ROS 2** a través de **Ethernet RMII**.

## 🚀 Características Principales
- **Hardware:** Soporte nativo para Waveshare ESP32-P4-ETH.
- **Protocolo:** XRCE-DDS sobre UDP/Ethernet.
- **Rendimiento:** Latencia < 1ms en red local.
- **Telemetría:** Incluye envío de timestamp del sistema con precisión de microsegundos.

## 📁 Estructura del Proyecto
- `micro_ros_espidf_component/`: El componente principal para integrar en ESP-IDF.
- `examples/esp_p4_telemetry/`: Aplicación de ejemplo funcional con telemetría de tiempo cada 2s.
- `03-docs/`: Documentación técnica detallada (Arquitectura, Uso, Integración).

## 🛠️ Inicio Rápido
1. Clona este repositorio en tu entorno de desarrollo.
2. Configura los pines de tu placa en `menuconfig` o usa los defaults para Waveshare P4.
3. Compila y flashea:
   ```bash
   idf.py build
   idf.py flash
   ```
4. Inicia el agente en tu Raspberry Pi 5 (vía Docker):
   ```bash
   sudo docker run -it --rm --net=host microros/micro-ros-agent:humble udp4 --port 8888
   ```

## 📖 Documentación Detallada
- [Arquitectura del Sistema](03-docs/architecture.md)
- [Guía de Uso y Despliegue](03-docs/usage_guide.md)
- [Integración en Otros Proyectos](03-docs/component_integration.md)
- [Desarrollo de Nodos](03-docs/nodes_development.md)

---
*Desarrollado con estándares de Ingeniería de Software Senior para sistemas embebidos y robótica.*
