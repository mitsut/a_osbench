/*
 *  TOPPERS/A-OSBENCH
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Operating System Benchmark
 *
 *  Copyright (C) 2015-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by FUJI SOFT INCORPORATED, JAPAN
 *
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�AUTOSAR��AUTomotive Open System ARchitecture�˻�
 *  �ͤ˴�Ť��Ƥ��롥�嵭�ε����ϡ�AUTOSAR����Ū�⻺������������Τ�
 *  �Ϥʤ���AUTOSAR�ϡ�AUTOSAR���ͤ˴�Ť������եȥ�����������Ū����
 *  �Ѥ���Ԥ��Ф��ơ�AUTOSAR�ѡ��ȥʡ��ˤʤ뤳�Ȥ���Ƥ��롥
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 *
 *  $Id: test_target_lib.h 1658 2016-01-21 10:17:23Z fsi-asanoto $
 */

#ifndef TEST_TARGET_LIB_H
#define TEST_TARGET_LIB_H

#include "test_implement_lib.h"
#include "test_trap_include.h"


/* ������ֹ�(perf_sc3) */
#define INTNO_ACTISR2_1    76
#define INTNO_ACTISR2_2    77
#ifndef USE_TP_ISR
#ifndef USE_SYSCYC
#define INTNO_RESOURCE     78
#define INTNO_SET_EVENT_1  79
#define INTNO_SET_EVENT_2  80
#define INTNO_TERAPP_F1    81
#define INTNO_TERAPP_F2    82
#define INTNO_TERAPP_F3    83
#define INTNO_TERAPP_F4    84
#define INTNO_PROHOOK_A    85
#define INTNO_INT_A_TIMER  137
#define INTNO_INT_B_TIMER  138
#else /* USE_SYSCYC */
#define INTNO_SYSCYC_ISR2_A      81+8
#define INTNO_SYSCYC_ISR2_B      82+8
#define INTNO_SYSCYC_ISR2_C      83+8
#define INTNO_SYSCYC_ISR2_D      83+8
#define INTNO_SYSCYC_ISR2_E      84+8
#define INTNO_SYSCYC_ISR2_F      85+8
#define INTNO_SYSCYC_ISR2_G      137+8
#define INTNO_SYSCYC_ISR2_H      138+8
#endif /* USE_SYSCYC */
#else /* USE_TP_ISR */
#define INTNO_INT_A_NW_TIMER     137+8
#define INTNO_INT_A_W_TIMER      138+8
#define INTNO_INT_B_NW_TIMER     139+8
#define INTNO_INT_B_W_TIMER      140+8
#endif /* USE_TP_ISR */


/* �����ͥ���� */
#define INTPRI_TIMER_NUM   4


/* �����ɥ륿��������� */
#if defined(TotalNumberOfCores) && defined(SLAVE_IDLE_TASK)
#define IDLE_LOOP_NUM 9
extern volatile boolean IdleTaskFlag[TotalNumberOfCores];

LOCAL_INLINE void
idle_task_process(uint8 core_num)
{
	volatile uint32 i;
	while(1){
		IdleTaskFlag[core_num] = TRUE;
		for(i = 0; i < IDLE_LOOP_NUM; i++){
			;
		}
	}
}
#endif /* TotalNumberOfCores && SLAVE_IDLE_TASK */


/* trap�б� */
#ifdef USE_SCALABILITYCLASS3
#ifndef TotalNumberOfCores
#define TFN_TEST_CHECK_POINT	 64
#else   /* TotalNumberOfCores */
#define TFN_INVALIDATE_ICACHE	 75
#define TFN_TEST_MC_CHECK_POINT	 76
#define TFN_TEST_BARRIER_SYNC	 77
#endif	/* TotalNumberOfCores */


#ifndef TotalNumberOfCores
LOCAL_INLINE void
_trap_test_check_point(uint32 count)
{
	CAL_SVC_1N(void, TFN_TEST_CHECK_POINT, uint32, count);
}
#else   /* TotalNumberOfCores */
LOCAL_INLINE void
_trap_test_mc_check_point(uint8 id, uint32 count)
{
	CAL_SVC_2N(void, TFN_TEST_MC_CHECK_POINT, uint8, id, uint32, count);
}

LOCAL_INLINE void
_trap_test_barrier_sync(uint32 phase, uint16 core_num)
{
	CAL_SVC_2N(void, TFN_TEST_BARRIER_SYNC, uint32, phase, uint16, core_num);
}
#endif	/* TotalNumberOfCores */

LOCAL_INLINE void
_trap_invalidate_icache(void)
{
#if 0
	CAL_SVC_0N(void, TFN_INVALIDATE_ICACHE);
#endif
}
#endif /* USE_SCALABILITYCLASS3 */


#endif  /* TEST_TARGET_LIB_H */
