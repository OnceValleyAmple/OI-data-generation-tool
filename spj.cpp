#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
#define PC 99  // 部分正确
#define AC 100 // 全部正确
#define PE 101 // 格式错误
#define WA 102 // 答案错误
#define ERROR 103 // 系统错误
const int N = 1e6 + 7;
const int INF = 0x7fffffff;

void closeFile( FILE * f ) {
    if ( f != nullptr ) {
        fclose( f );
    }
}

FILE * f_in = nullptr;
FILE * f_out = nullptr;
FILE * f_user = nullptr;

ifstream FIn;
ifstream FOut;
ifstream FUser;

void initFiles( int argc , char * args[] ) {
    // 初始化全局的 FIn、FUser 和 FOut
    FIn.open( args[1] , ios::in | ios::binary );
    if ( !FIn.is_open() ) {
        cerr << "无法打开输入文件: " << args[1] << endl;
        exit( ERROR );
    }

    FUser.open( args[2] , ios::in | ios::binary );
    if ( !FUser.is_open() ) {
        cerr << "无法打开用户文件: " << args[2] << endl;
        exit( ERROR );
    }

    FOut.open( args[3] , ios::in | ios::binary );
    if ( !FOut.is_open() ) {
        cerr << "无法打开输出文件: " << args[3] << endl;
        exit( ERROR );
    }
}

void closeFiles() {
    if ( FIn.is_open() ) FIn.close();
    if ( FOut.is_open() ) FOut.close();
    if ( FUser.is_open() ) FUser.close();
    closeFile( f_in );
    closeFile( f_out );
    closeFile( f_user );
}

int solve() {
    return AC;
}

int main( int argc , char * args[] ) {
    // args[1] = "D:\\Desktop\\Data\\20.in";
    // args[2] = "D:\\Desktop\\Data\\20.out";
    // args[3] = "D:\\Desktop\\Data\\20.out";

    initFiles( argc , args );
    /*****spj*******/
    int result = solve();
    /*****spj-end********/
    closeFiles();
    // cout << result << endl;
    return result;
}
