
Ӳ���ӿڣ�

USART1			PA9		USART1_TX
				PA10	USART1_RX
			
AD9361_RESET	PB0


SPI1			PA4		SPI1_CS
				PA5		SPI1_CLK
				PA6		SPI1_MISO
				PA7		SPI1_MOSI

��������ʹ��˵��������command����Ҫ�����������("\r\n")


�汾V0.2��ʵ��console_command����
��1��������console_check_commands()��ע�͵��ڲ�while((received_cmd[char_index] != '\n') && (received_cmd[char_index] != '\r'))��ԭ�򣺴�ȡusart_rx���ݲ���ȡ�ò���
		�ᵼ����ѭ��
		
��2��������ad9361_init()�ڴ���䣺phy = (struct ad9361_rf_phy *)zmalloc(sizeof(phy)); ԭ��phy = (struct ad9361_rf_phy *)zmalloc(sizeof(*phy)); �ᵼ���ڴ����ʧ��

��3����ST MCU��ֹһ��Ұָ���������AD9361��ʼ��ʧ�ܣ���ô��������ڴ�ռ�Ҳ�ͻᶼ�ͷŵ�����ʱ��ʹ��USART_RXǿ�����command�����⹦��command�������HardFault_Handler_Error

��4������AD9361��ʼ��ʧ�ܣ���ǿ�Ʒ����ڴ���е��ԣ��ɰ��²��������ɹ��HardFault_Handler_Error��ע�⣺����HardFault_Handler_Error��������Ϊphy->xxx û�����ڴ浼��
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

�汾V0.1������ʵ��SPI�ײ㼰USART_TX����
��1����uart_read_char���ع��������
��2����strtok �滻 strsep
��3����strtok ��strsep��֤���£�
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

while ((line = strsep(&ptr, "\n"))) { ///����"\n"ǰ�ַ��� strtok
	while ((line = strtok(ptr, "\n"))) {
	