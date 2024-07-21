#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/gatt.h>

#define TIME_1_SEC_IN_MS (1000U)
#define TIME_15_MS       (15U)

typedef struct Battery_context
{
	int battery_voltage;
	int voltage_counter;
	bool battery_status_charge;
	uint8_t battery_percentage;
	uint8_t rfu[2]; // padding
} Battery_ctx_s;

typedef struct Friend_context
{
	bool is_recording;
	bool is_connected;
	bool is_allowed;
	uint8_t settings_enable;

	Battery_ctx_s battery_ctx;
} Friend_ctx_s;

static const struct device *gpio0_port = DEVICE_DT_GET(DT_NODELABEL(gpio0));
static const struct device *gpio1_port = DEVICE_DT_GET(DT_NODELABEL(gpio1));

#define ASSERT_OK(result)                                          \
    if ((result) < 0)                                              \
    {                                                              \
        printk("Error at %s:%d:%d\n", __FILE__, __LINE__, result); \
        return (result);                                           \
    }

#define ASSERT_TRUE(result)                                        \
    if (!result)                                                   \
    {                                                              \
        printk("Error at %s:%d:%d\n", __FILE__, __LINE__, result); \
        return -1;                                                 \
    }


#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */
