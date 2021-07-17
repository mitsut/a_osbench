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
 *  $Id: test_implement_lib.c 646 2016-05-06 05:07:47Z ertl-honda $
 */

/*
 *  テストライブラリ(実装依存部)
 */

#include "test_common_lib.h"
#include "test_target_lib.h"


/*
 *  テストライブラリ初期化(実装依存部)
 */
void test_init_impl_lib(void)
{
#ifndef TotalNumberOfCores
	/*
	 * OSが初期化時に行うの初期化処理
	 * (受信割込み許可や，ハードウェア使用に
	 *  対する初期化処理等)の処理をここで実施する
	 */
#endif /* TotalNumberOfCores */
	test_init_target_lib();
}


#ifdef TotalNumberOfCores
/*
 *  各コアで実行するテストライブラリ初期化
 */
void
test_init_impl_lib_eachcore(void)
{
	/*
	 * OSが初期化時に行うの初期化処理
	 * (受信割込み許可や，ハードウェア使用に
	 *  対する初期化処理等)の処理をコアごとに
	 *  行う処理が必要な場合はここで実施する
	 */

	test_init_target_lib_eachcore();
}


/*
 *  TaskIDが所属するCOREがアイドル状態か否か
 */
boolean
test_is_core_idle(uint32 TaskID)
{
	boolean	state = FALSE;

	/* 
	 * TaskIDの情報を取得してTaskIDの
	 * 所属しているCOREがアイドル状態の場合は
	 * stateにTRUEを設定し，アイドル状態以外の場合は
	 * stateにFALSEを戻り値にしてreturnする
	 */

	return (state)
}

#endif /* TotalNumberOfCores */


/*
 *  全割込み禁止状態取得API
 */
boolean
test_get_disallint_state(void)
{
	boolean	state = FALSE;
#ifdef TotalNumberOfCores
	/*
	 * 全割込み禁止状態の情報取得時
	 * マルチコア特有の処理や情報の取得
	 * が必要な場合はここに記載する
	 */
#endif /* TotalNumberOfCores */

	/*
	 * 全割込み禁止状態であるかの情報を取得して
	 * 全割込み禁止状態である場合はstate = TRUE
	 * を設定する
	 */

	return(state);
}


/*
 *  ログ出力(書式指定子0)
 */
void
test_outputlog_0(const char8* Format)
{
	/*
	 * OSで使用しているログ情報を出力する処理及び
	 * 関数を記載するFormatについては
	 * メッセージのフォーマット記述のポインタの
	 * ためログのメッセージを表示するにはFormatを
	 * 使用する
	 */
}


/*
 *  ログ出力(書式指定子1)
 */
void
test_outputlog_1(const char8* Format, sintptr Arg1)
{
	/*
	 * OSで使用しているログ情報を出力する処理及び
	 * 関数を記載するFormatについては
	 * メッセージのフォーマット記述のポインタの
	 * ためログのメッセージを表示するにはFormatを
	 * 使用する
	 * ログメッセージのフォーマット記述中で参照する値
	 * についてはArg1を使用する
	 */
}


/*
 *  ログ出力(書式指定子2)
 */
void
test_outputlog_2(const char8* Format, sintptr Arg1, sintptr Arg2)
{
	/*
	 * OSで使用しているログ情報を出力する処理及び
	 * 関数を記載するFormatについては
	 * メッセージのフォーマット記述のポインタの
	 * ためログのメッセージを表示するにはFormatを
	 * 使用する
	 * ログメッセージのフォーマット記述中で参照する値
	 * についてはArg1，Arg2を使用する
	 */
}


/*
 *  ログ出力(書式指定子3)
 */
void
test_outputlog_3(const char8* Format, sintptr Arg1, sintptr Arg2, sintptr Arg3)
{
	/*
	 * OSで使用しているログ情報を出力する処理及び
	 * 関数を記載するFormatについては
	 * メッセージのフォーマット記述のポインタの
	 * ためログのメッセージを表示するにはFormatを
	 * 使用する
	 * ログメッセージのフォーマット記述中で参照する値
	 * についてはArg1，Arg2，Arg3を使用する
	 */
}

