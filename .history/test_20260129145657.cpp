
#include "DataGenerator.h"

vector < string > INPUT_DATA;
vector < string > OUTPUT_DATA;
stringstream ss;
stringstream ins;
// 数据规模
const int DATA_RANGE = 1e5;
GradientScaleGenerator gen( DATA_RANGE );
// 当前数据范围
int DATAL , DATAR;

void solve() {
    return;
}

void init() {
    return;
}

signed main() {
    ios::sync_with_stdio( false );
    cin.tie( nullptr ), cout.tie( nullptr );
    // 起始
    int startIndex = 1;
    // 截止
    int endIndex = 20;
    for ( int index = startIndex ; index <= endIndex ; ++index ) {
        ofstream writeInputFile = openOutputFileStream( dataFilePath + to_string( index ) + inFileType );
        if ( writeInputFile.is_open() ) {
            cout << "成功打开文件 " << index << "\n";
            init();
            int _ = 1;
            //            cin >> _;
            auto [L , R] = gen.getGradientRange( index );
            DATAL = L;
            DATAR = R;
            while ( _ -- ) {
                solve();
                INPUT_DATA.emplace_back( ins.str() );
                ins.str( "" );
                OUTPUT_DATA.emplace_back( ss.str() );
                ss.str( "" );
            }
            // -------- 输入 Start --------
            for ( const auto &item : INPUT_DATA ) {
                writeInputFile << item;
            }
            INPUT_DATA.clear();
            // -------- 输入 End --------
            writeInputFile.close();
            cout << "数据写入成功 " << index << "\n";
        } else {
            cerr << "写入文件失败 " << index << "\n";
        }
        ofstream writeOutputFile = openOutputFileStream( dataFilePath + to_string( index ) + outFileType );
        if ( writeOutputFile.is_open() ) {
            cout << "成功打开文件 " << index << "\n";
            // -------- 输出 Start --------
            for ( const auto &item : OUTPUT_DATA ) {
                writeOutputFile << item;
            }
            OUTPUT_DATA.clear();
            // -------- 输出 End --------
            writeOutputFile.close();
            cout << "数据写入成功 " << index << "\n";
        } else {
            cerr << "写入文件失败 " << index << "\n";
        }
    }
    return 0;
}
