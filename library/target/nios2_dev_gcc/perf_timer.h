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
 *  $Id: perf_timer.h 646 2016-05-06 05:07:47Z ertl-honda $
 */

/*
 *  性能評価用タイマードライバ
 */
#ifndef PERF_TIMER_H
#define PERF_TIMER_H

#include "test_implement_lib.h"


/* ********************** フリーランタイマー ********************** */
/*
 *  フリーランタイマーのベースアドレス
 */
/* シングルコアの場合 */
#ifndef TotalNumberOfCores
#define PERF_TIMER_BASE  0x080003e0 /* タイマベースアドレス */

/* マルチコアの場合 */
#else
#define PERF_TIMER_BASE  0x0c0003e0 /* タイマベースアドレス */
#endif  /* TotalNumberOfCores */

/*
 *  レジスタ
 */
#define PERF_TIMER_STATUS   0x00
#define PERF_TIMER_CONTROL  0x04
#define PERF_TIMER_PERIODL  0x08
#define PERF_TIMER_PERIODH  0x0C
#define PERF_TIMER_SNAPL    0x10
#define PERF_TIMER_SNAPH    0x14

/*
 *  ビット定義
 */
#define PERF_TIMER_STATUS_RUN  0x02
#define PERF_TIMER_STATUS_TO   0x01

#define PERF_TIMER_CONTROL_STOP   0x08
#define PERF_TIMER_CONTROL_START  0x04
#define PERF_TIMER_CONTROL_CONT   0x02
#define PERF_TIMER_CONTROL_ITO    0x01

/*
 *  タイマクロック（単位は[MHz]）
 */
#define PERF_TIMER_CLOCK  60

/*
 *	タイマカウント値(最大)
 */
#define TIM_CYC  ((uint32) 0xfffffffeU)


/*
 *  タイマの現在値の読出し
 */
LOCAL_INLINE uint32
perf_timer_get_current(void)
{
    uint32 count;

	/* スナップレジスタに書き込むと値をホールドする */
	sil_wrw_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_SNAPL), 0x00U);

	/* カウント値を読み込む */
	count = sil_rew_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_SNAPL));
	count |= sil_rew_iop((void *) (PERF_TIMER_BASE + PERF_TIMER_SNAPH)) << 16;

    return(TIM_CYC - count);
}

/*
 * 時刻取得
 */
LOCAL_INLINE
void
perf_get_100ntime(uint32 *p_time)
{
	*p_time = perf_timer_get_current() * 10 / PERF_TIMER_CLOCK;
}


/* ********************** 周期タイマー（割込み用） ********************** */
/*
 *  周期タイマーのベースアドレス
 */
/* シングルコアの場合 */
#ifndef TotalNumberOfCores
#define INT_A_TIMER_BASE  0x080003c0
#define INT_B_TIMER_BASE  0x080003a0

/* マルチコアの場合 */
#else   /* TotalNumberOfCores */
#define INT_A_TIMER_BASE  0x0c0003c0
#define INT_B_TIMER_BASE  0x0c0003a0
#define INT_C_TIMER_BASE  0x0c0003e0
#endif  /* TotalNumberOfCores */

/*
 *  レジスタ
 */
#define INT_TIMER_STATUS   0x00
#define INT_TIMER_CONTROL  0x04
#define INT_TIMER_PERIODL  0x08
#define INT_TIMER_PERIODH  0x0C
#define INT_TIMER_SNAPL    0x10
#define INT_TIMER_SNAPH    0x14

/*
 *  ビット定義
 */
#define INT_TIMER_STATUS_RUN  0x02
#define INT_TIMER_STATUS_TO   0x01

#define INT_TIMER_CONTROL_STOP   0x08
#define INT_TIMER_CONTROL_START  0x04
#define INT_TIMER_CONTROL_CONT   0x02
#define INT_TIMER_CONTROL_ITO    0x01

/* タイマクロック（単位は[MHz]） */
#define INT_TIMER_CLOCK  60

/*
 *	タイマカウント値(1[ms])
 */
#define INT_TIM_CYC  ((uint32) (60000U))

/*
 * 周期タイマーの種類を表すための型定義
 */

#define INT_A_TIMER   0
#define INT_B_TIMER   1
#ifdef TotalNumberOfCores
#define INT_C_TIMER   2
#endif  /* TotalNumberOfCores */

/* ベースアドレステーブル */
extern const uint32 int_timer_base_table[];

/*
 *  タイマの現在値の読出し
 */
LOCAL_INLINE uint32
int_timer_get_current(uint32 tim_type)
{
    uint32 count;
	volatile uint32 int_timer_base;

	/* タイマーのベースアドレスを引数から設定 */
	int_timer_base = int_timer_base_table[tim_type];

	/* スナップレジスタに書き込むと値をホールドする */
	sil_wrw_iop((void *) (int_timer_base + INT_TIMER_SNAPL), 0x00U);

	/* カウント値を読み込む */
	count = sil_rew_iop((void *) (int_timer_base + INT_TIMER_SNAPL));
	count |= sil_rew_iop((void *) (int_timer_base + INT_TIMER_SNAPH)) << 16;

    return(INT_TIM_CYC - count);
}

/*
 * 時刻取得
 */
LOCAL_INLINE void
int_get_100ntime(uint32 *p_time, uint32 tim_type)
{
	*p_time = int_timer_get_current(tim_type) * 10 / INT_TIMER_CLOCK;
}


#endif /* PERF_TIMER_H */
