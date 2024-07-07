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

static void update_mic_if_needed(Friend_ctx_s *ctx);
static void transport_subscribed(Friend_ctx_s *ctx);
static void transport_unsubscribed(Friend_ctx_s *ctx);
static void codec_handler(uint8_t *data, size_t len);
static void mic_handler(int16_t *buffer);
#ifdef ENABLE_BUTTON
static void on_button_pressed(void);
#endif // ENABLE_BUTTON
static void refresh_state_indication(Friend_ctx_s *ctx);
static int friend_context_init(Friend_ctx_s *ctx);


Friend_ctx_s friend_ctx;


static int friend_context_init(Friend_ctx_s *ctx)
{
	ctx->is_recording = false;
	ctx->is_connected = false;
	ctx->is_allowed = true;
	ctx->settings_enable = 0;

	ctx->battery_ctx.battery_status_charge = false;
	ctx->battery_ctx.battery_voltage = 0;
	ctx->battery_ctx.battery_percentage = 0;
	ctx->battery_ctx.voltage_counter = 0;

	return 0;
}

//
// Mic State
//

static void update_mic_if_needed(Friend_ctx_s *ctx)
{
	bool should_record = (ctx->is_allowed && ctx->is_connected);

	if (should_record != ctx->is_recording)
	{
		ctx->is_recording = should_record;
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

static void transport_subscribed(Friend_ctx_s *ctx)
{
	ctx->is_connected = true;
	update_mic_if_needed(ctx);
	refresh_state_indication(ctx);
}

static void transport_unsubscribed(Friend_ctx_s *ctx)
{
	ctx->is_connected = false;
	update_mic_if_needed(ctx);
	refresh_state_indication(ctx);
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
	friend_ctx.is_allowed = !(friend_ctx.is_allowed);
	set_allowed(friend_ctx.is_allowed);
	settings_write_enable(&friend_ctx);
	printk("Mic allowed: %d\n", friend_ctx.is_allowed);

	// Update mic
	update_mic_if_needed(&friend_ctx);

	// Refresh LED
	refresh_state_indication(&friend_ctx);
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

static void refresh_state_indication(Friend_ctx_s *ctx)
{
	bool red = false;
	bool green = false;
	bool blue = false;

	if (ctx->is_allowed)
	{
		if (ctx->is_recording)
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
	else if (ctx->battery_ctx.battery_status_charge)
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

	// Init Friend context with default values
	ASSERT_OK(friend_context_init(&friend_ctx));

	// Led start
	ASSERT_OK(led_start());

	// Settings start
	ASSERT_OK(settings_start());
#ifdef ENABLE_BUTTON
	friend_ctx.is_allowed = settings_read_enable(&friend_ctx);
#endif // ENABLE_BUTTON
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Battery start
	ASSERT_OK(battery_start(&friend_ctx));
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Camera start
#ifdef ENABLE_CAMERA
	ASSERT_OK(camera_start());
	ASSERT_OK(wdt_feed(wdt_dev, 0));
#endif // ENABLE_CAMERA

	// Transport start
	set_transport_callbacks(&transport_callbacks);
	ASSERT_OK(transport_start());
	set_allowed(friend_ctx.is_allowed);
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
	update_mic_if_needed(&friend_ctx);
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Set LED
	refresh_state_indication(&friend_ctx);
	ASSERT_OK(wdt_feed(wdt_dev, 0));

	// Main loop
	while (1)
	{
		// Wait wdt
		k_msleep(WDT_FEED_MS);

		// Watchdog
		ASSERT_OK(wdt_feed(wdt_dev, 0));

		// Update battery state
		if (friend_ctx.battery_ctx.battery_status_charge)
		{
			refresh_state_indication(&friend_ctx);
		}
	}

	// Unreachable
	return 0;
}
