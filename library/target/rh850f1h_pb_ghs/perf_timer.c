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
 *  $Id: perf_timer.c 1658 2016-01-21 10:17:23Z fsi-asanoto $
 */

/*
 *  ɾ���ѥ����ޡ��ɥ饤��
 */

#include "perf_timer.h"


/* ********************** �ե꡼��󥿥��ޡ� ********************** */

/*
 *  �����ޤε�ư����
 */
void
perf_timer_initialize(void)
{
	uint16	wk;

	/* �����ͥ����ޤΥץꥹ����������� PCLK/2^0 */
	wk = sil_reh_mem((void *) TAUBTPS(HWC_PTIM_UNIT));
	wk &= MCU_TAUB_MASK_CK0;
	wk |= MCU_TAUB_CK0_0;
	sil_wrh_mem((void *) TAUBTPS(HWC_PTIM_UNIT), wk);

	/* �����ͥ�������߽��� */
	PerfSetTimerStopTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID);
	/* ����߶ػ� */
	HwcounterDisableInterrupt(TAUB_INTNO(HWC_PTIM_UNIT, HWC_PTIM_ID));
	/* ������׵᥯�ꥢ */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_PTIM_UNIT, HWC_PTIM_ID));

	/* �����ͥ����ޤ򥤥󥿡��Х륿���ޤȤ������� */
	sil_wrh_mem((void *) TAUBCMOR(HWC_PTIM_UNIT, HWC_PTIM_ID), MCU_TAUB00_CMOR);
	sil_wrb_mem((void *) TAUBCMUR(HWC_PTIM_UNIT, HWC_PTIM_ID), MCU_TAUB00_CMUR);

	/* �����ޥ�����ȼ������� */
	sil_wrh_mem((void *) TAUBCDR(HWC_PTIM_UNIT, HWC_PTIM_ID), 0xfffe);

	/* �����ͥ����޳��� */
	PerfSetTimerStartTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID);
}


/*
 *  �����ޤ���߽���
 */
void
perf_timer_terminate(void)
{
	/* �����ͥ����ޤ���� */
	PerfSetTimerStopTAUB(HWC_PTIM_UNIT, HWC_PTIM_ID);

	/* �����ͥ����ޤγ�����׵�Υ��ꥢ */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_PTIM_UNIT, HWC_PTIM_ID));
}


/* ********************** ���������ޡ��ʳ�����ѡ� ********************** */

/*
 *  �����ޤε�ư����
 */
void
int_timer_initialize(uint32 tim_type)
{
	uint16	wk;

	/* �����ͥ�������߽��� */
	PerfSetTimerStopTAUB(HWC_ITIM_UNIT, tim_type);
	/* ����߶ػ� */
	HwcounterDisableInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
	/* ������׵᥯�ꥢ */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
	/* ��ʬ�����ޤγ���ߵ��� */
	HwcounterEnableInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));

	/* �����ͥ����ޤ򥤥󥿡��Х륿���ޤȤ������� */
	sil_wrh_mem((void *) TAUBCMOR(HWC_ITIM_UNIT, tim_type), MCU_TAUB00_CMOR);
	sil_wrb_mem((void *) TAUBCMUR(HWC_ITIM_UNIT, tim_type), MCU_TAUB00_CMUR);

	/* �����ޥ�����ȼ������� */
	sil_wrh_mem((void *) TAUBCDR(HWC_ITIM_UNIT, tim_type), INT_TIM_CYC);

	/* �����ͥ����޳��� */
	PerfSetTimerStartTAUB(HWC_ITIM_UNIT, tim_type);
}


/*
 *  �����ޤ���߽���
 */
void
int_timer_terminate(uint32 tim_type)
{
	/* �����ͥ����ޤ���� */
	PerfSetTimerStopTAUB(HWC_ITIM_UNIT, tim_type);

	/* �����ͥ����ޤγ���߶ػ� */
	HwcounterDisableInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
	/* �����ͥ����ޤγ�����׵�Υ��ꥢ */
	HwcounterClearInterrupt(TAUB_INTNO(HWC_ITIM_UNIT, tim_type));
}


