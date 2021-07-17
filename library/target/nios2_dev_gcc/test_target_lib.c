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
 *  $Id: test_target_lib.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

/*
 *  テストライブラリ(依存部)
 */

#include "test_target_lib.h"
#include "target_hw_counter.h"


/* 自コア以外の割込み番号を無視するマクロ(マルチコア用) */
#ifdef TotalNumberOfCores
#define TEST_IGNORE_INTERRUPT(intno)			\
	if ((GetCoreID() + 1) != (intno >> 16U)) {	\
		return;									\
	}
#endif /* TotalNumberOfCores */


/*
 *  テストライブラリ初期化(ターゲット依存部)
 */
void
test_init_target_lib(void)
{
}

/*
 *  各コアで実行するテストライブラリ初期化(ターゲット依存部)
 */
void
test_init_target_lib_eachcore(void)
{
}


/*
 *  割込み処理
 */
void
test_int_raise(uint32 intno)
{
#ifdef TotalNumberOfCores
	TEST_IGNORE_INTERRUPT(intno);
#endif /* TotalNumberOfCores */

	/* タイマ停止 */
	sil_wrw_iop((void *) TIMER_CONTROL(intno), TIMER_CONTROL_STOP);

	/* タイマの割込み要求のクリア */
	sil_wrw_iop((void *) TIMER_STATUS(intno), 0x00);

	/* タイマカウンタに0x01をセットすることで，すぐ満了 */
	sil_wrw_iop((void *) TIMER_PERIODH(intno), 0x00);
	sil_wrw_iop((void *) TIMER_PERIODL(intno), 0x01);

	/* タイマ開始 */
	sil_wrw_iop((void *) TIMER_CONTROL(intno), TIMER_CONTROL_ITO | TIMER_CONTROL_START);
}

/*
 *  割込み要求のクリア
 */
void
test_clear_int_req(uint32 intno)
{
#ifdef TotalNumberOfCores
	TEST_IGNORE_INTERRUPT(intno);
#endif /* TotalNumberOfCores */

	/* タイマの割込み要求のクリア */
	sil_wrw_iop((void *) TIMER_STATUS(intno), 0x00);
}


/*
 *  CPU例外発生処理
 */
void
raise_cpu_exception(void)
{
	Asm("div zero, zero, zero");
}
