/*
 *  TOPPERS/A-OSBENCH
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Operating System Benchmark
 *
 *  Copyright (C) 2015-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by FUJI SOFT INCORPORATED, JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 *  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 *  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 *  用する者に対して，AUTOSARパートナーになることを求めている．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id: test_target_lib.h 1658 2016-01-21 10:17:23Z fsi-asanoto $
 */

#ifndef TEST_TARGET_LIB_H
#define TEST_TARGET_LIB_H

#include "test_implement_lib.h"
#include "test_trap_include.h"


/* 割込み番号(perf_sc3) */
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
#define INTNO_SYSCYC_ISR2_A      80
#define INTNO_SYSCYC_ISR2_B      81
#define INTNO_SYSCYC_ISR2_C      82
#define INTNO_SYSCYC_ISR2_D      83
#define INTNO_SYSCYC_ISR2_E      84
#define INTNO_SYSCYC_ISR2_F      85
#define INTNO_SYSCYC_ISR2_G      137
#define INTNO_SYSCYC_ISR2_H      138
#endif /* USE_SYSCYC */
#else /* USE_TP_ISR */
#define INTNO_INT_A_NW_TIMER     137
#define INTNO_INT_A_W_TIMER      138
#define INTNO_INT_B_NW_TIMER     139
#define INTNO_INT_B_W_TIMER      140
#endif /* USE_TP_ISR */


/* 割込み優先度 */
#define INTPRI_TIMER_NUM   4


/* アイドルタスク内処理 */
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


/* trap対応 */
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
