// #include <lvgl.h>
// #include <TFT_eSPI.h>
// #include <SPI.h>
// #include <SD.h>

// TFT_eSPI tft = TFT_eSPI();

// #define SD_CS 5  // SD card chip select pin

// // LVGL flush callback function to update the display
// void my_disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p) {
//     int32_t w = lv_area_get_width(area);
//     int32_t h = lv_area_get_height(area);

//     tft.startWrite();
//     tft.setAddrWindow(area->x1, area->y1, w, h);
//     tft.pushColors((uint16_t*)color_p, w * h, true);
//     tft.endWrite();

//     lv_disp_flush_ready(disp);
// }

// // Function to open a file from SD card
// void* sd_fs_open(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode) {
//     File* file = new File(SD.open(path, (mode == LV_FS_MODE_RD) ? FILE_READ : FILE_WRITE));
//     if (!file || !file->available()) {
//         delete file;
//         return NULL;
//     }
//     return (void*)file;
// }

// // Function to read data from SD card
// lv_fs_res_t sd_fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br) {
//     File* file = (File*)file_p;
//     *br = file->read((uint8_t*)buf, btr);
//     return LV_FS_RES_OK;
// }

// // Function to close an SD card file
// lv_fs_res_t sd_fs_close(lv_fs_drv_t* drv, void* file_p) {
//     File* file = (File*)file_p;
//     file->close();
//     delete file;
//     return LV_FS_RES_OK;
// }

// // Function to set file position
// lv_fs_res_t sd_fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence) {
//     File* file = (File*)file_p;
//     file->seek(pos, (whence == LV_FS_SEEK_SET) ? SeekSet : SeekCur);
//     return LV_FS_RES_OK;
// }

// // Function to get file position
// lv_fs_res_t sd_fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p) {
//     File* file = (File*)file_p;
//     *pos_p = file->position();
//     return LV_FS_RES_OK;
// }

// void setup() {
//     Serial.begin(115200);
    
//     // Initialize TFT display
//     tft.begin();
//     tft.setRotation(1);
//     tft.fillScreen(TFT_BLACK);

//     // Initialize LVGL
//     lv_init();

//     // Corrected LVGL draw buffer initialization
//     static lv_color_t buf1[240 * 10];  // Adjust buffer size as needed
//     static lv_draw_buf_t draw_buf;

//     lv_draw_buf_init(&draw_buf, 240, 320, LV_COLOR_FORMAT_ARGB8888, 240, buf1, sizeof(buf1));

//     // Register the display driver
//     lv_display_t* disp = lv_display_create(240, 320);
//     lv_display_set_flush_cb(disp, my_disp_flush);
//     lv_display_set_draw_buffers(disp, &draw_buf, NULL);

//     // Initialize the SD card
//     if (!SD.begin(SD_CS)) {
//         Serial.println("SD Card initialization failed!");
//         return;
//     } else {
//         Serial.println("SD Card initialized successfully.");
//     }

//     // Register the SD card filesystem to LVGL
//     lv_fs_drv_t fs_drv;
//     lv_fs_drv_init(&fs_drv);
//     fs_drv.letter = 'S';  // Use 'S' as the prefix to access files
//     fs_drv.open_cb = sd_fs_open;
//     fs_drv.read_cb = sd_fs_read;
//     fs_drv.close_cb = sd_fs_close;
//     fs_drv.seek_cb = sd_fs_seek;
//     fs_drv.tell_cb = sd_fs_tell;
//     lv_fs_drv_register(&fs_drv);

//     // Load and display image from SD card
//     lv_obj_t *img = lv_image_create(lv_screen_active());
//     lv_image_set_src(img, "S:/my_image.c");
//     lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
// }

// void loop() {
//     lv_timer_handler();  // Handle LVGL tasks
//     delay(5);
// }