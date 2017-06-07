/*
 * Copyright (c) 2016 RedBear
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */
 
/******************************************************
 *                      Macros
 ******************************************************/
/* 
 * Defaultly disabled. More details: https://docs.particle.io/reference/firmware/photon/#system-thread 
 */
//SYSTEM_THREAD(ENABLED);

/*
 * Defaultly disabled. If BLE setup is enabled, when the Duo is in the Listening Mode, it will de-initialize and re-initialize the BT stack.
 * Then it broadcasts as a BLE peripheral, which enables you to set up the Duo via BLE using the RedBear Duo App or customized
 * App by following the BLE setup protocol: https://github.com/redbear/Duo/blob/master/docs/listening_mode_setup_protocol.md#ble-peripheral 
 * 
 * NOTE: If enabled and upon/after the Duo enters/leaves the Listening Mode, the BLE functionality in your application will not work properly.
 */
//BLE_SETUP(ENABLED);

/*
 * SYSTEM_MODE:
 *     - AUTOMATIC: Automatically try to connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     - SEMI_AUTOMATIC: Manually connect to Wi-Fi and the Particle Cloud, but automatically handle the cloud messages.
 *     - MANUAL: Manually connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     
 * SYSTEM_MODE(AUTOMATIC) does not need to be called, because it is the default state. 
 * However the user can invoke this method to make the mode explicit.
 * Learn more about system modes: https://docs.particle.io/reference/firmware/photon/#system-modes .
 */
#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif

/* 
 * BLE peripheral preferred connection parameters:
 *     - Minimum connection interval = MIN_CONN_INTERVAL * 1.25 ms, where MIN_CONN_INTERVAL ranges from 0x0006 to 0x0C80
 *     - Maximum connection interval = MAX_CONN_INTERVAL * 1.25 ms,  where MAX_CONN_INTERVAL ranges from 0x0006 to 0x0C80
 *     - The SLAVE_LATENCY ranges from 0x0000 to 0x03E8
 *     - Connection supervision timeout = CONN_SUPERVISION_TIMEOUT * 10 ms, where CONN_SUPERVISION_TIMEOUT ranges from 0x000A to 0x0C80
 */
#define MIN_CONN_INTERVAL          0x0028 // 50ms.
#define MAX_CONN_INTERVAL          0x0190 // 500ms.
#define SLAVE_LATENCY              0x0000 // No slave latency.
#define CONN_SUPERVISION_TIMEOUT   0x03E8 // 10s.

// Learn about appearance: http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
#define BLE_PERIPHERAL_APPEARANCE  BLE_APPEARANCE_HID_MOUSE

#define BLE_DEVICE_NAME            "BLE_Mouse"

// Length of characteristic value.
#define BATTERY_LEVEL_CHARACTERISTIC_MAX_LEN              1
#define PNP_ID_CHARACTERISTIC_MAX_LEN                     7
#define HID_INFORMATION_CHARACTERISTIC_MAX_LEN            4
#define HID_CONTROL_POINT_CHARACTERISTIC_MAX_LEN          1
#define REPORT_MAP_VALUE_CHARACTERISTIC_MAX_LEN           64
#define BOOT_MOUSE_INPUT_REPORT_CHARACTERISTIC_MAX_LEN    3

// from: https://github.com/jpbrucker/BLE_HID/blob/master/BLE_HID/USBHID_Types.h
/* HID Class Report Descriptor */
/* Short items: size is 0, 1, 2 or 3 specifying 0, 1, 2 or 4 (four) bytes */
/* of data as per HID Class standard */

/* Main items */
#define IN(size)             (0x80 | size)
#define OUT(size)            (0x90 | size)
#define FEATURE(size)           (0xb0 | size)
#define COLLECTION(size)        (0xa0 | size)
#define END_COLLECTION(size)    (0xc0 | size)

/* Global items */
#define USAGE_PAGE(size)        (0x04 | size)
#define LOGICAL_MINIMUM(size)   (0x14 | size)
#define LOGICAL_MAXIMUM(size)   (0x24 | size)
#define PHYSICAL_MINIMUM(size)  (0x34 | size)
#define PHYSICAL_MAXIMUM(size)  (0x44 | size)
#define UNIT_EXPONENT(size)     (0x54 | size)
#define UNIT(size)              (0x64 | size)
#define REPORT_SIZE(size)       (0x74 | size)
#define REPORT_ID(size)         (0x84 | size)
#define REPORT_COUNT(size)      (0x94 | size)
#define PUSH(size)              (0xa4 | size)
#define POP(size)               (0xb4 | size)

/* Local items */
#define USAGE(size)                 (0x08 | size)
#define USAGE_MINIMUM(size)         (0x18 | size)
#define USAGE_MAXIMUM(size)         (0x28 | size)
#define DESIGNATOR_INDEX(size)      (0x38 | size)
#define DESIGNATOR_MINIMUM(size)    (0x48 | size)
#define DESIGNATOR_MAXIMUM(size)    (0x58 | size)
#define STRING_INDEX(size)          (0x78 | size)
#define STRING_MINIMUM(size)        (0x88 | size)
#define STRING_MAXIMUM(size)        (0x98 | size)
#define DELIMITER(size)             (0xa8 | size)

/******************************************************
 *               Variable Definitions
 ******************************************************/
static uint16_t conn_handle = 0xFFFF;
 
// Battery service 16-bits UUID
static uint16_t battery_service_uuid = 0x180F;
// Characteristics 16-bits UUID
static uint16_t battery_level_characteristic_uuid = 0x2A19; 

// Device information service 16-bits UUID
static uint16_t device_information_service_uuid = 0x180A;
// Characteristics 16-bits UUID
static uint8_t pnp_id_characteristic_uuid = 0x2A50;

//  HID service 16-bits UUID
static uint16_t hid_service_uuid = 0x1812;
// Characteristics 16-bits UUID
static uint16_t hid_information_characteristic_uuid = 0x2A4A;
static uint16_t hid_control_point_characteristic_uuid = 0x2A4C;
static uint16_t report_map_characteristic_uuid = 0x2A4B;
static uint16_t mouse_input_report_characteristic_uuid = 0x2A33;

// GAP and GATT characteristics value
static uint8_t  appearance[2] = { 
  LOW_BYTE(BLE_PERIPHERAL_APPEARANCE), 
  HIGH_BYTE(BLE_PERIPHERAL_APPEARANCE) 
};

static uint8_t  change[4] = {
  0x00, 0x00, 0xFF, 0xFF
};

static uint8_t  conn_param[8] = {
  LOW_BYTE(MIN_CONN_INTERVAL), HIGH_BYTE(MIN_CONN_INTERVAL), 
  LOW_BYTE(MAX_CONN_INTERVAL), HIGH_BYTE(MAX_CONN_INTERVAL), 
  LOW_BYTE(SLAVE_LATENCY), HIGH_BYTE(SLAVE_LATENCY), 
  LOW_BYTE(CONN_SUPERVISION_TIMEOUT), HIGH_BYTE(CONN_SUPERVISION_TIMEOUT)
};

/* 
 * BLE peripheral advertising parameters:
 *     - advertising_interval_min: [0x0020, 0x4000], default: 0x0800, unit: 0.625 msec
 *     - advertising_interval_max: [0x0020, 0x4000], default: 0x0800, unit: 0.625 msec
 *     - advertising_type: 
 *           BLE_GAP_ADV_TYPE_ADV_IND 
 *           BLE_GAP_ADV_TYPE_ADV_DIRECT_IND 
 *           BLE_GAP_ADV_TYPE_ADV_SCAN_IND 
 *           BLE_GAP_ADV_TYPE_ADV_NONCONN_IND
 *     - own_address_type: 
 *           BLE_GAP_ADDR_TYPE_PUBLIC 
 *           BLE_GAP_ADDR_TYPE_RANDOM
 *     - advertising_channel_map: 
 *           BLE_GAP_ADV_CHANNEL_MAP_37 
 *           BLE_GAP_ADV_CHANNEL_MAP_38 
 *           BLE_GAP_ADV_CHANNEL_MAP_39 
 *           BLE_GAP_ADV_CHANNEL_MAP_ALL
 *     - filter policies: 
 *           BLE_GAP_ADV_FP_ANY 
 *           BLE_GAP_ADV_FP_FILTER_SCANREQ 
 *           BLE_GAP_ADV_FP_FILTER_CONNREQ 
 *           BLE_GAP_ADV_FP_FILTER_BOTH
 *     
 * Note:  If the advertising_type is set to BLE_GAP_ADV_TYPE_ADV_SCAN_IND or BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, 
 *        the advertising_interval_min and advertising_interval_max should not be set to less than 0x00A0.
 */
static advParams_t adv_params = {
  .adv_int_min   = 0x001E,
  .adv_int_max   = 0x0032,
  .adv_type      = BLE_GAP_ADV_TYPE_ADV_IND,
  .dir_addr_type = BLE_GAP_ADDR_TYPE_PUBLIC,
  .dir_addr      = {0,0,0,0,0,0},
  .channel_map   = BLE_GAP_ADV_CHANNEL_MAP_ALL,
  .filter_policy = BLE_GAP_ADV_FP_ANY
};

// BLE peripheral advertising data
static uint8_t adv_data[] = {
  0x02,
  BLE_GAP_AD_TYPE_FLAGS,
  BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE, 
  
  0x11,
  BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE,
  0x00, 0x00, 0x11, 0x24, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB      
};

// BLE peripheral scan respond data
static uint8_t scan_response[] = {
  0x08,
  BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,
  'M', 'O', 'U', 'S', 'E', '-', '1'
};

// Characteristic value handle
static uint16_t battery_level_characteristic_handle = 0x0000;
static uint16_t pnp_id_characteristic_handle = 0x0000;
static uint16_t hid_information_characteristic_handle = 0x0000;
static uint16_t hid_control_point_characteristic_handle = 0x0000;
static uint16_t report_map_characteristic_handle = 0x0000;
static uint16_t mouse_input_report_characteristic_handle = 0x0000;

// Buffer of characterisitc value.
static uint8_t battery_level_characteristic_data[BATTERY_LEVEL_CHARACTERISTIC_MAX_LEN] = { 0x5A };
static uint8_t pnp_id_characteristic_data[PNP_ID_CHARACTERISTIC_MAX_LEN] = { 0x01, 0xFF, 0xFF, 0xAA, 0xAA, 0x00, 0x01 };
static uint8_t hid_information_characteristic_data[HID_INFORMATION_CHARACTERISTIC_MAX_LEN] = { 0x01, 0x11, 0x00, 0x02 };
static uint8_t hid_control_point_characteristic_data[HID_CONTROL_POINT_CHARACTERISTIC_MAX_LEN] = { 0x00 };
// from: https://github.com/jpbrucker/BLE_HID/blob/master/BLE_HID/MouseService.h
static uint8_t report_map_characteristic_data[REPORT_MAP_VALUE_CHARACTERISTIC_MAX_LEN] = { 
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x02,         // Mouse
    COLLECTION(1),      0x01,         // Application
    USAGE(1),           0x01,         //  Pointer
    COLLECTION(1),      0x00,         //  Physical
    USAGE_PAGE(1),      0x09,         //   Buttons
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x03,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x02,         //   3 bits (Buttons)
    REPORT_SIZE(1),     0x01,
    IN(1),              0x02,         //   Data, Variable, Absolute
    REPORT_COUNT(1),    0x01,         //   5 bits (Padding)
    REPORT_SIZE(1),     0x05,
    IN(1),              0x01,         //   Constant
    USAGE_PAGE(1),      0x01,         //   Generic Desktop
    USAGE(1),           0x30,         //   X
    USAGE(1),           0x31,         //   Y
    LOGICAL_MINIMUM(1), 0x81,         //   -127
    LOGICAL_MAXIMUM(1), 0x7f,         //   127
    REPORT_SIZE(1),     0x08,         //   Three bytes
    REPORT_COUNT(1),    0x03,
    IN(1),              0x06,         //   Data, Variable, Relative
    END_COLLECTION(0),
    END_COLLECTION(0),
  };
static uint8_t mouse_input_report_characteristic_data[BOOT_MOUSE_INPUT_REPORT_CHARACTERISTIC_MAX_LEN] = { 0x02, 0x32, 0x23 };

// Timer task.
//static btstack_timer_source_t characteristic2;
/******************************************************
 *               Function Definitions
 ******************************************************/
/**
 * @brief Connect handle.
 *
 * @param[in]  status   BLE_STATUS_CONNECTION_ERROR or BLE_STATUS_OK.
 * @param[in]  handle   Connect handle.
 *
 * @retval None
 */
void deviceConnectedCallback(BLEStatus_t status, uint16_t handle) {
  switch (status) {
    case BLE_STATUS_OK:
      Serial.println("Device connected!");
      Serial.print("Connection handle: ");
      Serial.println(handle, HEX);
      conn_handle = handle;
      break;
    case BLE_STATUS_CONNECTION_ERROR:
      Serial.println("Connection error!");
      break;
  }
}

/**
 * @brief Disconnect handle.
 *
 * @param[in]  handle   Connect handle.
 *
 * @retval None
 */
void deviceDisconnectedCallback(uint16_t handle) {
  Serial.println("Disconnected.");
  Serial.print("Connection handle: ");
  Serial.println(handle, HEX);
  conn_handle = 0xFFFF;
}

/**
 * @brief Callback for reading event.
 *
 * @note  If characteristic contains client characteristic configuration,then client characteristic configration handle is value_handle+1.
 *        Now can't add user_descriptor.
 *
 * @param[in]  value_handle    
 * @param[in]  buffer 
 * @param[in]  buffer_size    Ignore it.
 *
 * @retval  Length of current attribute value.
 */
uint16_t gattReadCallback(uint16_t value_handle, uint8_t * buffer, uint16_t buffer_size) {   
  uint8_t characteristic_len = 0;

  Serial.print("Read value handler: ");
  Serial.println(value_handle, HEX);

  if (battery_level_characteristic_handle == value_handle){
    Serial.println("Battery level characteristic read:");
    memcpy(buffer, battery_level_characteristic_data, BATTERY_LEVEL_CHARACTERISTIC_MAX_LEN);
    characteristic_len = BATTERY_LEVEL_CHARACTERISTIC_MAX_LEN;
  } 
  else if (pnp_id_characteristic_handle == value_handle){
    Serial.println("PNP ID characteristic read:");
    memcpy(buffer, pnp_id_characteristic_data, PNP_ID_CHARACTERISTIC_MAX_LEN);
    characteristic_len = PNP_ID_CHARACTERISTIC_MAX_LEN;
  } 
  else if (hid_information_characteristic_handle == value_handle){
    Serial.println("HID information characteristic read:");
    memcpy(buffer, hid_information_characteristic_data, HID_INFORMATION_CHARACTERISTIC_MAX_LEN);
    characteristic_len = HID_INFORMATION_CHARACTERISTIC_MAX_LEN;
  } 
  else if (report_map_characteristic_handle == value_handle){
    Serial.println("HID information characteristic read:");
    memcpy(buffer, hid_information_characteristic_data, HID_INFORMATION_CHARACTERISTIC_MAX_LEN);
    characteristic_len = HID_INFORMATION_CHARACTERISTIC_MAX_LEN;
  } 
  else if (mouse_input_report_characteristic_handle == value_handle){
    Serial.println("Mouse input report characteristic read:");
    memcpy(buffer, mouse_input_report_characteristic_data, BOOT_MOUSE_INPUT_REPORT_CHARACTERISTIC_MAX_LEN);
    characteristic_len = BOOT_MOUSE_INPUT_REPORT_CHARACTERISTIC_MAX_LEN;
  } 
  
  return characteristic_len;
}

/**
 * @brief Callback for writting event.
 *
 * @param[in]  value_handle  
 * @param[in]  *buffer       The buffer pointer of writting data.
 * @param[in]  size          The length of writting data.   
 *
 * @retval 
 */
int gattWriteCallback(uint16_t value_handle, uint8_t *buffer, uint16_t size) {
  Serial.print("Write value handler: ");
  Serial.println(value_handle, HEX);

  if (hid_control_point_characteristic_handle == value_handle) {
    memcpy(hid_control_point_characteristic_data, buffer, size);
    Serial.print("HID control point characteristic write value: ");
    for (uint8_t index = 0; index < size; index++) {
      Serial.print(hid_control_point_characteristic_data[index], HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  return 0;
}

/**
 * @brief Timer task for sending notify of characteristic to client.
 *
 * @param[in]  *ts   
 *
 * @retval None
 */
static void mouse_input_report_characteristic_notify(btstack_timer_source_t *ts) {
  Serial.println("Mouse input report characteristic - notify");

  mouse_input_report_characteristic_data[BOOT_MOUSE_INPUT_REPORT_CHARACTERISTIC_MAX_LEN-1]++;
  ble.sendNotify(mouse_input_report_characteristic_handle, mouse_input_report_characteristic_data, BOOT_MOUSE_INPUT_REPORT_CHARACTERISTIC_MAX_LEN);
  // Restart timer.
  ble.setTimer(ts, 10000);
  ble.addTimer(ts);
}

/**
 * @brief Setup.
 */

SerialDebugOutput debugOutput(115200, DEBUG_LEVEL);

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("BLE mouse demo.");

  // Open debugger, must befor init().
    ble.debugLogger(true);
    ble.debugError(true);
    ble.debugInfo(true);
//  ble.enablePacketLogger();
    
  // Initialize ble_stack.
  ble.init();

  // Register BLE callback functions.
  ble.onConnectedCallback(deviceConnectedCallback);
  ble.onDisconnectedCallback(deviceDisconnectedCallback);
  ble.onDataReadCallback(gattReadCallback);
  ble.onDataWriteCallback(gattWriteCallback);

  // Add GAP service and characteristics
  ble.addService(BLE_UUID_GAP);
  ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_DEVICE_NAME, ATT_PROPERTY_READ|ATT_PROPERTY_WRITE, (uint8_t*)BLE_DEVICE_NAME, sizeof(BLE_DEVICE_NAME));
  ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_APPEARANCE, ATT_PROPERTY_READ, appearance, sizeof(appearance));
  ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_PPCP, ATT_PROPERTY_READ, conn_param, sizeof(conn_param));

  // Add GATT service and characteristics
  ble.addService(BLE_UUID_GATT);
  ble.addCharacteristic(BLE_UUID_GATT_CHARACTERISTIC_SERVICE_CHANGED, ATT_PROPERTY_INDICATE, change, sizeof(change));

  // Add primary Battery service.
  ble.addService(battery_service_uuid);
  // Add battery level characteristic to battery service, return value handle of characteristic.
  battery_level_characteristic_handle = ble.addCharacteristicDynamic(battery_level_characteristic_uuid, ATT_PROPERTY_READ, battery_level_characteristic_data, BATTERY_LEVEL_CHARACTERISTIC_MAX_LEN);

  // Add primary Device Information service.
  ble.addService(device_information_service_uuid);
  // Add PNP ID characteristic to device infortation service, return value handle of characteristic.
  pnp_id_characteristic_handle = ble.addCharacteristicDynamic(pnp_id_characteristic_uuid, ATT_PROPERTY_READ, pnp_id_characteristic_data, PNP_ID_CHARACTERISTIC_MAX_LEN);

  // Add primary HID service.
  ble.addService(hid_service_uuid);
  // Add HID information characteristic to HID service, return value handle of characteristic.
  hid_information_characteristic_handle = ble.addCharacteristicDynamic(hid_information_characteristic_uuid, ATT_PROPERTY_READ, hid_information_characteristic_data, HID_INFORMATION_CHARACTERISTIC_MAX_LEN);
  // Add HID control point characteristic to HID service, return value handle of characteristic.
  hid_control_point_characteristic_handle = ble.addCharacteristicDynamic(hid_control_point_characteristic_uuid, ATT_PROPERTY_WRITE_WITHOUT_RESPONSE, hid_control_point_characteristic_data, HID_CONTROL_POINT_CHARACTERISTIC_MAX_LEN);
  // Add report map characteristic to HID service, return value handle of characteristic.
  report_map_characteristic_handle = ble.addCharacteristicDynamic(report_map_characteristic_uuid, ATT_PROPERTY_READ, report_map_characteristic_data, REPORT_MAP_VALUE_CHARACTERISTIC_MAX_LEN);
  // Add mouse input report characteristic to HID service, return value handle of characteristic.
  mouse_input_report_characteristic_handle = ble.addCharacteristicDynamic(mouse_input_report_characteristic_uuid, ATT_PROPERTY_READ | ATT_PROPERTY_NOTIFY, mouse_input_report_characteristic_data, BOOT_MOUSE_INPUT_REPORT_CHARACTERISTIC_MAX_LEN);
  
  // Set BLE advertising parameters
  ble.setAdvertisementParams(&adv_params);

  // Set BLE advertising and scan respond data
  ble.setAdvertisementData(sizeof(adv_data), adv_data);
  ble.setScanResponseData(sizeof(scan_response), scan_response);
  
  // Start advertising.
  ble.startAdvertising();
  Serial.println("BLE start advertising.");
    
//  // set one-shot timer
//  mouse_input_report_characteristic.process = &mouse_input_report_characteristic_notify;
//  ble.setTimer(&mouse_input_report_characteristic, 10000);
//  ble.addTimer(&mouse_input_report_characteristic);
}


void loop() {
  // put your main code here, to run repeatedly:
//  delay(5000);
//  Serial.println("BLE mouse demo.");
}
