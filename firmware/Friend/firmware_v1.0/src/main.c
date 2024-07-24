#include <zephyr/kernel.h>
#include "transport.h"
#include "mic.h"
#include "utils.h"
#include "led.h"
#include "config.h"
#include "audio.h"
#include "codec.h"

// Main device context
Friend_Ctx_s Friend_ctx;

static void init_context(Friend_Ctx_s *ctx);
static void codec_handler(uint8_t *data, size_t len);
static void mic_handler(int16_t *buffer);

static void init_context(Friend_Ctx_s *ctx)
{
	ctx->is_charging = false;
	ctx->is_connected = false;
}

static void codec_handler(uint8_t *data, size_t len)
{
	broadcast_audio_packets(data, len); // Errors are logged inside
}

static void mic_handler(int16_t *buffer)
{
	codec_receive_pcm(buffer, MIC_BUFFER_SAMPLES); // Errors are logged inside
}

Friend_Ctx_s* get_friend_context(void)
{
    return &Friend_ctx;
}

void bt_ctlr_assert_handle(char *name, int type)
{
	if (name != NULL)
	{
		printk("Bt assert-> %s", name);
	}
}

void set_led_state(Friend_Ctx_s *ctx)
{
	// Recording and connected state - BLUE
	if (ctx->is_connected == true)
	{
		set_led_red(false);
		set_led_green(false);
		set_led_blue(true);
		return;
	}

	// Recording but lost connection - RED
	if (ctx->is_connected == false)
	{
		set_led_red(true);
		set_led_green(false);
		set_led_blue(false);
		return;
	}

	// Not recording, but charging - WHITE
	if (ctx->is_charging == true)
	{
		set_led_red(true);
		set_led_green(true);
		set_led_blue(true);
		return;
	}

	// Not recording - OFF
	set_led_red(false);
	set_led_green(false);
	set_led_blue(false);
}

// Main loop
int main(void)
{
	init_context(&Friend_ctx);

	// Led start
	ASSERT_OK(led_start());
	set_led_blue(true);

	// Transport start
	ASSERT_OK(transport_start());

	// Codec start
	set_codec_callback(codec_handler);
	ASSERT_OK(codec_start());

	// Mic start
	set_mic_callback(mic_handler);
	ASSERT_OK(mic_start());

	while (1)
	{
		set_led_state(&Friend_ctx);
		k_msleep(500);
	}

	// Unreachable
	return 0;
}
