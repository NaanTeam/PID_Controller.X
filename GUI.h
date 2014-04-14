/* 
 * File:   GUI.h
 * Author: Connor
 *
 * Created on April 14, 2014, 3:21 PM
 */

#ifndef GUI_H
#define	GUI_H

#ifdef	__cplusplus
extern "C" {
#endif

extern float GUI_ROLL,         //From the GUI
             GUI_PITCH,
             GUI_YAW,
             GUI_THROTTLE;

void GUI_adjustRoll(float offset);
void GUI_adjustPitch(float offset);
void GUI_adjustYaw(float offset);
void GUI_adjustThrottle(float offset);

#ifdef	__cplusplus
}
#endif

#endif	/* GUI_H */

