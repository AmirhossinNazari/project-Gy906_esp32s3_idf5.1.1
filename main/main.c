

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#define  I2C_FREQ       100000
#define  I2C_GPIO_SDA   GPIO_NUM_17
#define  I2C_GPIO_SCL   GPIO_NUM_18
#define  I2C_PORT_NUM   I2C_NUM_1
#define  ACK_CHECK_EN    0x1
#define  ACK_CHECK_DES   0x0
#define  ACK_VAL         0x0
#define  NACK_VAL        0x1


static const char *TAG = "Gy906";
uint8_t *buffer;
uint8_t l_data ,h_data,p_data;
float r_data ;
int ret;
float result_data;

//+++++++++++++++++++++++++++++++++++++

void gy906_init_read(uint8_t *low_data ,uint8_t *high_data , uint8_t *pec_data  ){
      


      i2c_cmd_handle_t cmd = i2c_cmd_link_create();

      ret = i2c_master_start(cmd);

      ret = i2c_master_write_byte(cmd, 0xB4 , ACK_CHECK_EN  );

      ret = i2c_master_write_byte(cmd, 0x07 , ACK_CHECK_EN );     

      ret = i2c_master_start(cmd);   
    
      ret = i2c_master_write_byte(cmd, 0xB5 , ACK_CHECK_EN );  

      ret = i2c_master_read_byte(cmd, low_data   , NACK_VAL);
      ret = i2c_master_read_byte(cmd, high_data  , NACK_VAL);
      ret = i2c_master_read_byte(cmd, pec_data   ,  ACK_VAL);
     
      ret = i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_PERIOD_MS);

      i2c_cmd_link_delete(cmd);

      usleep(1000);


}



void app_main()
{

i2c_config_t  i2c_conf ={


        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_GPIO_SDA,
        .scl_io_num = I2C_GPIO_SCL,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_FREQ,
		.clk_flags = 0,

        
    };
//___________________________________________________________________
i2c_param_config(I2C_PORT_NUM ,  &i2c_conf);
//___________________________________________________________________
i2c_driver_install(I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0);
//___________________________________________________________________


while (1)
{
    
   // ESP_LOGE(TAG, "__________gy906__________: ");

    gy906_init_read(&l_data ,&h_data,&p_data );
    
    result_data =((((( h_data & 0x007F )<<8)+ l_data )*0.02)-0.01);

    result_data = result_data - 273.15 ;

    printf("Temp : %f\n",result_data);

    //ESP_LOGE(TAG, "_________end___________: ");

    vTaskDelay(1000 / portTICK_PERIOD_MS);



  }



}

 





























