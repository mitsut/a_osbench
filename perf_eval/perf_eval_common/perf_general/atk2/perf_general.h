/*
 *  TOPPERS/A-OSBENCH
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Operating System Benchmark
 *
 *  Copyright (C) 2014 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2014 by FUJISOFT INCORPORATED, JAPAN
 *  Copyright (C) 2014 by Sunny Giken Inc., JAPAN
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  $Id: perf_general.h 652 2016-05-06 17:45:35Z ertl-honda $
 */

#ifndef PERF_GENERAL_H
#define PERF_GENERAL_H

#include "Os.h"
#include "perf_histogram.h"
#include "test_common_lib.h"
#include "test_target_lib.h"


#define TASK_STACK_SIZE 512
#define ISR_STACK_SIZE 256


/* 計測上限単位時間 */
#define MAX_TIME        300U
#define MAX_TIME_100US  2000U

/* 10s, 100us test */
#ifdef USE_MC_KERNEL
#define NO_LOOP1 510
#else  /* USE_MC_KERNEL */
#define NO_LOOP1 525
#endif /* USE_MC_KERNEL */


/* Event */
#define EVENT_T2_MASK       0x01U
#define SETEVENT_T2_MASK    0x01U
#define EVENT_T3_MASK       0x01U
#define NT_EVENT_T2_MASK    0x01U
#define NT_SETEVENT_T2_MASK 0x01U
#define NT_EVENT_T3_MASK    0x01U
#define EVENT_MC_T3_MASK    0x01U
#define NT_EVENT_MC_T3_MASK 0x01U


typedef enum int_category_type {
	NONE, 
	INT_1_1,
	INT_1_2,
	INT_2,
	INT_3_1,
	INT_3_2,
	INT_4_1,
	INT_4_2,
} INT_CATEGORY;

/* シーケンスチェックを有効にするマクロ */
/* #define VALIDATE_SEQUENCE */
#ifdef VALIDATE_SEQUENCE
#define SEQUENCE_CHECK_START() (test_init_check_cnt())
#ifndef USE_MC_KERNEL
#define SEQUENCE_CHECK(arg1, arg2) (test_check_point(arg2))
#else  /* USE_MC_KERNEL */
#define SEQUENCE_CHECK(arg1, arg2) (test_mc_check_point(arg1, arg2))
#endif /* USE_MC_KERNEL */
#else   /* VALIDATE_SEQUENCE */
#define SEQUENCE_CHECK_START()
#define SEQUENCE_CHECK(arg1, arg2)
#endif  /* VALIDATE_SEQUENCE */

/* 計測回数 */
#ifdef VALIDATE_SEQUENCE
#define LOOP_COUNT 2U
#else  /* VALIDATE_SEQUENCE */
#define LOOP_COUNT 10000U
#endif /* VALIDATE_SEQUENCE */

/* MC */
/* OS_CORE_ID_SLAVE の設定 */
#ifdef USE_MC_KERNEL
#if OS_CORE_ID_MASTER == OS_CORE_ID_0
#define OS_CORE_ID_SLAVE OS_CORE_ID_1
#else  /* OS_CORE_ID_MASTER  */
#define OS_CORE_ID_SLAVE OS_CORE_ID_0
#endif /* OS_CORE_ID_MASTER */
#endif /* USE_MC_KERNEL */

extern uint16 histarea[7][MAX_TIME + 1];
extern uint16 histarea100us[MAX_TIME_100US + 1];
extern volatile uint32 overhead_time;
extern uint32 cnt_int;
extern volatile boolean int_measure_end;
extern volatile INT_CATEGORY int_category;


#endif /* PERF_GENERAL_H */
