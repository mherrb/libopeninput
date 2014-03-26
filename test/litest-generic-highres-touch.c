/*
 * Copyright © 2013 Red Hat, Inc.
 * Copyright © 2014 Jonas Ådahl
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include "config.h"

#include "litest.h"
#include "litest-int.h"
#include "libinput-util.h"

void litest_generic_highres_touch_setup(void)
{
	struct litest_device *d =
		litest_create_device(LITEST_GENERIC_HIGHRES_TOUCH);
	litest_set_current_device(d);
}

void
litest_generic_highres_touch_touch_down(struct litest_device *d,
					unsigned int slot,
					int x, int y)
{
	static int tracking_id;
	struct input_event *ev;
	struct input_event down[] = {
		{ .type = EV_ABS, .code = ABS_X, .value = x  },
		{ .type = EV_ABS, .code = ABS_Y, .value = y },
		{ .type = EV_ABS, .code = ABS_MT_SLOT, .value = slot },
		{ .type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = ++tracking_id },
		{ .type = EV_ABS, .code = ABS_MT_POSITION_X, .value = x },
		{ .type = EV_ABS, .code = ABS_MT_POSITION_Y, .value = y },
		{ .type = EV_KEY, .code = BTN_TOUCH, .value = 1 },
		{ .type = EV_SYN, .code = SYN_REPORT, .value = 0 },
	};

	down[0].value = litest_scale(d, ABS_X, x);
	down[1].value = litest_scale(d, ABS_Y, y);
	down[4].value = litest_scale(d, ABS_X, x);
	down[5].value = litest_scale(d, ABS_Y, y);

	ARRAY_FOR_EACH(down, ev)
		litest_event(d, ev->type, ev->code, ev->value);
}

void
litest_generic_highres_touch_move(struct litest_device *d,
				  unsigned int slot,
				  int x, int y)
{
	struct input_event *ev;
	struct input_event move[] = {
		{ .type = EV_ABS, .code = ABS_MT_SLOT, .value = slot },
		{ .type = EV_ABS, .code = ABS_X, .value = x  },
		{ .type = EV_ABS, .code = ABS_Y, .value = y },
		{ .type = EV_ABS, .code = ABS_MT_POSITION_X, .value = x },
		{ .type = EV_ABS, .code = ABS_MT_POSITION_Y, .value = y },
		{ .type = EV_KEY, .code = BTN_TOUCH, .value = 1 },
		{ .type = EV_SYN, .code = SYN_REPORT, .value = 0 },
	};

	move[1].value = litest_scale(d, ABS_X, x);
	move[2].value = litest_scale(d, ABS_Y, y);
	move[3].value = litest_scale(d, ABS_X, x);
	move[4].value = litest_scale(d, ABS_Y, y);

	ARRAY_FOR_EACH(move, ev)
		litest_event(d, ev->type, ev->code, ev->value);
}

static struct litest_device_interface interface = {
	.touch_down = litest_generic_highres_touch_touch_down,
	.touch_move = litest_generic_highres_touch_move,
};

void
litest_create_generic_highres_touch(struct litest_device *d)
{
	struct input_absinfo abs[] = {
		{ ABS_X, 0, 32767, 75 },
		{ ABS_Y, 0, 32767, 129 },
		{ ABS_MT_SLOT, 0, 1, 0 },
		{ ABS_MT_POSITION_X, 0, 32767, 0, 0, 10 },
		{ ABS_MT_POSITION_Y, 0, 32767, 0, 0, 9 },
		{ ABS_MT_TRACKING_ID, 0, 65535, 0 },
		{ .value = -1 },
	};
	struct input_id id = {
		.bustype = 0x3,
		.vendor = 0xabcd, /* Some random vendor. */
		.product = 0x1234, /* Some random product id. */
	};

	d->interface = &interface;
	d->uinput = litest_create_uinput_abs_device("Generic emulated highres touch device",
						    &id,
						    abs,
						    EV_KEY, BTN_TOUCH,
						    INPUT_PROP_MAX, INPUT_PROP_DIRECT,
						    -1, -1);
}

struct litest_test_device litest_generic_highres_touch_device = {
	.type = LITEST_GENERIC_HIGHRES_TOUCH,
	.features = LITEST_TOUCH,
	.shortname = "generic-highres-touch",
	.setup = litest_generic_highres_touch_setup,
	.teardown = litest_generic_device_teardown,
	.create = litest_create_generic_highres_touch,
};
