#ifndef BLE_INIT_H__
#define	BLE_INIT_H__


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "app_timer.h"
#include "ble_nus.h"


#define IS_SRVC_CHANGED_CHARACT_PRESENT		0

//以下2个变量会决定SD的运行空间(app_ram_base.h有定义)，从而决定编译的时候，APP的运行空间设置
#define CENTRAL_LINK_COUNT					0
#define PERIPHERAL_LINK_COUNT				1

#define APP_COMPANY_ID						0x5a5a			//自定义厂商ID

#define DEVICE_NAME							"tecsheild_door"//蓝牙设备名称，蓝牙广播给其他设备的名字
#define DEVICE_NAME_SIZE					31 //名称最长20 -2字节，设备规定最长31
//#define MANUFACTURER_NAME					"NordicSemiconductor"   //设备制造商，Will be passed to Device Information Service
//#define MODEL_NUMBER						"nRF52"// 型号字符串. Will be passed to Device Information Service.
//#define MANUFACTURER_ID					0x55AA55AA55 //设备制造商ID(可修改为自己的). Will be passed to Device Information Service.
//#define ORG_UNIQUE_ID						0xEEBBEE //BLE组织联盟中唯一的ID. Will be passed to Device Information Service

// UUID type for the Nordic UART Service (vendor specific)，主要是可以用官方的APP测试
#define NUS_SERVICE_UUID_TYPE				BLE_UUID_TYPE_VENDOR_BEGIN

#define APP_ADV_FAST_INTERVAL					640 //快速广播间隔(0.625 ms * 64 = 40 ms)，广播间隔越大，越省电
#define APP_ADV_FAST_TIMEOUT_IN_SECONDS			5//30 //快速广播超时，单位s
#define APP_ADV_SLOW_INTERVAL					8000 //慢速广播间隔(0.625 ms * 8000 = 5 s)，广播间隔越大，越省电
#define APP_ADV_SLOW_TIMEOUT_IN_SECONDS			0//0//慢速广播超时，单位s


#define APP_TIMER_PRESCALER					0 // Value of the RTC1 PRESCALER register
#define APP_TIMER_MAX_TIMERS            	4
#define APP_TIMER_OP_QUEUE_SIZE				4 //Size of timer operation queues

//配对定时器
//APP_TIMER_DEF(m_sec_req_timer_id);
//开启定时广播
APP_TIMER_DEF(m_ad_repeat_timer_id);

#define SECURITY_REQUEST_DELAY				APP_TIMER_TICKS(4000, APP_TIMER_PRESCALER)		//配对定时器时间间隔：4000MS
#define AD_REPEAT_DELAY						APP_TIMER_TICKS(20000,APP_TIMER_PRESCALER)	//重复广播时间间隔：20000MS


#define MIN_CONN_INTERVAL					MSEC_TO_UNITS(20, UNIT_1_25_MS)
#define MAX_CONN_INTERVAL					MSEC_TO_UNITS(75, UNIT_1_25_MS)
#define SLAVE_LATENCY						0
#define CONN_SUP_TIMEOUT					MSEC_TO_UNITS(4000, UNIT_10_MS)
#define FIRST_CONN_PARAMS_UPDATE_DELAY		APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)
#define NEXT_CONN_PARAMS_UPDATE_DELAY		APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)
#define MAX_CONN_PARAMS_UPDATE_COUNT		3

#define SEC_PARAM_BOND						0
#define SEC_PARAM_MITM						1

//如果下面的属性换成BLE_GAP_IO_CAPS_NONE，则无需输出配对密码，就可以配对
#define SEC_PARAM_IO_CAPABILITIES			BLE_GAP_IO_CAPS_DISPLAY_ONLY//BLE_GAP_IO_CAPS_NONE                       	/**< No I/O capabilities. */
#define SEC_PARAM_OOB						0
#define SEC_PARAM_MIN_KEY_SIZE				7
#define SEC_PARAM_MAX_KEY_SIZE				16


#define DEAD_BEEF																								0xDEADBEEF

//广播设备类型(外设种类)
#define BIT_TOUCH		BIT_0
#define	BIT_FIG			BIT_1



//#define APP_FEATURE_NOT_SUPPORTED      	BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2

#define UART_TX_BUF_SIZE					512
#define UART_RX_BUF_SIZE					512


extern ble_nus_t				m_nus; /*Nordic UART Service*/
extern uint16_t					m_conn_handle;
//extern app_timer_id_t			m_backlit_timer_id;

extern uint8_t					mac[8];//第一位：标志位，第二位：长度

//以下3个变量是在uart service中保存的全局变量，交给operate_code_check函数去处理,蓝牙串口接收的数据
extern bool							operate_code_setted;
extern uint8_t						nus_data_recieve[BLE_NUS_MAX_DATA_LEN];
extern uint16_t						nus_data_recieve_length;

extern uint8_t						nus_data_send[BLE_NUS_MAX_DATA_LEN];//20位,发送给蓝牙串口的数据
extern uint32_t						nus_data_send_length;//测试数据存储时，长度的全局变量

//指纹模块发送给蓝牙芯片的数据
extern uint8_t						fig_recieve_data[UART_RX_BUF_SIZE];
extern uint16_t						fig_recieve_data_length;

//在编译的时候，代替弱连接，做ble的回调函数
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);

void timers_init(void);
void application_timers_start(void);
//BLE启动的设置函数
void gap_params_init(void);
void services_init(void);
void conn_params_init(void);
void ble_stack_init(void);
void advertising_init(void);
void adverts_start(void);
void power_manage(void);
void device_manager_init(bool erase_bonds);
void buttons_leds_init(bool * p_erase_bonds);

//与BLE有部分分离，接收板子UART0的数据，通过蓝牙进行传输出去
//初始化uart，供给板子上的application使用，(工程的编译选项有NRF_LOG_USES_UART=1 )
void uart_init(void);

#endif		//BLE_INIT_H__
