/******************************************************************
1995 - 2026


                          < o < o n u t


                                                    қuran Wolfgang
******************************************************************/
#ifndef MBXI_IF_H
#define MBXI_IF_H

#include "bpci_if.h"

typedef UCHAR MAILBOX;
typedef UCHAR MAIL;

#define NULL_MBX ((MAILBOX)ZERO)

#define MBXI_Create() mMBXI_Create()

void MBXI_Init(void);
MAILBOX mMBXI_Create(void);
void MBXI_Connect(MAILBOX mailbox, TASK task);
void MBXI_Disconnect(MAILBOX mailbox, TASK task);
BOOL MBXI_Send(MAILBOX mailbox, MAIL value);
UCHAR MBXI_Count(MAILBOX mailbox);
BOOL MBXI_Receive(MAILBOX mailbox, MAIL *value);

#endif
