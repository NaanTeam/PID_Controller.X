/* 
 * File:   setup.h
 * Author: Connor
 *
 * Created on March 13, 2014, 10:18 PM
 */

#ifndef SETUP_H
#define	SETUP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "hardwareProfile.h"
#include "RC_Receiver.h"


void Setup_initializeBoard(void);
void Setup_setupPorts(void);
void Setup_setupTimers(void);
void Setup_configInterrupts(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SETUP_H */

