#include <zephyr/kernel.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/drivers/gpio.h>
#include "transport.h"
#include "mic.h"
#include "utils.h"
#include "led.h"
#include "config.h"
#include "audio.h"
#include "codec.h"
#include "camera.h"
#include "controls.h"
#include "battery.h"
#include "settings.h"

// State
#ifdef ENABLE_BUTTON
bool is_allowed = true;
#else // ENABLE_BUTTON
#define is_allowed true
#endif // !ENABLE_BUTTON

bool is_recording = false;
bool is_connected = false;
bool is_charging = false;

static void update_mic_if_needed(void);
static void transport_subscribed(void);
static void transport_unsubscribed(void);
static void codec_handler(uint8_t *data, size_t len);
static void mic_handler(int16_t *buffer);
#ifdef ENABLE_BUTTON
static void on_button_pressed(void);
#endif // ENABLE_BUTTON
static void refresh_state_indication(void);

//
// Mic State
//

static void update_mic_if_needed(void)
{
	bool should_record = (is_allowed && is_connected);

	if (should_record != is_recording)
	{
		is_recording = should_record;
		if (should_record)
		{
			mic_resume();
		}
		else
		{
			mic_pause();
		}
	}
}


//
// Transport callbacks
//

static void transport_subscribed(void)
{
	is_connected = true;
	update_mic_if_needed();
	refresh_state_indication();
}

static void transport_unsubscribed(void)
{
	is_connected = false;
	update_mic_if_needed();
	refresh_state_indication();
}

static struct transport_cb transport_callbacks = {
	.subscribed = transport_subscribed,
	.unsubscribed = transport_unsubscribed,
};

//
// Button
//

#ifdef ENABLE_BUTTON
static void on_button_pressed(void)
{
	// Update allowed flag
	is_allowed = !is_allowed;
	set_allowed(is_allowed);
	settings_write_enable(is_allowed);
	printk("Mic allowed: %d\n", is_allowed);

	// Update mic
	update_mic_if_needed();

	// Refresh LED
	refresh_state_indication();
}
#endif // ENABLE_BUTTON

//
// Audio Pipeline
//

static void codec_handler(uint8_t *data, size_t len)
{
	broadcast_audio_packets(data, len); // Errors are logged inside
}

static void mic_handler(int16_t *buffer)
{
	codec_receive_pcm(buffer, MIC_BUFFER_SAMPLES); // Errors are logged inside
}

//
// LED indication
//

static void refresh_state_indication(void)
{
	bool red = false;
	bool green = false;
	bool blue = false;

	if (is_allowed)
	{
		if (is_recording)
		{
			// Recording and connected - BLUE
			blue = true;
		}
		else
		{
			// Recording but lost connection - RED
			red = true;
		}
	}
	else if (is_charging)
	{
		// Not recording, but charging - WHITE
		red = true;
		green = true;
		blue = true;
	}
	else
	{
		/* do nothing */
	}

	// Otherwise, not recording - OFF (all LEDs remain false)
	set_led_red(red);
	set_led_green(green);
	set_led_blue(blue);
}

//
// Main
//

int main(void)
{
	// Start watchdog
	struct wdt_timeout_cfg wdt_config;
	const struct device *wdt_dev = DEVICE_DT_GET(DT_NODELABEL(wdt));
	wdt_config.flags = WDT_FLAG_RESET_SOC;
	wdt_config.window.min = 0;
	wdt_config.window.max = WDT_TIMEOUT_MS;
	wdt_config.callback = NULL; // Set to NULL to cause a system reset
	ASSERT_OK(wdt_install_timeout(wdt_dev, &wdt_config));
	ASSERT_OK(wdt_setup(wdt_dev, WDT_OPT_PAUSE_HALTED_BY_DBG));

	// Led start
	ASSERT_OK(led_start());

	// Settings start
	ASSERT_OK(settings_start());
#ifdef ENABLE_BUTTON
	is_allowed = settings_read_enable();
#endif // ENABLE_BUTTON
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Battery start
	ASSERT_OK(battery_start());
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Camera start
#ifdef ENABLE_CAMERA
	ASSERT_OK(camera_start());
	ASSERT_OK(wdt_feed(wdt_dev, 0));
#endif // ENABLE_CAMERA

	// Transport start
	set_transport_callbacks(&transport_callbacks);
	ASSERT_OK(transport_start());
	set_allowed(is_allowed);
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Controls
#ifdef ENABLE_BUTTON
	set_button_handler(on_button_pressed);
	ASSERT_OK(start_controls());
#endif // ENABLE_BUTTON
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Codec start
	set_codec_callback(codec_handler);
	ASSERT_OK(codec_start());
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Mic start
	set_mic_callback(mic_handler);
	ASSERT_OK(mic_start());
	update_mic_if_needed();
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Set LED
	is_charging = is_battery_charging();
	refresh_state_indication();
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Main loop
	while (1)
	{
		// Wait wdt
		k_msleep(WDT_FEED_MS);

		// Watchdog
		ASSERT_OK(wdt_feed(wdt_dev, 0));

		// Update battery state
		bool charging = is_battery_charging();
		if (charging != is_charging)
		{
			is_charging = charging;
			refresh_state_indication();
		}
	}

	// Unreachable
	return 0;
}
