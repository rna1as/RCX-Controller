#include <esp_log.h>

#include "application.h"
#include "audio/codecs/no_audio_codec.h"
#include "boards/common/button.h"
#include "config.h"
#include "display/display.h"
#include "led/single_led.h"
#include "wifi_board.h"

#define TAG "RcxJarvisN16R8"

class RcxJarvisAudioCodec : public NoAudioCodecSimplex {
public:
    RcxJarvisAudioCodec()
        : NoAudioCodecSimplex(
              AUDIO_INPUT_SAMPLE_RATE,
              AUDIO_OUTPUT_SAMPLE_RATE,
              AUDIO_I2S_SPK_GPIO_BCLK,
              AUDIO_I2S_SPK_GPIO_LRCK,
              AUDIO_I2S_SPK_GPIO_DOUT,
              I2S_STD_SLOT_LEFT,
              AUDIO_I2S_MIC_GPIO_SCK,
              AUDIO_I2S_MIC_GPIO_WS,
              AUDIO_I2S_MIC_GPIO_DIN,
              I2S_STD_SLOT_LEFT) {
        SetOutputVolume(25);
    }
};

class RcxJarvisN16R8 : public WifiBoard {
private:
    NoDisplay display_;
    Button boot_button_;

    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            if (app.GetDeviceState() == kDeviceStateStarting) {
                EnterWifiConfigMode();
                return;
            }
            app.ToggleChatState();
        });
    }

public:
    RcxJarvisN16R8() : boot_button_(BOOT_BUTTON_GPIO) {
        InitializeButtons();
        ESP_LOGI(TAG, "RCX Jarvis N16R8 initialized");
    }

    AudioCodec* GetAudioCodec() override {
        static RcxJarvisAudioCodec audio_codec;
        return &audio_codec;
    }

    Display* GetDisplay() override {
        return &display_;
    }

    Led* GetLed() override {
        static SingleLed led(BUILTIN_LED_GPIO);
        return &led;
    }
};

DECLARE_BOARD(RcxJarvisN16R8);
