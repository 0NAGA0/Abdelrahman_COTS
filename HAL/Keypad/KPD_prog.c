#include "STD_TYPES.h"
#include "ErrType.h"

#include "DIO_interface.h"

#include "KPD_interface.h"
#include "KPD_cfg.h"
#include "KPD_prv.h"



uint8 KDF_u8GetPressedKey(void)
{
	uint8 local_u8ColIdx, local_u8RowIdx, Local_u8PinState, Local_u8PressedKey = KPD_u8No_PRESSED_KEY_VAL;

	uint8 Local_au8ColArr[COL_NUM] = {KPD_u8COL0_PIN,KPD_u8COL1_PIN,KPD_u8COL2_PIN,KPD_u8COL3_PIN};
	uint8 Local_au8RowArr[ROW_NUM] = {KPD_u8ROW0_PIN,KPD_u8ROW1_PIN,KPD_u8ROW2_PIN,KPD_u8ROW3_PIN};

	uint8 Local_auKPDARR[ROW_NUM][COL_NUM] = KPD_au8_BUTTON_ARR;

	/*Activate the column pins*/
	for(local_u8ColIdx=0u; local_u8ColIdx < COL_NUM; local_u8ColIdx++)
	{
		/*Activate the current column*/
		DIO_u8SetPinValue(KPD_u8COL_PORT, Local_au8ColArr[local_u8ColIdx], DIO_u8PIN_LOW);

		/*Read the row pins*/
		for(local_u8RowIdx = 0u; local_u8RowIdx < ROW_NUM; local_u8RowIdx++)
		{
			/*Read the current row*/
			DIO_u8ReadPinValue(KPD_u8ROW_PORT, Local_au8RowArr[local_u8RowIdx], &Local_u8PinState);

			if(Local_u8PinState == DIO_u8PIN_LOW)
			{
				Local_u8PressedKey = Local_auKPDARR[local_u8RowIdx][local_u8ColIdx];

				/*Polling with blocking(waiting) until the key is released*/
				while(Local_u8PinState == DIO_u8PIN_LOW)
				{
					DIO_u8ReadPinValue(KPD_u8ROW_PORT, Local_au8RowArr[local_u8RowIdx], &Local_u8PinState);
				}

				return Local_u8PressedKey;
			}
		}

		/*Deactivate the current column*/
		DIO_u8SetPinValue(KPD_u8COL_PORT, Local_au8ColArr[local_u8ColIdx], DIO_u8PIN_HIGH);
	}

	return Local_u8PressedKey;
}
