# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/pi5/esp/esp-idf/components/bootloader/subproject"
  "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader"
  "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader-prefix"
  "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader-prefix/tmp"
  "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader-prefix/src/bootloader-stamp"
  "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader-prefix/src"
  "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/pi5/micro_ros_espidf_component/examples/esp_p4_ptp_telemetry/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
