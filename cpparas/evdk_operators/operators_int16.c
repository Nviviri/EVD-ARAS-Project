/******************************************************************************
 * Project    : Embedded Vision Design
 * Copyright  : 2019 HAN Electrical and Electronic Engineering
 * Author     : Hugo Arends
 *
 * Description: Implementation file for int16 image processing operators
 *
 * Copyright (C) 2019 HAN University of Applied Sciences. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************
  Change History:

    Version 1.0 - November 2017
    > Initial revision

    Version 2.0 - September 2019
    > Updated for EVDK3.0

******************************************************************************/
#include "operators_int16.h"
#include "math.h"

#ifdef STM32F746xx
#include "mem_manager.h"
#endif

// ----------------------------------------------------------------------------
// Function implementations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
image_t* newInt16Image(const uint32_t cols, const uint32_t rows)
{
    image_t* img = (image_t*)malloc(sizeof(image_t));
    if (img == NULL) {
        // Unable to allocate memory for new image
        return NULL;
    }

#ifdef STM32F746xx
    img->data = mem_manager_alloc();
#else
    img->data = (uint8_t*)malloc((rows * cols) * sizeof(int16_pixel_t));
#endif

    if (img->data == NULL) {
        // Unable to allocate memory for data
        free(img);
        return NULL;
    }

    img->cols = cols;
    img->rows = rows;
    img->view = IMGVIEW_CLIP;
    img->type = IMGTYPE_INT16;
    return (img);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
image_t* toInt16Image(image_t* src)
{
    image_t* dst = newInt16Image(src->cols, src->rows);
    if (dst == NULL)
        return NULL;

    convertToInt16Image(src, dst);

    return dst;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void deleteInt16Image(image_t* img)
{
#ifdef STM32F746xx
    mem_manager_free(img->data);
#else
    free(img->data);
#endif

    free(img);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void convertToInt16Image(const image_t* src, image_t* dst)
{
    register long int i = src->rows * src->cols;
    register int16_pixel_t* d = (int16_pixel_t*)dst->data;

    dst->view = src->view;
    dst->cols = src->cols;
    dst->rows = src->rows;
    dst->view = src->view;

    switch (src->type) {
    case IMGTYPE_BASIC: {
        basic_pixel_t* s = (basic_pixel_t*)src->data;
        // Loop all pixels and copy
        while (i-- > 0)
            *d++ = (int16_pixel_t)(*s++);

    } break;
    case IMGTYPE_INT16: {
        copy_int16(src, dst);

    } break;
    case IMGTYPE_FLOAT: {
        float_pixel_t* s = (float_pixel_t*)src->data;
        // Loop all pixels and copy
        while (i-- > 0)
            *d++ = (int16_pixel_t)(*s++);

    } break;
    case IMGTYPE_RGB888: {
        rgb888_pixel_t* s = (rgb888_pixel_t*)src->data;
        // Loop all pixels, convert and copy
        while (i-- > 0) {
            unsigned char r = s->r;
            unsigned char g = s->g;
            unsigned char b = s->b;

            *d++ = (int16_pixel_t)(0.212671f * r + 0.715160f * g + 0.072169f * b);
            s++;
        }

    } break;
    default:
        break;
    }
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void contrastStretch_int16(const image_t* src,
    image_t* dst,
    const int16_pixel_t bottom,
    const int16_pixel_t top)
{
    // ********************************************
    // Added to prevent compiler warnings
    // Remove these when implementation starts
    (void)src;
    (void)dst;
    (void)bottom;
    (void)top;

    return;
    // ********************************************
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void erase_int16(const image_t* img)
{
    register long int i = img->rows * img->cols;
    register int16_pixel_t* s = (int16_pixel_t*)img->data;

    // Loop through the image and set all pixels to the value 0
    while (i-- > 0)
        *s++ = 0;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void threshold_int16(const image_t* src,
    image_t* dst,
    const int16_pixel_t low,
    const int16_pixel_t high)
{
    // ********************************************
    // Added to prevent compiler warnings
    // Remove these when implementation starts
    (void)src;
    (void)dst;
    (void)low;
    (void)high;

    return;
    // ********************************************
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void copy_int16(const image_t* src, image_t* dst)
{
    register long int i = src->rows * src->cols;
    register int16_pixel_t* s = (int16_pixel_t*)src->data;
    register int16_pixel_t* d = (int16_pixel_t*)dst->data;

    dst->rows = src->rows;
    dst->cols = src->cols;
    dst->type = src->type;
    dst->view = src->view;

    // Loop all pixels and copy
    while (i-- > 0)
        *d++ = *s++;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void setSelectedToValue_int16(const image_t* src,
    image_t* dst,
    const int16_pixel_t selected,
    const int16_pixel_t value)
{
    // ********************************************
    // Added to prevent compiler warnings
    // Remove these when implementation starts
    (void)src;
    (void)dst;
    (void)selected;
    (void)value;

    return;
    // ********************************************
}

// ----------------------------------------------------------------------------
// EOF
// ----------------------------------------------------------------------------
