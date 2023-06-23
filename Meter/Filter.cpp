#include "Filter.h"

// Filter spec from Section 6 of
// https://ams.com/documents/20143/36005/AS7341_DS000504_3-00.pdf/5eca1f59-46e2-6fc5-daf5-d71ad90c9b2b
// Wavelength to RGB conversion from
// https://www.johndcook.com/wavelength_to_RGB.html
const decltype(Filters::filters) Filters::filters{{
    {0, 415, 26, AS7341_CHANNEL_415nm_F1, 0x7600ed},
    {1, 445, 30, AS7341_CHANNEL_445nm_F2, 0x0028ff},
    {2, 480, 36, AS7341_CHANNEL_480nm_F3, 0x00d5ff},
    {3, 515, 39, AS7341_CHANNEL_515nm_F4, 0x1fff00},
    {4, 555, 39, AS7341_CHANNEL_555nm_F5, 0xb3ff00},
    {5, 590, 40, AS7341_CHANNEL_590nm_F6, 0xffdf00},
    {6, 630, 50, AS7341_CHANNEL_630nm_F7, 0xff4f00},
    {7, 680, 52, AS7341_CHANNEL_680nm_F8, 0xff0000},
}};

// Corrective coefficient from Figure 10 of
// https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740
// with my own correction
const decltype(Filters::whiteBalance) Filters::whiteBalance{
    {3.20 * 0.8, 3.00 * 0.8, 2.07 * 0.8, 1.30, 1.07, 0.93, 0.78, 0.71}};
