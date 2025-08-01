
#ifdef __has_include
  #if __has_include("lvgl.h")
    #ifndef LV_LVGL_H_INCLUDE_SIMPLE
      #define LV_LVGL_H_INCLUDE_SIMPLE
    #endif
  #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
  #include "lvgl.h"
#else
  #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
  #define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_ICON_MOSTLY_SUNNY
  #define LV_ATTRIBUTE_ICON_MOSTLY_SUNNY
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_ICON_MOSTLY_SUNNY uint8_t
    icon_mostly_sunny_map[] = {

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcd, 0xfe, 0xea, 0xfe, 0xea, 0xfe, 0xe9,
        0xfe, 0xe8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xe8, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe,
        0xcd, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xea, 0xfe, 0x50, 0xff, 0xe9, 0xfe, 0x84, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe, 0xe8, 0xfe,
        0xc8, 0xfe, 0xc8, 0xfe, 0xe8, 0xfe, 0x82, 0xfe, 0xc8, 0xfe, 0x0e, 0xff, 0xc8, 0xfe, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xea, 0xfe, 0xea, 0xfe, 0xc8, 0xfe,
        0xe9, 0xfe, 0xe9, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8,
        0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xea, 0xfe, 0xea, 0xfe, 0xa5, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe, 0xc8, 0xfe, 0xc8,
        0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe,
        0xc7, 0xfe, 0xa3, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0x00, 0x00, 0xcd, 0xfe, 0x4f, 0xff, 0xc8,
        0xfe, 0xe9, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe,
        0xc8, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7,
        0xfe, 0x2f, 0xff, 0xcd, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe, 0xe9, 0xfe,
        0xe8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc7,
        0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe,
        0xe9, 0xfe, 0xa6, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8,
        0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe,
        0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc6, 0xfe, 0xc7, 0xfe, 0xe8, 0xfe, 0xe8, 0xfe, 0xc8,
        0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe,
        0xc8, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7,
        0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe,
        0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe, 0xc7,
        0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe,
        0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8,
        0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe,
        0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc6, 0xfe, 0xc6, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8,
        0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe,
        0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc6,
        0xfe, 0xa6, 0xfe, 0xc6, 0xfe, 0xc8, 0xfe, 0xc7, 0xfe, 0xea, 0xfe, 0x0f, 0xff, 0xed, 0xfe,
        0xc8, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7,
        0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc6, 0xfe, 0xa6, 0xfe, 0xa6, 0xfe,
        0x58, 0xf7, 0x59, 0xf7, 0x7a, 0xf7, 0x7d, 0xef, 0x7c, 0xf7, 0x13, 0xf7, 0xa8, 0xfe, 0xc7,
        0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe,
        0xc7, 0xfe, 0xc6, 0xfe, 0xc6, 0xfe, 0xa6, 0xfe, 0xa6, 0xfe, 0x97, 0xe6, 0x7d, 0xef, 0x7e,
        0xef, 0x7d, 0xef, 0x7e, 0xef, 0x7d, 0xef, 0x36, 0xf7, 0x10, 0xf7, 0xc8, 0xfe, 0xc7, 0xfe,
        0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc6, 0xfe, 0xc6, 0xfe, 0xa6,
        0xfe, 0x06, 0xff, 0xa7, 0xfe, 0x3c, 0xef, 0x7d, 0xef, 0x7d, 0xef, 0x7d, 0xef, 0x7d, 0xef,
        0x7d, 0xef, 0x9e, 0xef, 0x7d, 0xef, 0xad, 0xf6, 0xa7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7,
        0xfe, 0xc7, 0xfe, 0xc6, 0xfe, 0xc6, 0xfe, 0xa6, 0xfe, 0xa6, 0xfe, 0xa6, 0xfe, 0xa6, 0xfe,
        0x9e, 0xef, 0x7d, 0xef, 0x7d, 0xef, 0x7d, 0xef, 0x7d, 0xef, 0x7e, 0xef, 0x7e, 0xef, 0x7c,
        0xef, 0x8c, 0xf6, 0x86, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc6, 0xfe, 0xc6, 0xfe,
        0xa6, 0xfe, 0xa6, 0xfe, 0xc6, 0xfe, 0x05, 0xed, 0xcd, 0xfe, 0x3c, 0xef, 0x1b, 0xe7, 0x5d,
        0xef, 0x5d, 0xef, 0x5d, 0xef, 0x18, 0xef, 0xf5, 0xee, 0xaf, 0xf6, 0x47, 0xf6, 0x66, 0xfe,
        0xa6, 0xfe, 0xc6, 0xfe, 0xc6, 0xfe, 0xc6, 0xfe, 0xa6, 0xfe, 0xa6, 0xfe, 0x67, 0xff, 0x86,
        0xfe, 0x86, 0xfe, 0x00, 0x00, 0x3c, 0xef, 0x3c, 0xef, 0x7d, 0xef, 0xff, 0xff, 0xda, 0x8c,
        0x27, 0xf6, 0x46, 0xf6, 0x66, 0xfe, 0x86, 0xfe, 0x86, 0xfe, 0x86, 0xfe, 0xa6, 0xfe, 0xa6,
        0xfe, 0xa6, 0xfe, 0xa6, 0xfe, 0xc6, 0xfe, 0x86, 0xfe, 0x86, 0xfe, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xb9, 0xe6, 0x99, 0xe6, 0xdb, 0xde, 0xbc, 0xde, 0x27, 0xf6, 0x09, 0xee, 0x66,
        0xfe, 0x86, 0xfe, 0x86, 0xfe, 0x86, 0xfe, 0x86, 0xfe, 0x86, 0xfe, 0x87, 0xff, 0xa6, 0xfe,
        0x05, 0xed, 0x86, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xcd, 0xfe, 0x46, 0xfe, 0x46, 0xf6, 0x66, 0xfe, 0x86, 0xfe, 0x86, 0xfe,
        0x86, 0xfe, 0x86, 0xfe, 0x86, 0xfe, 0x66, 0xfe, 0x86, 0xfe, 0xcd, 0xfe, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x17, 0x3a, 0x53, 0x53, 0x3a, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x86, 0xd3, 0xf3, 0xfc, 0xfc, 0xf3, 0xd3, 0x86, 0x24,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4d, 0xd5, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xd5, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4d, 0xe9,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe8, 0x4d, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x24, 0xd5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xd5, 0x23, 0x00, 0x00, 0x00, 0x00, 0x86, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x86, 0x00, 0x00, 0x00, 0x17, 0xd3, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd3, 0x16, 0x00,
        0x00, 0x3a, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xf2, 0x3a, 0x00, 0x00, 0x53, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x52, 0x00, 0x00, 0x52, 0xfc, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x52, 0x00,
        0x00, 0x38, 0xf2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xf2, 0x39, 0x00, 0x00, 0x3e, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd3, 0x16, 0x00, 0x00, 0x97, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x86, 0x00, 0x00,
        0x03, 0xa8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xd5, 0x23, 0x00, 0x00, 0x00, 0x56, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xe8, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x51, 0x9b, 0xa7,
        0xd8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd5, 0x4d, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x24, 0x86, 0xd3, 0xf3, 0xfc, 0xfc, 0xf3, 0xd3, 0x86, 0x23,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x3a, 0x52,
        0x52, 0x3a, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

const lv_image_dsc_t icon_mostly_sunny = {
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.cf = LV_COLOR_FORMAT_RGB565A8,
    .header.flags = 0,
    .header.w = 20,
    .header.h = 20,
    .header.stride = 40,
    .header.reserved_2 = 0,
    .data_size = sizeof(icon_mostly_sunny_map),
    .data = icon_mostly_sunny_map,
    .reserved = NULL,
};
