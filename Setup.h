/* 
 * File:   setup.h
 * Author: Connor
 *
 * Created on March 13, 2014, 10:18 PM
 */

#ifndef SETUP_H
#define	SETUP_H
    
#include "hardwareProfile.h"
#include "RcRx.h"

void Setup_initializeBoard(void);
void Setup_ports(void);
void Setup_timers(void);
void Setup_configInterrupts(void);
void Setup_communications(void);
void Setup_sensorComm(void);

#endif	/* SETUP_H */
