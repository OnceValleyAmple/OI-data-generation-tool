#include "DataGenerator.h"
using namespace std ;
#define int long long

// 输入数据存储字符串流（类比 cin）
stringstream ins;
// 输出数据存储字符串流（类比 cout）
stringstream ous;

/**
 * @brief 数据生成逻辑代码
 * @param DATAL 数据范围下限
 * @param DATAR 数据范围上限
 * @param OFFSET 影响因子
 */
void solve( int DATAL , int DATAR , double OFFSET ) {
    return;
}

/**
 * @brief 初始化函数
 */
void init() {
    return;
}

signed main() {
    ios::sync_with_stdio( false );
    cin.tie( nullptr );
    init();
    if ( batchGenerateFiles( 1 , 20 , solve , 1e5 , ins , ous ) ) {
        cout << "-------数据文件生成成功-------\n";
    } else {
        cerr << "-------数据文件生成失败-------\n";
    }
    return 0;
}
