#include <LovyanGFX.hpp>

// PIN NUM SETUP
#define TFT_SCLK 18
#define TFT_MOSI 23
#define TFT_MISO -1
#define TFT_DC   27
#define TFT_CS   14
#define TFT_RST  33


class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7735S     _panel_instance;

    lgfx::Bus_SPI           _bus_instance;


public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config();

            // SPIバスの設定
            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 8000000;
            cfg.freq_read  = 4000000;

            cfg.dma_channel = SPI_DMA_CH_AUTO;

            cfg.pin_sclk = TFT_SCLK;
            cfg.pin_mosi = TFT_MOSI;
            cfg.pin_miso = TFT_MISO;
            cfg.pin_dc = TFT_DC;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        } 

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs = TFT_CS;
            cfg.pin_rst = TFT_RST;
            cfg.pin_busy = -1;

            cfg.panel_width = 80;
            cfg.panel_height = 160;
            cfg.offset_x = 26;
            cfg.offset_y = 1;
            cfg.offset_rotation = 0;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.invert = true;

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance);
    }
};
