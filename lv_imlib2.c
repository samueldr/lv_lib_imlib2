#include <stdio.h>
#include "lvgl/lvgl.h"
#include "lv_imlib2.h"
#include <Imlib2.h>

// Struct used to hold data for special operation to apply on the next image load.
typedef struct {
	int width;
	int height;
} lv_imlib2_next_data_t;

static lv_imlib2_next_data_t next_data;

void lv_imlib2_next_reset(void);

static lv_res_t decoder_info(struct _lv_img_decoder * decoder, const void * src, lv_img_header_t * header);
static lv_res_t decoder_open(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);
static void decoder_close(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);

void lv_imlib2_init(void)
{
	lv_imlib2_next_reset();
	lv_img_decoder_t * dec = lv_img_decoder_create();
	lv_img_decoder_set_info_cb(dec, decoder_info);
	lv_img_decoder_set_open_cb(dec, decoder_open);
	lv_img_decoder_set_close_cb(dec, decoder_close);
}

static lv_res_t decoder_info(struct _lv_img_decoder * decoder, const void * src, lv_img_header_t * header)
{
	(void) decoder;
	lv_img_src_t src_type = lv_img_src_get_type(src);
	int height = 0;
	int width = 0;

	if (src_type == LV_IMG_SRC_FILE) {
		const char * filename = src;
		Imlib_Load_Error error;
		Imlib_Image image;
		Imlib_Image previous_image = imlib_context_get_image();

		image = imlib_load_image_with_error_return(filename, &error);
		imlib_context_set_image(image);
		if (error) {
			fprintf(stderr, "Failed to load image %s, error: %d\n", filename , error);
			return LV_RES_INV;
		}

		header->always_zero = 0;
		header->cf = LV_IMG_CF_RAW_ALPHA;

		width = imlib_image_get_width();
		height = imlib_image_get_height();

		if (next_data.width > 0 || next_data.height > 0) {
			if (next_data.width == 0) {
				next_data.width = width * next_data.height / height;
			}
			if (next_data.height == 0) {
				next_data.height = height * next_data.width / width;
			}
			width = next_data.width;
			height = next_data.height;
		}

		header->w = width;
		header->h = height;

		imlib_free_image();

		imlib_context_set_image(previous_image);

		return LV_RES_OK;
	}

	return LV_RES_INV;
}

static lv_res_t decoder_open(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
	if (dsc->src_type != LV_IMG_SRC_FILE) {
		return LV_RES_INV;
	}

	(void) decoder;
	const char * filename = dsc->src;
	Imlib_Image image;
	Imlib_Image previous_image = imlib_context_get_image();

	image = imlib_load_image(filename);
	imlib_context_set_image(image);
	
	if (next_data.width > 0 || next_data.height > 0) {
		int img_height = 0;
		int img_width = 0;
		img_width = imlib_image_get_width();
		img_height = imlib_image_get_height();
		Imlib_Image new_image;

		imlib_context_set_anti_alias(true);
		new_image = imlib_create_cropped_scaled_image(0, 0, img_width, img_height, next_data.width, next_data.height);

		// Free `image`
		imlib_free_image();

		image = new_image;
		imlib_context_set_image(new_image);
	}

	dsc->img_data = (uint8_t *) imlib_image_get_data();

	imlib_free_image();
	imlib_context_set_image(previous_image);
	lv_imlib2_next_reset();

	return LV_RES_OK;
}

static void decoder_close(lv_img_decoder_t * decoder, lv_img_decoder_dsc_t * dsc)
{
	(void) decoder;
	imlib_image_put_back_data((DATA32 *) dsc->img_data);
}

void lv_imlib2_next_reset()
{
	next_data.width = 0;
	next_data.height = 0;
}

void lv_imlib2_resize_next_width(int width)
{
	next_data.width = width;
}

void lv_imlib2_resize_next_height(int height)
{
	next_data.height = height;
}

