#include "lvgl/lvgl.h"
#include "lv_imlib2.h"
#include <Imlib2.h>

static lv_res_t decoder_info(struct _lv_img_decoder * decoder, const void * src, lv_img_header_t * header);
static lv_res_t decoder_open(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);
static void decoder_close(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);

void lv_imlib2_init(void)
{
	lv_img_decoder_t * dec = lv_img_decoder_create();
	lv_img_decoder_set_info_cb(dec, decoder_info);
	lv_img_decoder_set_open_cb(dec, decoder_open);
	lv_img_decoder_set_close_cb(dec, decoder_close);
}


static lv_res_t decoder_info(struct _lv_img_decoder * decoder, const void * src, lv_img_header_t * header)
{
	(void) decoder;
	lv_img_src_t src_type = lv_img_src_get_type(src);

	if (src_type == LV_IMG_SRC_FILE) {
		const char * filename = src;
		Imlib_Image image;
		Imlib_Image previous_image = imlib_context_get_image();

		image = imlib_load_image(filename);
		imlib_context_set_image(image);

		header->always_zero = 0;
		// FIXME: bad assumption, is it always "raw alpha" with imlib2?
		header->cf = LV_IMG_CF_RAW_ALPHA;
		header->w = imlib_image_get_width();
		header->h = imlib_image_get_height();

		imlib_context_set_image(previous_image);

		return LV_RES_OK;
	}

	return LV_RES_INV;
}

static lv_res_t decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
	(void) decoder;

	if (dsc->src_type == LV_IMG_SRC_FILE) {
		const char * filename = dsc->src;
		Imlib_Image image;
		Imlib_Image previous_image = imlib_context_get_image();

		image = imlib_load_image(filename);
		imlib_context_set_image(image);

		dsc->img_data = (uint8_t *) imlib_image_get_data();

		imlib_context_set_image(previous_image);

		return LV_RES_OK;
	}

	return LV_RES_INV;
}

static void decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
	(void) decoder;
	imlib_image_put_back_data((DATA32 *) dsc->img_data);
}
