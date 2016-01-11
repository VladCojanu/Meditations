#include <pebble.h>
//To add: Blutooth,
// Clean up code 
// Figure out how to implement a second window 

static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_layer, *s_moto_layer, *s_battery_layer, *s_quote_layer;
static GFont s_time_font, s_moto_font;
static BitmapLayer *s_background_layer, *s_bluetooth_layer;
static GBitmap *s_background_bitmap, *s_bluetooth_bitmap;
static bool quote_is_shown = false; 
static int quote_number =1;
// Layer Setups
static void date_layer_setup(){
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text(s_date_layer, "00/00/00");
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
}
static void time_layer_setup(){
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_24));
  text_layer_set_font(s_time_layer, s_time_font);

}
static void moto_layer_setup(){
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_moto_layer, GColorClear);
  text_layer_set_text_color(s_moto_layer, GColorBlack);
  text_layer_set_text(s_moto_layer, "DO MORE!");
  text_layer_set_text_alignment(s_moto_layer, GTextAlignmentCenter);
  s_moto_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_24));
  text_layer_set_font(s_moto_layer, s_moto_font);
}
static void battery_layer_setup(){
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorClear);
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  // Create GFont
  // Apply font to TextLayer
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

}
static void quote_layer_setup(){
    
  text_layer_set_background_color(s_quote_layer, GColorClear);
  text_layer_set_text_color(s_quote_layer, GColorClear);
  text_layer_set_text_alignment(s_quote_layer, GTextAlignmentLeft);
  // Create GFont
  // Apply font to TextLayer
  text_layer_set_font(s_quote_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
}



static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time[8];
  strftime(s_time, sizeof(s_time), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  static char s_date[16];
  strftime(s_date, sizeof(s_date), "%a %d %b", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time);
  //Displays date on text layer
  text_layer_set_text(s_date_layer, s_date);
}

// Handlers
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  }

static void tap_handler(AccelAxisType axis, int32_t direction) {
    // Try to setup the layer using it simply (show & hide), else show and hide everything 
  // If quotes shown: hiden them else, show them. 
  if (quote_is_shown == false){
      quote_is_shown = true;
      // show & change quote
     text_layer_set_background_color(s_quote_layer, GColorBlack);
    
    switch (quote_number){
      
      case 1: 
      quote_number++;
      text_layer_set_text(s_quote_layer,       "Never, never, never give up. ~ Winston Churchill");

      break;

      case 2:
      quote_number++;
      text_layer_set_text(s_quote_layer,  "When you are going through hell, keep going. ~ Winston Churchill");

      break;
      
      case 3:
      quote_number++;
      text_layer_set_text(s_quote_layer,       "That which is not good for the bee-hive cannot be good for the bees. ~ Marcus Aurelius");

      break;
    
      case 4:
      quote_number++;
      text_layer_set_text(s_quote_layer,  " A man's worth is no greater than his ambitions. ~ Marcus Aurelius");

      break;
      
      case 5:
      quote_number++;
      text_layer_set_text(s_quote_layer," Silence is a source of great strength. ~ Lao Tzu");

      break;

      case 6:
      quote_number++;
      text_layer_set_text(s_quote_layer,  "He who controls others may be powerful, but he who has mastered himself is mightier still. ~ Lao Tzu");

      break;
      
      case 7:
      quote_number = 1;
      text_layer_set_text(s_quote_layer,  "You have to learn the rules of the game. And then you have to play better than anyone else. ~ Albert Einstein");

      break;
    }
    
    
  } else {
    quote_is_shown = false;
    // hide quote
    text_layer_set_background_color(s_quote_layer, GColorClear);
     text_layer_set_text(s_quote_layer, "");
  }
  
  
  // Change quote text
  
  
}
static void connection_handler(bool connected) {
  if (connected){
    s_bluetooth_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_ON); 
  } else{
    s_bluetooth_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_OFF);     
  }
}
static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "charging";

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "charging");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(s_battery_layer, battery_text);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
   GRect bounds = layer_get_bounds(window_layer);

  // Background Bitmap GBitmaps
s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CASEY); 
// Create BitmapLayer to display the GBitmap
s_background_layer = bitmap_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 17), bounds.size.w, bounds.size.h -17));
// Set the bitmap onto the layer and add to the window
bitmap_layer_set_alignment(s_background_layer,GAlignTop);  
bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  
  // Date layer
  s_date_layer = text_layer_create( GRect(0, PBL_IF_ROUND_ELSE(58, 0), bounds.size.w, 17));
  date_layer_setup();
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  //Time Layer
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 140), bounds.size.w, 50));
  time_layer_setup();
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  //Moto layer
  s_moto_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 110), bounds.size.w, 50));
  moto_layer_setup();
  layer_add_child(window_layer, text_layer_get_layer(s_moto_layer));
  
  //Quote Layer
 s_quote_layer = text_layer_create(GRect(0, 17, bounds.size.w, bounds.size.h-17));
 quote_layer_setup();
  layer_add_child(window_layer, text_layer_get_layer(s_quote_layer));
 accel_tap_service_subscribe(tap_handler);
  
  //Battery Layer
  s_battery_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 0), bounds.size.w-3, 17));
  battery_layer_setup();

  static char s_battery_buffer[8];
  BatteryChargeState charge_state = battery_state_service_peek();
if (charge_state.is_charging) {
  snprintf(s_battery_buffer, sizeof(s_battery_buffer), "charging");
} else {
  snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", charge_state.charge_percent);

  text_layer_set_text(s_battery_layer, s_battery_buffer);
  battery_state_service_subscribe(handle_battery);
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
  
  // Bluetooth Layer 
  connection_handler(bluetooth_connection_service_peek());
  bluetooth_connection_service_subscribe(connection_handler);
  // Create BitmapLayer to display the GBitmap
  s_bluetooth_layer = bitmap_layer_create(GRect(3, PBL_IF_ROUND_ELSE(58, 0), 17, 17));
  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_alignment(s_bluetooth_layer,GAlignLeft);  
  bitmap_layer_set_bitmap(s_bluetooth_layer, s_bluetooth_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bluetooth_layer)); 
 
  
}

}


static void main_window_unload(Window *window) {
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_moto_font);
  // Destroy TextLayers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_quote_layer);
  
  // Unsubscribe
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe(); 
  bluetooth_connection_service_unsubscribe();
  accel_tap_service_unsubscribe();
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimer
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}