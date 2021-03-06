
/******************************************************************************
*  File           :  ctl.c
*  Version        :  1
*  Last Changes   :  2019-05-16 15:57:59
******************************************************************************/

/******************************************************************************
*  Include Files
******************************************************************************/

#include "typ.h"
#include "ctl.h"
#include "led.h"
#include <string.h>

/******************************************************************************
*  Local Constants
******************************************************************************/

/******************************************************************************
*  Local Type Definitions
******************************************************************************/

/******************************************************************************
*  Local Variables
******************************************************************************/

/******************************************************************************
*  Global Variables
******************************************************************************/

TUINT8 CTL_Temp = 0;

TUINT8 CTL_Channel_R[CTL_CHANNEL_SIZE];
TUINT8 CTL_Channel_G[CTL_CHANNEL_SIZE];
TUINT8 CTL_Channel_B[CTL_CHANNEL_SIZE];

/******************************************************************************
*  Local Function Prototypes
******************************************************************************/

/******************************************************************************
*  Local Functions
******************************************************************************/

/******************************************************************************
*  Global Functions
******************************************************************************/

/******************************************************************************
*  Function Name  :  
*  Description    :  
*  Parameter(s)   :  
*  Return Value   :  
******************************************************************************/

void CTL_Ini (void)
{

}

/******************************************************************************
*  Function Name  :
*  Description    :
*  Parameter(s)   :
*  Return Value   :
******************************************************************************/

void CTL_Main(void)
{


	/*
	Layer3_SET;
	Layer2_SET;
	Layer1_SET;
	Layer0_SET;
	SM16126_DAT_A_SET;
	SM16126_DAT_B_SET;
	SM16126_DAT_C_SET;
	SM16126_CLK_SET;
	SM16126_STK_SET;
	SM16126_OE_SET;





	Layer3_RESET;
	Layer2_RESET;
	Layer1_RESET;
	Layer0_RESET;
	SM16126_DAT_A_RESET;
	SM16126_DAT_B_RESET;
	SM16126_DAT_C_RESET;
	SM16126_CLK_RESET;
	SM16126_STK_RESET;
	SM16126_OE_RESET;

	*/


	//SM16126_DAT_C_SET;

	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, ENABLE);




	//HAL_Delay(200);
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, DISABLE);

	//SM16126_DAT_C_RESET;
	//HAL_Delay(100);


}

/******************************************************************************
*  Changes        :  
*                    
*                    
*                    
******************************************************************************/

/******************************************************************************
*  END OF FILE    :  ctl.h
******************************************************************************/
