
#include "DataGenerator.h"

// 随机字符串
const string charNumber = "0123456789";
const string charLowerCase = "abcdefghijklmnopqrstuvwxyz";
const string charUpperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const vector < string > customChars = { "AC" , "WA" , "TLE" , "MLE" , "PE" , "RE" };
using vvi = vector < vector < int > >;
using vvI = vector < vector < signed > >;
using vi = vector < int >;
using vI = vector < signed >;
using vs = vector < string >;
using vpii = vector < pair < int , int > >;
using vpII = vector < pair < signed , signed > >;
using vb = vector < bool >;
using vvb = vector < vb >;
using vc = vector < char >;
int __ = 1;

vector < string > INPUT_DATA;
vector < string > OUTPUT_DATA;

double OFFSET;
stringstream ss;
stringstream ins;

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
            OFFSET = 2.0 / ( endIndex - startIndex + 1 ) * index;
            init();
            int _ = 1;
            //            _ = random( 1 , 1e2 , OFFSET );
            //            writeInputFile << _ << "\n";
            //            cin >> _;
            // 当前执行次数
            __ = _;
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
