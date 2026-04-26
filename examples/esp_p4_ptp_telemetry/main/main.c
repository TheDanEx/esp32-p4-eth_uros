#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"

#include <uros_network_interfaces.h>
#include <std_msgs/msg/string.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#ifdef CONFIG_MICRO_ROS_ESP_XRCE_DDS_MIDDLEWARE
#include <rmw_microros/rmw_microros.h>
#endif

#define STRING_BUFFER_LEN 160
#define SYNC_INTERVAL_MS 5000 
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d.\n",__LINE__,(int)temp_rc); return;}}

rcl_publisher_t time_publisher;
std_msgs__msg__String esp_time_msg;

float g_latency_ms = 0.0f;
float g_offset_ms = 0.0f;
float g_jitter_ms = 0.0f;
float g_last_latency_ms = -1.0f;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
	RCLC_UNUSED(last_call_time);
	if (timer != NULL) {
		struct timeval tv;
		gettimeofday(&tv, NULL); 
		
		struct tm timeinfo;
		localtime_r(&tv.tv_sec, &timeinfo);
		
		char strftime_buf[32];
		strftime(strftime_buf, sizeof(strftime_buf), "%H:%M:%S", &timeinfo);
		
		sprintf(esp_time_msg.data.data, 
                "[SYNC] %s.%06ld | Lat: %.3fms | Off: %.3fms | Jit: %.3fms", 
                strftime_buf, (long)tv.tv_usec,
                g_latency_ms, g_offset_ms, g_jitter_ms);
                
		esp_time_msg.data.size = strlen(esp_time_msg.data.data);
		
		rcl_ret_t pub_ret = rcl_publish(&time_publisher, (const void*)&esp_time_msg, NULL);
        (void)pub_ret;
		printf("DIAG: %s\n", esp_time_msg.data.data);
	}
}

void micro_ros_task(void * arg)
{
	rcl_allocator_t allocator = rcl_get_default_allocator();
	rclc_support_t support;
	rcl_node_t node;
	rcl_timer_t timer;
	rclc_executor_t executor;

	while(1) {
		rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
		RCCHECK(rcl_init_options_init(&init_options, allocator));
#ifdef CONFIG_MICRO_ROS_ESP_XRCE_DDS_MIDDLEWARE
		rmw_init_options_t* rmw_options = rcl_init_options_get_rmw_init_options(&init_options);
		rmw_uros_options_set_udp_address(CONFIG_MICRO_ROS_AGENT_IP, CONFIG_MICRO_ROS_AGENT_PORT, rmw_options);
#endif
		if (rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator) != RCL_RET_OK) {
			rcl_ret_t f_ret = rcl_init_options_fini(&init_options); (void)f_ret;
			vTaskDelay(pdMS_TO_TICKS(2000));
			continue;
		}

		node = rcl_get_zero_initialized_node();
		RCCHECK(rclc_node_init_default(&node, "esp32_p4_diag_node", "", &support));

		RCCHECK(rclc_publisher_init_default(&time_publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/microROS/esp_diag_time"));
		RCCHECK(rclc_timer_init_default2(&timer, &support, RCL_MS_TO_NS(2000), timer_callback, true));

		RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
		RCCHECK(rclc_executor_add_timer(&executor, &timer));

		char buffer[STRING_BUFFER_LEN];
		esp_time_msg.data.data = buffer;
		esp_time_msg.data.capacity = STRING_BUFFER_LEN;

        int64_t last_sync_time = 0;

		while(1){
            int64_t now = esp_timer_get_time();
            if ((now - last_sync_time) > (SYNC_INTERVAL_MS * 1000)) {
                int64_t t_before = esp_timer_get_time();
                if (rmw_uros_sync_session(1000) == RCL_RET_OK) {
                    int64_t t_after = esp_timer_get_time();
                    int64_t rtt_us = t_after - t_before;
                    
                    g_latency_ms = (float)rtt_us / 2000.0f;
                    if (g_last_latency_ms >= 0) {
                        g_jitter_ms = fabsf(g_latency_ms - g_last_latency_ms);
                    }
                    g_last_latency_ms = g_latency_ms;

                    // --- CÁLCULO DE PRECISIÓN DE MICROSEGUNDOS ---
                    int64_t agent_ms = rmw_uros_epoch_millis();
                    // El tiempo del agente en el momento de recibir la respuesta es:
                    // Agent_Time + (RTT / 2)
                    int64_t agent_us = (agent_ms * 1000) + (rtt_us / 2);
                    
                    struct timeval tv_now;
                    gettimeofday(&tv_now, NULL);
                    int64_t local_us = (int64_t)tv_now.tv_sec * 1000000 + tv_now.tv_usec;
                    
                    g_offset_ms = (float)(agent_us - local_us) / 1000.0f;
                    
                    // Sincronizar el reloj con precisión de microsegundos
                    struct timeval tv_sync = { .tv_sec = agent_us / 1000000, .tv_usec = agent_us % 1000000 };
                    settimeofday(&tv_sync, NULL);
                    
                    last_sync_time = esp_timer_get_time();
                }
            }

			if (rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)) != RCL_RET_OK) break;
			vTaskDelay(pdMS_TO_TICKS(100));
		}

		rcl_ret_t f1 = rcl_publisher_fini(&time_publisher, &node); (void)f1;
		rcl_ret_t f2 = rcl_node_fini(&node); (void)f2;
		rcl_ret_t f3 = rclc_support_fini(&support); (void)f3;
		rcl_ret_t f4 = rcl_init_options_fini(&init_options); (void)f4;
	}
}

void app_main(void)
{
    ESP_ERROR_CHECK(uros_network_interface_initialize());
    xTaskCreate(micro_ros_task, "uros_task", 16384, NULL, 5, NULL);
}
