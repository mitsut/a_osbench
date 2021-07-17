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
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�AUTOSAR��AUTomotive Open System ARchitecture�˻�
 *  �ͤ˴�Ť��Ƥ��롥�嵭�ε����ϡ�AUTOSAR����Ū�⻺������������Τ�
 *  �Ϥʤ���AUTOSAR�ϡ�AUTOSAR���ͤ˴�Ť������եȥ�����������Ū����
 *  �Ѥ���Ԥ��Ф��ơ�AUTOSAR�ѡ��ȥʡ��ˤʤ뤳�Ȥ���Ƥ��롥
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 *
 *  $Id: perf_timer.h 1658 2016-01-21 10:17:23Z fsi-asanoto $
 */

/*
 *  ��ǽɾ���ѥ����ޡ��ɥ饤��
 */
#ifndef PERF_TIMER_H
#define PERF_TIMER_H

#include "test_implement_lib.h"
#include "test_target_lib.h"
#include "target_hw_counter.h"

/*
 *  TAUB��Ϣ�쥸����
 */
#define TAUB_BASE(n)	((uint32) (0xffe30000U + (n * 0x1000U)))
#define TAUBTPS(n)		(TAUB_BASE(n) + 0x240U)
#define TAUBCDR(n, ch)	(TAUB_BASE(n) + (ch * 0x04U))
#define TAUBCNT(n, ch)	(TAUB_BASE(n) + 0x80U + (ch * 0x04U))
#define TAUBCMOR(n, ch)	(TAUB_BASE(n) + 0x2000U + (ch * 0x04U))
#define TAUBCMUR(n, ch)	(TAUB_BASE(n) + 0xC0U + (ch * 0x04U))
#define TAUBTS(n)		(TAUB_BASE(n) + 0x1C4U)
#define TAUBTT(n)		(TAUB_BASE(n) + 0x1C8U)

#define MCU_TAUB_MASK_CK0				((uint16) 0xfff0)
#define MCU_TAUB_CK0_0					((uint16) 0x0000) /* ʬ���ʤ� */
#define MCU_TAUB00_CMOR					((uint16) 0x0000)
#define MCU_TAUB00_CMUR					((uint8) 0x01)

#define TAUB_INTNO(n, ch)	(n == 0)? ((uint32) ((134 + ch))) : ((uint32) (248 + ch))

/*
 *  TAUBn �����ޤ�ư��ϡ���߽���
 */
LOCAL_INLINE void
PerfSetTimerStartTAUB(uint8 n, uint8 ch)
{
	/* �����޳��Ͻ��� */
	sil_wrh_mem((void *) TAUBTS(n), (1 << ch));
}

LOCAL_INLINE void
PerfSetTimerStopTAUB(uint8 n, uint8 ch)
{
	/* ��������߽��� */
	sil_wrh_mem((void *) TAUBTT(n), (1 << ch));
}

/*
 *  TAUBn�ϡ��ɥ����������󥿸��ߥƥ��å��ͼ���
 */
LOCAL_INLINE TickType
PerfGetCurrentTimeTAUB(uint8 n, uint8 ch, TickType maxval)
{
	TickType	count;
	TickType	curr_time = 0U;

	count = (TickType)sil_reh_mem((void *) (TAUBCNT(n, ch)));

	/* �����󥫥��󥿤ΰ١����ߥ��å��ͤ��Ѵ� */
	curr_time = maxval - count;
	curr_time = (curr_time % maxval);

	return(curr_time);
}


/* ********************** �ե꡼��󥿥��ޡ� ********************** */
/*
 *  �ե꡼��󥿥��ޡ��Υ١������ɥ쥹
 */
/*
 *  ���Ѥ��륿���ޡ��Υ�˥å��ֹ�Ⱥ�ʬ�����ޤȸ����ͥ����ޤΥ���ͥ�
 */
#define HWC_PTIM_UNIT  0  /* 0 or 1 */
#define HWC_PTIM_ID    2

/*
 *  �����ޥ���å���ñ�̤�[MHz]��
 */
#define PERF_TIMER_CLOCK  40

/*
 *	�����ޥ��������(����)
 */
#define TIM_CYC  ((uint16) 0xfffeU)


/*
 *  �����ޤθ����ͤ��ɽФ�
 */

/*
 * �������
 */
LOCAL_INLINE
void
perf_get_100ntime(uint32 *p_time)
{
	*p_time = PerfGetCurrentTimeTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID, TIM_CYC) * 10 / PERF_TIMER_CLOCK;
}


/* ********************** ���������ޡ��ʳ�����ѡ� ********************** */

/* �����ޥ���å���ñ�̤�[MHz]�� */
#define INT_TIMER_CLOCK  40

/*
 *	�����ޥ��������(1[ms])
 */
#define INT_TIM_CYC  ((uint32) (40000U))

/*
 * ���������ޡ��μ����ɽ������η����
 */
#define HWC_ITIM_UNIT  0  /* 0 or 1 */

#ifndef USE_TP_ISR
#define INT_A_TIMER   3
#define INT_B_TIMER   4
#else /* USE_TP_ISR */
#define INT_A_NW_TIMER    3
#define INT_A_W_TIMER     4
#define INT_B_NW_TIMER    5
#define INT_B_W_TIMER     6
#endif /* USE_TP_ISR */

/*
 * �������
 */
LOCAL_INLINE void
int_get_100ntime(uint32 *p_time, uint32 tim_type)
{
	*p_time = PerfGetCurrentTimeTAUB(HWC_ITIM_UNIT, tim_type, INT_TIM_CYC) * 10 / PERF_TIMER_CLOCK;
}

/*
 * �����ƥ�����������०����ɥ�ɾ���ѥ�����
 */
#ifdef USE_TP_KERNEL

#include "target_tp_timer.h"
#include "target_hw_counter.h"

#define HWC_ITIM2_UNIT  0  /* 0 or 1 */
#define HWC_ITIM2_ID    5
#define INTNO_ACTISR2_E1 TAUB_INTNO(HWC_ITIM2_UNIT, HWC_ITIM2_ID)


/*
 * �������
 */
LOCAL_INLINE void
int_get_100ntime2(uint32 *p_time)
{
	*p_time = PerfGetCurrentTimeTAUB(HWC_ITIM2_UNIT, HWC_ITIM2_ID, 4000) * 10 / PERF_TIMER_CLOCK;

}

/*
 * �������ͤμ���(�����०����ɥ���������)
 */
LOCAL_INLINE void
twd_get_100ntime(uint32 *p_time)
{
    /* �����०����ɥ���0.001sec */
    /* �����०����ɥ������ޤ�8MHz */
	*p_time = GetCurrentTimeTAUJ(0, 2, 8000) * 10 / 8;
}

/*
* �������ͤμ���(�����ƥ����������)
 */
LOCAL_INLINE void
syscyc_get_100ntime(uint32 *p_time)
{
	/* �����ƥ������0.01sec */
    /* �����ƥ���������ޤ�8MHz */
	*p_time = GetCurrentTimeTAUJ(0, 3, 80000) * 10 / 8;
}

#endif /* USE_TP_KERNEL*/

#endif /* PERF_TIMER_H */
