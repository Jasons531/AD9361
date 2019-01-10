
硬件接口：

USART1			PA9		USART1_TX
				PA10	USART1_RX
			
AD9361_RESET	PB0


SPI1			PA4		SPI1_CS
				PA5		SPI1_CLK
				PA6		SPI1_MISO
				PA7		SPI1_MOSI

串口助手使用说明：发送command后需要点击发送新行("\r\n")


版本V0.2：实现console_command功能
【1】：更改console_check_commands()，注释掉内部while((received_cmd[char_index] != '\n') && (received_cmd[char_index] != '\r'))，原因：存取usart_rx数据不获取该部分
		会导致死循环
		
【2】：更改ad9361_init()内存分配：phy = (struct ad9361_rf_phy *)zmalloc(sizeof(phy)); 原：phy = (struct ad9361_rf_phy *)zmalloc(sizeof(*phy)); 会导致内存分配失败

【3】：ST MCU禁止一切野指针操作，若AD9361初始化失败，那么所申请的内存空间也就会都释放掉，此时若使用USART_RX强制输出command，特殊功能command将会出现HardFault_Handler_Error

【4】：若AD9361初始化失败，想强制分配内存进行调试，可按下操作，即可规避HardFault_Handler_Error，注意：所有HardFault_Handler_Error，都是因为phy->xxx 没分配内存导致
int32_t ad9361_get_tx_sampling_freq (struct ad9361_rf_phy *phy,
									 uint32_t *sampling_freq_hz)
{
	phy->ref_clk_scale[TX_SAMPL_CLK] = (struct refclk_scale *)malloc(sizeof(phy->ref_clk_scale[TX_SAMPL_CLK]));
	printf("%s, line: %d \r\n",__FILE__,__LINE__); 
	*sampling_freq_hz = (uint32_t)clk_get_rate(phy,
										phy->ref_clk_scale[TX_SAMPL_CLK]);
	free(phy->ref_clk_scale[TX_SAMPL_CLK]);
	printf("%s, line: %d \r\n",__FILE__,__LINE__);
	return 0;
}

版本V0.1：初步实现SPI底层及USART_TX功能
【1】：uart_read_char：重构，待完成
【2】：strtok 替换 strsep
【3】：strtok 、strsep验证如下：
char s[] = "hello, world! welcome to china!";
	char delim[] = " ,!";
 
	char *token;
	for(token = strtok(s, delim); token != NULL; token = strtok(NULL, delim)) {
		printf(token);
		printf("+");
	}
	printf("\n");
--------------------- 


char source[] = "hello, world! welcome to china!";
	char delim[] = " ,!";
 
	char *s = strdup(source);
	char *token;
	for(token = strsep(&s, delim); token != NULL; token = strsep(&s, delim)) {
		printf(token);
		printf("+");
	}
	printf("\n");

--------------------- 

while ((line = strsep(&ptr, "\n"))) { ///返回"\n"前字符串 strtok
	while ((line = strtok(ptr, "\n"))) {
	