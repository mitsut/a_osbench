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
 *  $Id: perf_timer.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

/*
 *  評価用タイマードライバ
 */

#include "perf_timer.h"


/* ********************** フリーランタイマー ********************** */

/*
 *  タイマの起動処理
 */
void
perf_timer_initialize(void)
{
	/* ストップ */
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_CONTROL), PERF_TIMER_CONTROL_STOP);

	/* リセット */
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_STATUS), 0x00U);
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_PERIODL), (TIM_CYC & 0xffffU));       /* カウンターセット 下位16bit */
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_PERIODH), ((uint32) TIM_CYC >> 16));  /* カウンターセット 上位16bit */

	/* スタート */
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_CONTROL), PERF_TIMER_CONTROL_START | PERF_TIMER_CONTROL_CONT);
}


/*
 *  タイマの停止処理
 */
void
perf_timer_terminate(void)
{
	/* ストップ */
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_CONTROL), PERF_TIMER_CONTROL_STOP);

	/* 割込み要求のクリア */
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_STATUS), 0x00U);
}


/* ********************** 周期タイマー（割込み用） ********************** */

/*
 *  周期割込みタイマーのベースアドレスを管理するテーブル
 */
const uint32
int_timer_base_table[] = {
  INT_A_TIMER_BASE,
  INT_B_TIMER_BASE,
#ifdef TotalNumberOfCores
  INT_C_TIMER_BASE,
#endif  /* TotalNumberOfCores */
};


/*
 *  タイマの起動処理
 */
void
int_timer_initialize(uint32 tim_type)
{
	volatile uint32 int_timer_base;
  
	/* タイマーのベースアドレスを引数から設定 */
	int_timer_base = int_timer_base_table[tim_type];

	/* ストップ */
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_CONTROL), INT_TIMER_CONTROL_STOP);

	/* リセット */
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_STATUS), 0x00U);
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_PERIODL), (INT_TIM_CYC & 0xffffU));       /* カウンターセット 下位16bit */
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_PERIODH), ((uint32) INT_TIM_CYC >> 16));  /* カウンターセット 上位16bit */

	/* スタート */
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_CONTROL), INT_TIMER_CONTROL_START | INT_TIMER_CONTROL_CONT | INT_TIMER_CONTROL_ITO);
}


/*
 *  タイマの停止処理
 */
void
int_timer_terminate(uint32 tim_type)
{
	volatile uint32 int_timer_base;

	/* タイマーのベースアドレスを引数から設定 */
	int_timer_base = int_timer_base_table[tim_type];

	/* ストップ */
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_CONTROL), INT_TIMER_CONTROL_STOP);

	/* 割込み要求のクリア */
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_STATUS), 0x00U);
}


