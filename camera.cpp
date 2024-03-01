#include "firebase.h"
#include <Arduino.h>
#include <base64.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include "camera.h"

String urlencode(String str)
{
    String encodedString = "";
    char c;
    char code0;
    char code1;
    char code2;

    for (int i = 0; i < str.length(); i++)
    {
        c = str.charAt(i);
        if (c == ' ')
        {
            encodedString += '+';
        }
        else if (isalnum(c))
        {
            encodedString += c;
        }
        else
        {
            code1 = (c & 0xf) + '0';
            if ((c & 0xf) > 9)
            {
                code1 = (c & 0xf) - 10 + 'A';
            }
            c = (c >> 4) & 0xf;
            code0 = c + '0';
            if (c > 9)
            {
                code0 = c - 10 + 'A';
            }
            code2 = '\0';
            encodedString += '%';
            encodedString += code0;
            encodedString += code1;
            encodedString += code2;
            yield();
        }
        return encodedString;
    }
}

String Photo2Base64()
{

    camera_fb_t *fb = NULL;

    fb = esp_camera_fb_get();

    if (!fb)
    {

        Serial.println("Camera capture failed");

        return "";
    }

    String imageFile = "data:image/jpeg;base64,";
    String encrypt = base64::encode(fb->buf, fb->len);

    esp_camera_fb_return(fb);

    //  return imageFile;
    // Serial.println(encrypt);
    return encrypt;
}

void setupCamera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    // init with high specs to pre-allocate larger buffers
    if (psramFound())
    {
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 12; // 0-63 lower number means higher quality
        config.fb_count = 1;
    }
    else
    {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12; // 0-63 lower number means higher quality
        config.fb_count = 1;
    }
    // camera init
    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        delay(1000);
        ESP.restart();
    }
    sensor_t *s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_QQVGA); // VGA|CIF|QVGA|HQVGA|QQVGA   ( UXGA? SXGA? XGA? SVGA? )
}

void processStream()
{
    String jsonData = "{\"photo\":\"" + Photo2Base64() + "\"}";
    store_camera_data("Camera", jsonData);
}