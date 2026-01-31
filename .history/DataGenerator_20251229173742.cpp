#include "DataGenerator.h"

#include <random>

std::mt19937_64 myrand( std::chrono::system_clock::now().time_since_epoch().count() );

int QuickPow( int x , int pow , int p ) {
    int res = 1;
    if ( p ) x %= p;
    while ( pow ) {
        if ( pow & 1 ) {
            if ( p ) res = ( res * x ) % p;
            else res = res * x;
        }
        if ( p ) x = ( x * x ) % p;
        else x = x * x;
        pow >>= 1;
    }
    if ( p ) return res % p;
    return res;
}

int exgcd( int a , int b , int &x , int &y ) {
    if ( !b ) {
        x = 1;
        y = 0;
        return a;
    }
    int d = exgcd( b , a % b , x , y );
    int tmp = x;
    x = y;
    y = tmp - a / b * y;
    return d;
}

string getRandomString( int n , string chars , bool allowZero ) {
    if ( n <= 0 || chars.empty() ) return "";

    const int len = chars.size();
    std::string result;
    result.reserve( n ); // 预分配

    // 生成首字符
    if ( !allowZero && chars.find( '0' ) != std::string::npos ) {
        std::string firstChars;
        for ( char c : chars ) if ( c != '0' ) firstChars += c;

        if ( firstChars.empty() ) {
            // 全是'0'，无法避免前导零
            result += '0';
        } else {
            result += firstChars[random( 0 , firstChars.size() - 1 )];
        }
    } else {
        result += chars[random( 0 , len - 1 )];
    }

    // 生成剩余字符
    for ( int i = 1 ; i < n ; ++i ) {
        result += chars[random( 0 , len - 1 )];
    }

    return result;
}

string getRandomString( vector < string > chars ) {
    int n = chars.size();
    return chars[random( 0 , n - 1 )];
}

Tree getRandomTree( int n ) {
    Tree tree;
    tree.numNodes = n;
    for ( int i = 2 ; i <= n ; ++i ) {
        // 根据随机数生成树的连接
        int parent = random( 1 , i - 1 ); // 随机选择一个父节点
        tree.edges.emplace_back( parent , i ); // 记录边 (parent, i)
    }
    return tree;
}

vector < int > getRandomTVector( int l , int r , int nums , int limit ) {
    vector < int > result( nums , l );
    assert( nums * l > limit ); // 至少要生成 nums * l 个随机数
    int steps = limit - ( nums * l );
    for ( int i = 0 ; i < nums ; ++i ) {
        if ( steps > 0 ) {
            if ( steps >= i ) {
                result[i] += i;
                steps -= i;
            } else {
                result[i] += steps;
                steps = 0;
            }
        }
    }
    shuffle( result.begin() , result.end() , std::mt19937( std::random_device()() ) );
    return result;
}

ifstream openInputFileStream( const string &str ) {
    return ifstream( str.c_str() , ios::in | ios::binary );
}

ofstream openOutputFileStream( const string &str ) {
    return ofstream( str.c_str() , ios::out | ios::binary );
}
