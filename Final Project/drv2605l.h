/*
 * drv2605l.h
 *
 *  Created on: Oct 14, 2019
 *      Author: Nick
 */


#ifndef DRV2605L_H_
#define DRV2605L_H_
#define DRV2605L    0x5A        // 7-bit address (8.5.3.1 note)
#define MODE        0x01
#define CONTROL3    0x1D
void config_drv2605L(void(*toggle_routine)());/*(*toggle_routine)()*/
#endif /* DRV2605L_H_ */
