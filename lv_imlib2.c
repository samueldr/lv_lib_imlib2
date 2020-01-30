#include "lvgl/lvgl.h"
#include "lv_imlib2.h"

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
	return LV_RES_INV;
}

static lv_res_t decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
	return LV_RES_INV;
}

static void decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
	(void) decoder;
}
