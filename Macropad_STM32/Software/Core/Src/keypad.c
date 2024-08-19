#include "keypad.h"
#include "usb_device.h"
#include "usbd_hid.h"

static uint8_t HID_Buffer[8];

extern USBD_HandleTypeDef hUsbDeviceFS; 

static char key_map[4][4] = 
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char Keypad_Scan(void)
{
    // Loop through columns, counting down
    for (int col = 3; col >= 0; col--)
    {
        // Shift initial pin to the correct pin, starting at PA6 to PA3
        HAL_GPIO_WritePin(GPIOA, COL3_Pin >> (3 - col), GPIO_PIN_RESET);

        // Check if any of the rows are low
        if (HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW0_Pin) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW0_Pin) == GPIO_PIN_RESET); // Wait for key to be released
            return key_map[0][col];
        }
        if (HAL_GPIO_ReadPin(ROW1_GPIO_Port, ROW1_Pin) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW1_Pin) == GPIO_PIN_RESET); // Wait for key to be released
            return key_map[1][col];
        }
        if (HAL_GPIO_ReadPin(ROW2_GPIO_Port, ROW2_Pin) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW2_Pin) == GPIO_PIN_RESET); // Wait for key to be released
            return key_map[2][col];
        }
        if (HAL_GPIO_ReadPin(ROW3_GPIO_Port, ROW3_Pin) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW3_Pin) == GPIO_PIN_RESET); // Wait for key to be released
            return key_map[3][col];
        }

        // Set column back to high
        HAL_GPIO_WritePin(GPIOA, COL3_Pin >> (3 - col), GPIO_PIN_SET);
    }

    return '\0'; // No key pressed
}

void Keypad_SendKeyViaUSB(char key)
{
    uint8_t hid_keycode = 0;

    // Convert ASCII to HID keycode, probably will integrate this to the key_map later

    if (key >= '0' && key <= '9') {
        hid_keycode = 0x27 + (key - '0'); // 0x27 is HID code for '0'
    } else if (key == 'A') {
        hid_keycode = 0x04; // HID code for 'A'
    } else if (key == 'B') {
        hid_keycode = 0x05; // HID code for 'B'
    } else if (key == 'C') {
        hid_keycode = 0x06; // HID code for 'C'
    } else if (key == 'D') {
        hid_keycode = 0x07; // HID code for 'D'
    } else if (key == '*') {
        hid_keycode = 0x55; // HID code for '*'
    } else if (key == '#') {
        hid_keycode = 0x2F; // HID code for '#'
    }

    // Prepare HID report
    HID_Buffer[0] = 0x00; // Modifier byte
    HID_Buffer[2] = hid_keycode; //Keycode byte
    USBD_HID_SendReport(&hUsbDeviceFS, HID_Buffer, sizeof(HID_Buffer));

    // Clear buffer
    HID_Buffer[2] = 0x00;
    USBD_HID_SendReport(&hUsbDeviceFS, HID_Buffer, sizeof(HID_Buffer));

}