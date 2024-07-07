#include <zephyr/kernel.h>
#include <zephyr/settings/settings.h>

#include "settings.h"

//TODO To be fixed
extern Friend_ctx_s friend_ctx;

static int settings_set(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg)
{
    const char *next;
    if (settings_name_steq(name, "enable", &next) && !next)
    {
        if (len != sizeof(friend_ctx.settings_enable))
        {
            return -EINVAL;
        }
        return read_cb(cb_arg, &friend_ctx.settings_enable, sizeof(friend_ctx.settings_enable));
    }
    return -ENOENT;
}

static struct settings_handler cfg = {
    .name = "main",
    .h_set = settings_set,
};

int settings_start(void)
{
    ASSERT_OK(settings_subsys_init());
    ASSERT_OK(settings_register(&cfg));
    ASSERT_OK(settings_load());

    return 0;
}

bool settings_read_enable(Friend_ctx_s *ctx)
{
    return  (bool)(ctx->settings_enable != 0);
}

int settings_write_enable(Friend_ctx_s *ctx)
{
    uint8_t v = ctx->is_allowed ? 1 : 0;
    if (ctx->settings_enable != v)
    {
        ctx->settings_enable = v;
        ASSERT_OK(settings_save_one("main/enable", &ctx->settings_enable, sizeof(ctx->settings_enable)));
    }

    return 0;
}
