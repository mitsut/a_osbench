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
 *  タイマドライバ
 */
#ifndef PERF_TIMER_H
#define PERF_TIMER_H


/* ********************** フリーランタイマ ********************** */
/*
 *  フリーランタイマのベースアドレスの定義
 */
#ifndef TotalNumberOfCores
/* シングルコアの場合 */
#else
/* マルチコアの場合 */
#endif  /* TotalNumberOfCores */

/*
 *  タイマのレジスタの定義
 */


/*
 *  タイマ関連ビット定義
 */


/*
 *  タイマクロック（単位は[MHz]）定義
 */

/*
 *	タイマカウント値の定義
 */

/*
 *  タイマの現在値の読出し
 */
LOCAL_INLINE uint32
perf_timer_get_current(void)
{
    uint32 count;

	/*
	 * フリーランタイマタイマのカウント値を読み込む
	 * 処理をここに実装して取得したカウント値をcount
	 * へ代入して本関数の戻り値に設定する
	 */

    return(count);
}

/*
 * 時刻取得
 */
LOCAL_INLINE void
perf_get_100ntime(uint32 *p_time)
{
	/*
	 * *p_timeにperf_timer_get_currentで取得したタイマ値
	 * をタイマクロックを考慮して時刻変化した値を設定する
	 */
}


/* ********************** 周期タイマ（割込み用） ********************** */
/*
 *  周期タイマのベースアドレス
 */
/* シングルコアの場合 */
#ifndef TotalNumberOfCores
#define PERF_TIMER_BASE

/* マルチコアの場合 */
#else   /* TotalNumberOfCores */
#define PERF_TIMER_BASE
#endif  /* TotalNumberOfCores */

/*
 *  周期タイマレジスタの定義
 */


/*
 *  周期タイマ関連ビット定義
 */

/*
 * タイマクロック（単位は[MHz]）定義
 */


/*
 * 周期タイマの種類を表すための型定義
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
	volatile uint32 int_timer_base;

	/* タイマのベースアドレスを引数から設定 */
	int_timer_base = int_timer_base_table[tim_type];

	/*
	 * int_timer_baseの周期タイマのカウント値を読み込む
	 * 処理をここに実装して取得したカウント値をcount
	 * へ代入して本関数の戻り値に設定する
	 */

    return(count);
}

/*
 * 時刻取得
 */
LOCAL_INLINE void
int_get_100ntime(uint32 *p_time, uint32 tim_type)
{
	/* 
	 * *p_timeにperf_timer_get_currentで取得したタイマ値
	 * をタイマクロックを考慮して時刻変化した値を設定する
	 */
}

#endif /* PERF_TIMER_H */
