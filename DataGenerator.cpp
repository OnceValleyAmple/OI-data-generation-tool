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

inline int getRandomNumber( int l , int r ) {
    return myrand() % ( r - l + 1 ) + l;
}

int getRandomNumber( int l1 , int r1 , int l2 , int r2 ) {
    if ( l1 > r1 ) {
        swap( l1 , r1 );
    }
    if ( l2 > r2 ) {
        swap( l2 , r2 );
    }
    int l = getRandomNumber( l1 , r1 );
    int r = getRandomNumber( l2 , r2 );
    if ( l > r ) {
        swap( l , r );
    }
    return getRandomNumber( l , r );
}

double getRandomDouble( double l , double r ) {
    random_device rd;
    mt19937 gen( rd() );
    uniform_real_distribution < double > dis( l , r );
    return dis( gen );
}

int getRandomLimit( int limit , int step ) {
    return pow( limit , max( 0.1 , log10( step ) ) );
}

vector < int > getRandomNumbers( int l , int r , int nums , int limit ) {
    std::vector < int > result( nums , l );
    int steps = limit - ( nums * l );
    if ( steps < 0 ) {
        throw "Not enough steps to generate random numbers";
    }
    int sum = nums * l;
    for ( int i = 0 ; i < nums ; ++i ) {
        int d = getRandomNumber( 0 , max( 1ll , steps / nums ) );
        if ( result[i] + d <= r ) {
            result[i] += d;
            steps -= d;
            sum += d;
        }
    }
    if ( sum > limit ) {
        throw "Too many random numbers generated";
    }
    return result;
}

string getRandomString( int n , string chars , bool allowZero) {
    int len = chars.size();
    string res = "";
    for ( int i = 0 ; i < n ; ++i ) {
        char c = chars[getRandomNumber( 0 , len - 1 , 0 , len - 1 )];
        if ( i == 0 && c == '0' && !allowZero ) {
            while ( c == '0' ) {
                c = chars[getRandomNumber( 0 , len - 1 , 0 , len - 1 )];
            }
        }
        res += c;
    }
    return res;
}

string getRandomString( vector < string > chars ) {
    int n = chars.size();
    return chars[getRandomNumber( 0 , n - 1 , 0 , n - 1 )];
}

Tree getRandomTree( int n ) {
    Tree tree;
    tree.numNodes = n;
    for ( int i = 2 ; i <= n ; ++i ) { // 根据随机数生成树的连接
        int parent = getRandomNumber( 1 , i - 1 ); // 随机选择一个父节点
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
