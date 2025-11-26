#ifndef JPEG_H
#define JPEG_H


#pragma once

#include <stdint.h>
#include <stddef.h>

// JPEG 编码函数接口
// 输入：原始图像数据（RGB565）、图像尺寸
// 输出：JPEG 数据指针和长度（动态分配，需外部释放）
uint8_t* jpeg_encode_rgb565(
    const uint8_t* rgb_data, 
    size_t rgb565_len, 
    uint16_t width, 
    uint16_t height, 
    size_t* jpeg_len);

#endif
