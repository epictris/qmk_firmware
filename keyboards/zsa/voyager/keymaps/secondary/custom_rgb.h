RGB_MATRIX_EFFECT(custom_rgb)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool custom_rgb(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV     hsv   = rgb_matrix_config.hsv;
    uint8_t scale = scale8(64, rgb_matrix_config.speed);
    uint8_t offset = -20;


    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        // The y range will be 0..64, map this to 0..4
        // Relies on hue being 8-bit and wrapping
        if (g_led_config.point[i].y == 64) {
            uint8_t brightness = hsv.v > 0;
            brightness = brightness + (pow(hsv.v, 2.2) / (255 * 6));
            rgb_matrix_set_color(i, brightness, brightness, brightness);
        } else {
            hsv.h   = (offset + rgb_matrix_config.hsv.h + scale * (g_led_config.point[i].y >> 4)) % 255;
            RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif
