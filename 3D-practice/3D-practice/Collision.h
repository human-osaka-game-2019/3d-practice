#ifndef COLLISION_H_
#define COLLISION_H_

#include "Directx.h"

/**
 * @brief 　ベクトルの大きさ計算する
 * @param 　vector　ベクトル
 * @return　ベクトルの大きさ
 */
FLOAT AbsVector(D3DXVECTOR3 vector);

/**
 * @brief  sphereの当たり判定の結果をbool返す
 * @param  thing1 当たり判定したい３ｄオブジェクト
 * @param  thing2 当たり判定したい３ｄオブジェクト
 * @return 当たっていたらtrue,当たってなかったらfalse
 */
bool isCollision(THING thing1, THING thing2);

#endif

