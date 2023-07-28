#include "dmamatrix.h"
#include "esphome/core/application.h"
#include "esp_log.h"

#define R1_PIN 25
#define G1_PIN 26
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13

#define A_PIN 23
#define B_PIN 19
#define C_PIN 5
#define D_PIN 17
#define E_PIN 18 
          
#define LAT_PIN 4
#define OE_PIN 15

#define CLK_PIN 16

static const char *TAG = "dmamatrix_display";

namespace esphome {
namespace dmamatrix_display {

void display_updater() {  }

float DmaMatrixDisplay::get_setup_priority() const { return setup_priority::PROCESSOR; }

void DmaMatrixDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Starting setup...");

  HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
  HUB75_I2S_CFG mxconfig(
      width_,   // module width
      height_,   // module height
      2,    // Chain length
      _pins, // pin mapping
      HUB75_I2S_CFG::FM6124
    );

  mxconfig.double_buff = true;
  mxconfig.clkphase = false;

  this->dma_matrix_ = new MatrixPanel_I2S_DMA(mxconfig);
  dma_matrix_->begin();
  dma_matrix_->setBrightness8(this->brightness_); //0-255
  dma_matrix_->clearScreen();
  ESP_LOGI(TAG, "Finished Setup");
}

void HOT DmaMatrixDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  uint16_t matrix_color = display::ColorUtil::color_to_565(color, display::ColorOrder::COLOR_ORDER_BGR);
  this->dma_matrix_->drawPixel(x, y, matrix_color);
}

void DmaMatrixDisplay::fill(Color color) {
  uint16_t matrix_color = display::ColorUtil::color_to_565(color,  display::ColorOrder::COLOR_ORDER_BGR);
  this->dma_matrix_->fillScreen(matrix_color);
}

void DmaMatrixDisplay::update() {
  this->dma_matrix_->flipDMABuffer();
  this->do_update_();
}

void HOT DmaMatrixDisplay::display() {}

// void DmaMatrixDisplay::set_pin_latch(IDFInternalGPIOPin *pin_latch) { this->pin_latch_ = pin_latch; }

// void DmaMatrixDisplay::set_pin_r1(IDFInternalGPIOPin *pin_r1) { this->pin_r1_ = pin_r1; }

// void DmaMatrixDisplay::set_pin_r2(IDFInternalGPIOPin *pin_r2) { this->pin_r2_ = pin_r2; }

// void DmaMatrixDisplay::set_pin_g1(IDFInternalGPIOPin *pin_g1) { this->pin_g1_ = pin_g1; }

// void DmaMatrixDisplay::set_pin_g2(IDFInternalGPIOPin *pin_g2) { this->pin_g2_ = pin_g2; }

// void DmaMatrixDisplay::set_pin_b1(IDFInternalGPIOPin *pin_b1) { this->pin_b1_ = pin_b1; }

// void DmaMatrixDisplay::set_pin_b2(IDFInternalGPIOPin *pin_b2) { this->pin_b2_ = pin_b2; }

// void DmaMatrixDisplay::set_pin_clk(IDFInternalGPIOPin *pin_clk) { this->pin_clk_ = pin_clk; }

// void DmaMatrixDisplay::set_pin_a(IDFInternalGPIOPin *pin_a) { this->pin_a_ = pin_a; }

// void DmaMatrixDisplay::set_pin_b(IDFInternalGPIOPin *pin_b) { this->pin_b_ = pin_b; }

// void DmaMatrixDisplay::set_pin_c(IDFInternalGPIOPin *pin_c) { this->pin_c_ = pin_c; }

// void DmaMatrixDisplay::set_pin_d(IDFInternalGPIOPin *pin_d) { this->pin_d_ = pin_d; }

// void DmaMatrixDisplay::set_pin_e(IDFInternalGPIOPin *pin_e) { this->pin_e_ = pin_e; }

// void DmaMatrixDisplay::set_pin_oe(IDFInternalGPIOPin *pin_oe) { this->pin_oe_ = pin_oe; }

void DmaMatrixDisplay::set_width(uint8_t width) { this->width_ = width; }

void DmaMatrixDisplay::set_height(uint8_t height) { this->height_ = height; }

void DmaMatrixDisplay::set_brightness(uint8_t brightness) { this->brightness_ = brightness; }

int DmaMatrixDisplay::get_width_internal() { return this->width_; }

int DmaMatrixDisplay::get_height_internal() { return this->height_; }

}  // namespace dmamatrix_display
}  // namespace esphome
