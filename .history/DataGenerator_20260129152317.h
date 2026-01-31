/**
    @Author Ample
    @Time 2024/7/11 15:05
    @Description: OI/ACM 题目数据生成头文件
*/

#ifndef CLION_DATAGENERATOR_H
#define CLION_DATAGENERATOR_H

#include <bits/stdc++.h>
/**
 * 这是由 Fesdrer 独立编写的随机数据类型生成器。
 *
 * 能够方便进行信息学奥林匹克竞赛（OI）中的测试数据生成及对拍工作。
 *
 * @param random 这是最基本的随机整数生成器
 *
 * @param RandomBasic 基本随机函数，包含随机打乱数组、随机生成数组、随机生成字符串
 *
 * @param RandomPrint 输出函数，方便地输出数组、图、树
 *
 * @param RandomAnother 其他随机函数，包括返回不同地若干数、分离若干数
 *
 * @param RandomGraphTool 打乱节点编号、加自环、去重边、生成不连通图等图操作
 *
 * @param RandomTreeGraphHackspfaDagSccEdccVdcc 随机生成各类图
 *
 * @param CheckTreeSccEdccVdcc 检查树深度、度数、极大连通分量
 *
 * @author Fesdrer
 *
 * 接下来介绍 AMPLE_RAND 核心部分的各种函数（除了 random() 和 randomreal() 其他的是一个类，用 . 调用其中包含的函数）：

    random() 这是最基本的随机整数生成器

    randomreal() 这是最基本的随机实数生成器

    RandomBasic 基本随机函数，包含随机打乱数组、随机生成数组、随机生成字符串

    RandomPrint 输出函数，方便地输出数组、图、树

    RandomAnother 其他随机函数，包括返回不同地若干数、分离若干数

    RandomGraphTool 打乱节点编号、加自环、去重边、生成不连通图等图操作

    RandomTree，RandomGraph，RandomHackSpfa，RandomDag，RandomScc，RandomEdcc，RandomVdcc 随机生成各类图，除了 RandomTree 和 RandomHackSpfa 外都包含 3 ? 个函数：check，nm 和一个以图名称命名的函数，分别实现检查点数边数等是否合法、随机生成一组合法点数边数、生成图的功能。
 */

namespace AMPLE_RAND {
    static std::mt19937_64 rng_64( std::chrono::steady_clock::now().time_since_epoch().count() );

    /**
    * 在 [l,r] 中随机生成一个整数。
    * opt 在 [0,+infinity]，其越大，生成的整数越可能接近 r。
    * @param opt ∈ (0, +∞)
    *            opt > 1 → 偏向 r
    *            opt < 1 → 偏向 l  
    *            opt = 1 → 均匀分布
    */
    inline long long random(long long l, long long r, double opt = 1.0) {
    // 参数校验
    if (l > r) std::swap(l, r);
    if (l == r) return l;
    
    // opt 必须为正
    if (opt <= 0.0) {
        throw std::invalid_argument("opt must be positive");
    }
    
    // opt ≈ 1 时走均匀分布快速路径
    if (std::fabs(opt - 1.0) <= 1e-8) {
        return std::uniform_int_distribution<long long>(l, r)(rng_64);
    }
    
    // 避免 static 带来的线程安全问题
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double x = dist(rng_64);
    
    // 处理 x = 0 的边界（虽然罕见，但 0^正数 = 0，无问题）
    // x ∈ [0, 1)，y = x^(1/opt) ∈ [0, 1)
    double y = std::pow(x, 1.0 / opt);
    
    // 使用 unsigned 避免大区间溢出
    unsigned long long range = static_cast<unsigned long long>(r) - 
                               static_cast<unsigned long long>(l);
    
    // 核心映射：y * range ∈ [0, range)，结果 ∈ [l, r]
    long long res = l + static_cast<long long>(
        static_cast<long double>(range) * static_cast<long double>(y)
    );
    
    // 双边界保护
    res = std::clamp(res, l, r);
    
    return res;
}

    /*
    在 [l,r] 中随机生成一个实数。
    opt 在 [0,+infinity]，其越大，生成的整数越可能接近 r。
    */
   inline double randomreal(double l, double r, double opt = 1.0) {
    if (l > r) std::swap(l, r);
    if (l == r) return l;
    if (opt <= 0.0) throw std::invalid_argument("opt must be positive");
    
    thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
    double x = dist(rng_64);
    
    // 简单幂变换：只需一次 pow
    // opt > 1 → 偏向 r
    // opt < 1 → 偏向 l
    double y = std::pow(x, 1.0 / opt);
    
    return l + (r - l) * y;
}

    //基本随机函数
    class RANDOMBASIC {
        public:
            //随机打乱数组
            template < typename T > inline void random_shuffle( T first , T last ) {
                if ( first == last ) return;
                for ( T i = first + 1 ; i != last ; i ++ ) {
                    T j = first + random( 0 , i - first );
                    if ( i != j ) iter_swap( i , j );
                }
            }

            //随机生成一个数组。
            template < typename T > inline std::vector < T > random_sequence( int n , T l , T r ) {
                std::vector < T > ret;
                ret.clear();
                while ( n -- ) ret.push_back( random( l , r ) );
                return ret;
            }

            template < typename T , typename V > inline void random_sequence( T first , T last , V l , V r ) {
                for ( T i = first ; i != last ; i ++ ) ( *i ) = random( l , r );
            }

            //随机返回一个字符串，字符集是 charset
            inline std::string random_string( int n , const char * charset = "abcdefghijklmnopqrstuvwxyz" ) {
                std::string ret = "";
                int leng = strlen( charset );
                for ( int i = 0 ; i < n ; i ++ ) ret += charset[random( 0 , leng - 1 )];
                return ret;
            }

            inline void random_string( char * first , char * last ,
                                       const char * charset = "abcdefghijklmnopqrstuvwxyz" ) {
                int leng = strlen( charset );
                for ( char * i = first ; i != last ; i ++ ) ( *i ) = charset[random( 0 , leng - 1 )];
            }
    };

    static RANDOMBASIC RandomBasic;

    //输出辅助函数。
    class RANDOMPRINT {
        public:
            //输出数组，输出一行。
            template < typename T > inline void print_sequence( T first , T second ) {
                for ( T it = first ; it != second ; it ++ ) std::cout << ( *it ) << " ";
                std::cout << '\n';
            }

            //输出数组，输出一行。
            template < typename T > inline void print_sequence( std::vector < T > s ) {
                for ( T i : s ) std::cout << i << " ";
                std::cout << '\n';
            }

            //输出由二元组组成的数组，每个二元组一行。
            template < typename T > inline void print_sequence_pair( T first , T second ) {
                for ( T it = first ; it != second ; it ++ ) std::cout << ( *it ).first << " " << ( *it ).second << '\n';
            }

            //输出由二元组组成的数组，每个二元组一行。
            template < typename T > inline void print_sequence_pair( std::vector < T > s ) {
                for ( T i : s ) std::cout << i.first << " " << i.second << '\n';
            }

            //输出二维数组，每个维度的数组一行。
            template < typename T > inline void print_sequence_of_sequence( std::vector < std::vector < T > > s ) {
                for ( std::vector < T > i : s ) {
                    for ( T j : i ) std::cout << j << " ";
                    std::cout << '\n';
                }
            }

            /*
            输出图中的每条边 (x,y)，每行输出一条边，格式为 "x y"。
            支持随机生成每条边的权值，支持输出给定的权值。
            */
            inline void print_graph( std::vector < std::pair < int , int > > graph ) {
                for ( std::pair < int , int > i : graph ) std::cout << i.first << " " << i.second << '\n';
            }

            /*
            输出图中的每条边 (x,y)，每行输出一条边，格式为 "x y"。
            支持随机生成每条边的权值，支持输出给定的权值。
            */
            inline void print_graph( std::vector < std::pair < int , int > > graph , long long l , long long r ) {
                for ( std::pair < int , int > i :
                      graph )
                    std::cout << i.first << " " << i.second << " " << random( l , r ) << '\n';
            }

            /*
            输出图中的每条边 (x,y)，每行输出一条边，格式为 "x y"。
            支持随机生成每条边的权值，支持输出给定的权值。
            */
            inline void print_graph( std::vector < std::pair < std::pair < int , int > , int > > graph ) {
                for ( std::pair < std::pair < int , int > , int > i :
                      graph )
                    std::cout << i.first.first << " " << i.first.second << " " << i.second << '\n';
            }

            //输出树上第 2 到 n 号节点的父亲（根节点为 1）
            inline void print_tree_father( std::vector < std::pair < int , int > > tree ) {
                int n = int( tree.size() ) + 1;
                std::vector < int > fa( n , 0 );
                std::vector < std::vector < int > > e( n , std::vector < int >( 0 ) );
                std::queue < int > q;
                for ( std::pair < int , int > &edges : tree ) {
                    e[edges.first - 1].push_back( edges.second );
                    e[edges.second - 1].push_back( edges.first );
                }
                while ( q.size() ) q.pop();
                q.push( 1 );
                while ( q.size() ) {
                    int x = q.front();
                    q.pop();
                    for ( int y : e[x - 1] )
                        if ( y != fa[x - 1] ) {
                            fa[y - 1] = x;
                            q.push( y );
                        }
                }
                for ( int i = 1 ; i < n ; i ++ ) std::cout << fa[i] << " ";
                std::cout << '\n';
            }
    };

    static RANDOMPRINT RandomPrint;

    //其余随机函数。
    class RANDOMANOTHER {
        public:
            //返回 [l,r] 中除了 S 中元素外的 p 个不同的元素。不调用 S 代表 S 为空。
            template < typename T > inline std::vector < T > random_different( T l , T r , int p ) {
                assert( r - l + 1 >= p );
                if ( p == 0 ) return std::vector < T >( 0 );
                std::vector < T > now( 0 );
                if ( ( r - l + 1 ) / p <= 10 ) {
                    for ( T i = l ; i <= r ; i ++ ) now.push_back( i );
                    RandomBasic.random_shuffle( now.begin() , now.end() );
                    int nowsiz = int( now.size() );
                    while ( nowsiz > p ) now.pop_back(), nowsiz --;
                    return now;
                }
                std::unordered_set < T > S;
                S.clear();
                while ( p -- ) {
                    T x = random( l , r );
                    while ( S.find( x ) != S.end() ) x = random( l , r );
                    now.push_back( x ), S.insert( x );
                }
                return now;
            }

            //返回 [l,r] 中除了 S 中元素外的 p 个不同的元素。不调用 S 代表 S 为空。
            template < typename T > inline std::vector < T > random_different(
                T l , T r , int p , std::unordered_set < T > S ) {
                assert( r - l + 1 >= p + S.size() );
                if ( p == 0 ) return std::vector < T >( 0 );
                std::vector < T > now( 0 );
                if ( ( r - l + 1 ) / p <= 10 ) {
                    for ( T i = l ; i <= r ; i ++ ) if ( S.find( i ) == S.end() ) now.push_back( i );
                    RandomBasic.random_shuffle( now.begin() , now.end() );
                    int nowsiz = int( now.size() );
                    while ( nowsiz > p ) now.pop_back(), nowsiz --;
                    return now;
                }
                while ( p -- ) {
                    T x = random( l , r );
                    while ( S.find( x ) != S.end() ) x = random( l , r );
                    now.push_back( x ), S.insert( x );
                }
                return now;
            }

            //将 [l,r] 中的数分成 p 份。
            inline std::vector < std::vector < int > > random_divide( int l , int r , int p ) {
                if ( l > r && p == 0 ) return std::vector < std::vector < int > >( 0 );
                assert( p >= 1 && p <= r - l + 1 );
                std::vector < int > idnum( 0 );
                std::vector < std::vector < int > > spread( p );
                for ( int i = l ; i <= r ; i ++ ) idnum.push_back( i );
                RandomBasic.random_shuffle( idnum.begin() , idnum.end() );
                for ( int i = 0 ; i < p ; i ++ ) spread[i].assign( 1 , idnum[i] );
                int top = 0;
                for ( int i = p ; i < r - l + 1 ; i ++ ) {
                    int id = ( top == p ? random( 0 , p - 1 ) : random( 0 , top ) );
                    if ( id == top ) top ++;
                    spread[id].push_back( idnum[i] );
                }
                return spread;
            }

            //将 [l,r] 中的数分成 p 份，并通过 limits 限制每份数量上限。
            inline std::vector < std::vector < int > > random_divide( int l , int r , int p ,
                                                                      std::vector < int > limits ) {
                if ( l > r && p == 0 ) return std::vector < std::vector < int > >( 0 );
                long long sum = 0;
                for ( int i : limits ) sum += i;
                assert( p >= 1 && p <= r - l + 1 && sum >= r - l + 1 && int( limits.size() ) == p );
                std::vector < int > id( p ) , idnum( 0 );
                for ( int i = l ; i <= r ; i ++ ) idnum.push_back( i );
                RandomBasic.random_shuffle( idnum.begin() , idnum.end() );
                for ( int i = 0 ; i < p ; i ++ ) id[i] = i;
                sort( id.begin() , id.end() , [ & ]( const int x , const int y ) { return limits[x] > limits[y]; } );
                std::vector < std::vector < int > > spread( p );
                FESDRER_STL::FVector choose;
                int top = 0;
                for ( int i = 0 ; i < p ; i ++ ) spread[i].assign( 1 , idnum[i] );
                for ( int i = p ; i < r - l + 1 ; i ++ ) {
                    int ids = ( top == p ? random( 0 , choose.size() - 1 ) : random( 0 , choose.size() ) );
                    if ( ids == choose.size() ) choose.push_back( top ++ );
                    spread[choose[ids]].push_back( idnum[i] );
                    if ( limits[id[choose[ids]]] == int( spread[choose[ids]].size() ) ) choose.erase( ids );
                }
                std::vector < std::vector < int > > ret( p );
                for ( int i = 0 ; i < p ; i ++ ) ret[id[i]] = spread[i];
                return ret;
            }

            //将 n 个数分成 p 份，并通过 limits 限制每份数量上限（limits 为空（即保持默认）表示无上限）。
            inline std::vector < int > random_divide_num( int n , int p ,
                                                          std::vector < int > limits = std::vector < int >( 0 ) ) {
                if ( n == 0 && p == 0 ) return std::vector < int >( 0 );
                assert( p >= 1 && p <= n );
                if ( limits.empty() ) for ( int i = 1 ; i <= p ; i ++ ) limits.push_back( n );
                else {
                    long long sum = 0;
                    for ( int i : limits ) sum += i;
                    assert( int(limits.size())==p );
                }
                std::vector < int > id( p );
                for ( int i = 0 ; i < p ; i ++ ) id[i] = i;
                sort( id.begin() , id.end() , [ & ]( const int x , const int y ) { return limits[x] > limits[y]; } );
                std::vector < int > spread( p );
                FESDRER_STL::FVector choose;
                int top = 0;
                for ( int i = 0 ; i < p ; i ++ ) spread[i] = 1;
                for ( int i = p ; i < n ; i ++ ) {
                    int ids = ( top == p ? random( 0 , choose.size() - 1 ) : random( 0 , choose.size() ) );
                    if ( ids == choose.size() ) choose.push_back( top ++ );
                    spread[choose[ids]] ++;
                    if ( limits[id[choose[ids]]] == int( spread[choose[ids]] ) ) choose.erase( ids );
                }
                std::vector < int > ret( p );
                for ( int i = 0 ; i < p ; i ++ ) ret[id[i]] = spread[i];
                return spread;
            }
    };

    static RANDOMANOTHER RandomAnother;

    //图操作辅助函数。
    class RANDOMGRAPHTOOL {
        public:
            //打乱 n 个节点的图的节点编号，不改变 1 号节点和 n 号节点。
            inline void random_vertices( int n , std::vector < std::pair < int , int > > &edges ) {
                std::vector < int > idnum;
                idnum.clear();
                for ( int i = 2 ; i < n ; i ++ ) idnum.push_back( i );
                RandomBasic.random_shuffle( idnum.begin() , idnum.end() );
                idnum.insert( idnum.begin() , 1 ), idnum.push_back( n );
                for ( std::pair < int , int > &i : edges ) i.first = idnum[i.first - 1], i.second = idnum[i.second - 1];
            }

            //打乱 n 个节点的图的节点编号，不改变 1 号节点和 n 号节点。
            inline void random_vertices( int n , std::vector < std::pair < std::pair < int , int > , int > > &edges ) {
                std::vector < int > idnum;
                idnum.clear();
                for ( int i = 2 ; i < n ; i ++ ) idnum.push_back( i );
                RandomBasic.random_shuffle( idnum.begin() , idnum.end() );
                idnum.insert( idnum.begin() , 1 ), idnum.push_back( n );
                for ( std::pair < std::pair < int , int > , int > &i :
                      edges )
                    i.first.first = idnum[i.first.first - 1], i.first.second = idnum[i.first.second - 1];
            }

            //返回 n 个节点的图中每个点的出边到达的点。directional 表示是否有向。下标从 0 开始。
            inline std::vector < std::vector < int > > get_outedge(
                int n , std::vector < std::pair < int , int > > edges , bool directional ) {
                std::vector < std::vector < int > > e( n , std::vector < int >( 0 ) );
                for ( std::pair < int , int > &it : edges ) {
                    e[it.first - 1].push_back( it.second );
                    if ( !directional ) e[it.second - 1].push_back( it.first );
                }
                return e;
            }

            //清空 n 个节点的图中的重边。directional 表示是否有向。
            inline void clear_repeated_edges( int n , std::vector < std::pair < int , int > > &graph ,
                                              bool directional ) {
                std::unordered_set < long long > S;
                S.clear();
                auto Gid = [ & ]( int x , int y ) {
                    if ( !directional && x > y ) std::swap( x , y );
                    return 1ll * ( x - 1 ) * n + y;
                };
                int m = int( graph.size() ) , _ = 0;
                for ( int i = 0 ; i < m ; i ++ ) {
                    if ( S.find( Gid( graph[i].first , graph[i].second ) ) == S.end() ) {
                        if ( _ == i ) _ ++;
                        else graph[_ ++] = graph[i];
                        S.insert( Gid( graph[i].first , graph[i].second ) );
                    }
                }
                while ( m > _ ) graph.pop_back(), m --;
            }

            //在 n 个节点的图中随即加入 addnum 个自环（不保证没有重复的自环）。
            inline void add_self_rings( int n , std::vector < std::pair < int , int > > &graph , int addnum ) {
                while ( addnum -- ) {
                    int x = random( 1 , n );
                    graph.push_back( { x , x } );
                }
            }

            /*
            随机生成一个不连通图。
            图的点数边数上限是 N,M 并且会尽量接近这一上限。
            Rnm(int n,int m)->vector<int> 是参数生成函数，随机生成点数不超过 n、边数不超过 m 的合法参数并以 vector<int> 的形式原样返回，如果 n,m 不合法则返回的 vector 的值全为 -1（详见下面的生成图相关库）。
            Rgraph(vector<int>)->vector<pair<int,int>> 是图生成函数，接受 Rnm 返回的参数并随机生成一个图。
            opt 是连通块数量，但最终连通块数量会比 opt 大。程序只能保证生成的连通块数量会在统计上随着 opt 增长而增长。
            */
            template < typename __Rnm , typename __Rgraph > inline std::vector < std::pair < int , int > >
            random_unconnected( int N , int M , __Rnm Rnm , __Rgraph Rgraph , double opt = 5 ) {
                std::vector < std::pair < int , int > > graph( 0 );
                int cnt = 0;
                opt = 1 / opt;
                while ( true ) {
                    std::vector < int > number = Rnm( N * opt , M * opt );
                    if ( number[0] == -1 ) break;
                    std::vector < std::pair < int , int > > now = Rgraph( number );
                    for ( std::pair < int , int > it : now ) graph.push_back( { it.first + cnt , it.second + cnt } );
                    N -= number[0], M -= number[1], cnt += number[0];
                }
                return graph;
            }
    };

    static RANDOMGRAPHTOOL RandomGraphTool;

    //随机树生成函数。
    class RANDOMTREE {
        public:
            /*
            随机生成一个 n 个点的以 1 为根的树。
            rho 在 [0,+infinity]，其越大，树的深度越深，点的度数越小。
            可以通过 vertives_rand 控制是否打乱节点编号，打乱后根节点仍然是 1，不打乱则父亲编号永远小于儿子。
            */
            inline std::vector < std::pair < int , int > > random_tree( int n , double rho = 2 ,
                                                                        bool vertices_rand = 1 ) {
                std::vector < std::pair < int , int > > tree;
                tree.clear();
                for ( int i = 2 ; i <= n ; i ++ ) tree.push_back( { random( 1 , i - 1 , rho ) , i } );
                if ( vertices_rand ) RandomGraphTool.random_vertices( n , tree );
                return tree;
            }

            //随机生成一个菊花，根节点为 1。
            inline std::vector < std::pair < int , int > > random_flower_tree( int n ) {
                std::vector < std::pair < int , int > > tree;
                tree.clear();
                int root = 1;
                for ( int i = 1 ; i <= n ; i ++ ) if ( i != root ) tree.push_back( { i , root } );
                return tree;
            }

            //随机生成一条链，不保证节点 1 为链的一端。
            inline std::vector < std::pair < int , int > > random_chain_tree( int n ) {
                std::vector < std::pair < int , int > > tree;
                tree.clear();
                for ( int i = 1 ; i < n ; i ++ ) tree.push_back( { i , i + 1 } );
                RandomGraphTool.random_vertices( n , tree );
                return tree;
            }

            /*
            随机生成一个灯笼（或者说蒲公英），即链的一段为菊花的树，根节点为 1。
            opt 在 [0,100]，为链约占总点数的百分比。
            */
            inline std::vector < std::pair < int , int > > random_lantern_tree( int n , int opt = 50 ) {
                std::vector < std::pair < int , int > > tree;
                tree.clear();
                int root = 1 , teil = root;
                for ( int i = 1 ; i <= n ; i ++ )
                    if ( i != root ) {
                        int test = random( 1 , 100 );
                        if ( test > opt ) tree.push_back( { i , root } );
                        else tree.push_back( { i , teil } ), teil = i;
                    }
                RandomGraphTool.random_vertices( n , tree );
                return tree;
            }

            /*
            随机生成一颗二叉树。
            opt 在 [0,100]，为右儿子约占总儿子的百分比。
            rho 在 [0,+infinity]，其越大，树的深度越大。
            可以通过 vertives_rand 控制是否打乱节点编号，打乱后根节点仍然是 1，不打乱则父亲编号永远小于儿子。
            */
            inline std::vector < std::pair < int , int > > random_binary_tree(
                int n , int opt = 50 , double rho = 1 , bool vertices_rand = 1 ) {
                std::vector < int > lson , rson;
                lson.clear(), rson.clear();
                std::vector < std::pair < int , int > > tree;
                tree.clear();
                lson.push_back( 1 ), rson.push_back( 1 );
                for ( int i = 2 ; i <= n ; i ++ ) {
                    int test = random( 1 , 100 ) , fa;
                    if ( test > opt ) {
                        int id = random( 0 , int( lson.size() ) - 1 , rho );
                        fa = lson[id], lson.erase( lson.begin() + id );
                    } else {
                        int id = random( 0 , int( rson.size() ) - 1 , rho );
                        fa = rson[id], rson.erase( rson.begin() + id );
                    }
                    tree.push_back( { fa , i } ), lson.push_back( i ), rson.push_back( i );
                }
                if ( vertices_rand ) RandomGraphTool.random_vertices( n , tree );
                return tree;
            }

            /**
             * 生成随机森林
             * @param n 节点数 (编号 0 ~ n-1)
             * @param m 边数 (要求 0 <= m <= n-1)
             * @param index 根节点（默认为 1）
             * @return 生成 n 个点 m 条边的森林 边集 vector<pair<int,int>>
             */
            inline std::vector < std::pair < int , int > > random_forest( int n , int m , unsigned index = 1 ) {
                // 边界处理
                if ( n <= 0 || m <= 0 ) return {};
                m = std::min( m , n - 1 ); // 森林最多 n-1 条边

                std::mt19937 rng( std::chrono::steady_clock::now().time_since_epoch().count() );

                std::vector < std::pair < int , int > > edges;
                edges.reserve( m );

                // nodes[i]: 以 i 为根的连通分量包含的所有节点
                std::vector < std::vector < int > > nodes( n );
                for ( int i = 0 ; i < n ; i ++ ) {
                    nodes[i].push_back( i );
                }

                // 当前活跃的连通分量的根节点列表
                std::vector < int > roots( n );
                std::iota( roots.begin() , roots.end() , 0 );

                while ( ( int ) edges.size() < m ) {
                    // 随机选择两个不同的连通分量
                    int i = rng() % roots.size();
                    int j = rng() % ( roots.size() - 1 );
                    if ( j >= i ) j ++;

                    int r1 = roots[i] , r2 = roots[j];

                    // 从两个分量中各随机选取一个点作为边的端点
                    int u = nodes[r1][rng() % nodes[r1].size()];
                    int v = nodes[r2][rng() % nodes[r2].size()];
                    edges.emplace_back( u , v );

                    // 启发式合并：小分量并入大分量
                    if ( nodes[r1].size() < nodes[r2].size() ) {
                        std::swap( r1 , r2 );
                        std::swap( i , j );
                    }
                    for ( int x : nodes[r2] ) {
                        nodes[r1].push_back( x );
                    }
                    nodes[r2].clear();
                    nodes[r2].shrink_to_fit(); // 释放内存

                    // O(1) 删除 roots[j]
                    roots[j] = roots.back();
                    roots.pop_back();
                }
                if ( index == 1 ) {
                    for ( auto &[u,v] : edges ) {
                        u ++;
                        v ++;
                    }
                }
                return edges;
            }
    };

    static RANDOMTREE RandomTree;

    //随机图生成函数。
    class RANDOMGRAPH {
        public:
            //检查点数 n 和边数 m 是否合法。其余的变量是图的若干限制条件，取名直观，不做解释。
            inline bool check( int n , int m , bool connected , bool repeated_edges , bool self_rings ,
                               bool directional ) {
                bool ret = true;
                ret &= ( !( connected && m < n - 1 ) );
                ret &= ( !( directional && !repeated_edges && 1ll * m > 1ll * n * ( n - 1 ) + n * ( self_rings ) ) );
                ret &= ( !( !directional && !repeated_edges && 1ll * m > 1ll * n * ( n - 1 ) / 2 + n * (
                                self_rings ) ) );
                ret &= ( !( n == 1 && !self_rings && m > 0 ) );
                if ( !ret )
                    std::cerr << "Error: " << n << " " << m << " " << connected << " " << repeated_edges << " "
                            << self_rings << " " << directional << '\n';
                return ret;
            }

            /*
            生成一个点数不超过 N，边数不超过 M 的点数和边数，以 {点数，边数} 的形式返回。注意 N 和 M 应大于等于 3。
            opt 在 [0,+infinity]，其越大，生成的点数边数越接近 N 和 M。
            其余的变量是图的若干限制条件，取名直观，不做解释。
            */
            inline std::vector < int > nm( int N , int M , bool connected , bool repeated_edges , bool self_rings ,
                                           bool directional ,
                                           double opt = 1 ) {
                if ( N < 3 ) return std::vector < int >{ -1 , -1 };
                int n = random( 3 , N , opt ) , m , Ml = 1 , Mr = M;
                if ( connected ) Ml = n - 1;
                if ( directional && !repeated_edges )
                    Mr = std::min(
                        1ll * Mr , 1ll * n * ( n - 1 ) + n * ( self_rings ) );
                if ( !directional && !repeated_edges )
                    Mr = std::min(
                        1ll * Mr , 1ll * n * ( n - 1 ) / 2 + n * ( self_rings ) );
                if ( Ml > Mr ) return { -1 , -1 };
                m = random( Ml , Mr , opt );
                return std::vector < int >{ n , m };
            }

            /*
            随机生成一个 n 个点 m 条边的图。
            vertices_rand 控制是否要打乱点，若不打乱则返回的每个 pair<int,int> 中 first 总是小于等于 second，即使打乱也不改变 1 号点的编号。
            图为有向图且连通时，数据保证从 1 号点可以到达任意一个点。
            其余的变量是图的若干限制条件，取名直观，不做解释。
            */
            inline std::vector < std::pair < int , int > > graph( int n , int m , bool connected , bool repeated_edges ,
                                                                  bool self_rings , bool directional ,
                                                                  bool vertices_rand = 1 ) {
                assert( check( n , m , connected , repeated_edges , self_rings , directional ) );
                auto Gid = [ & ]( int x , int y ) { return 1ll * ( x - 1 ) * n + y; };
                auto Gx = [ & ]( long long w ) { return ( w - 1 ) / n + 1; };
                auto Gy = [ & ]( long long w ) { return ( w - 1 ) % n + 1; };
                std::vector < std::pair < int , int > > graph;
                graph.clear();
                std::unordered_set < long long > S;
                if ( connected ) {
                    std::vector < std::pair < int , int > > tree = RandomTree.random_tree( n , 5 , 0 );
                    for ( std::pair < int , int > edge : tree ) {
                        graph.push_back( edge ), m --;
                        if ( directional ) S.insert( Gid( edge.first , edge.second ) );
                        else
                            S.insert( Gid( std::min( edge.first , edge.second ) ,
                                           std::max( edge.first , edge.second ) ) );
                    }
                }
                if ( m ) {
                    if ( !repeated_edges ) {
                        long long Unum =
                                1ll * n * ( n - 1 ) / ( directional ? 1 : 2 ) + ( self_rings ? n : 0 ) - graph.size();
                        std::vector < long long > edgenum;
                        edgenum.clear();
                        if ( Unum / m <= 10.0 ) {
                            for ( int i = 1 ; i <= n ; i ++ )
                                for ( int j = 1 ; j <= n ; j ++ ) {
                                    if ( !directional && j < i ) continue;
                                    if ( !self_rings && j == i ) continue;
                                    if ( S.find( Gid( i , j ) ) != S.end() ) continue;
                                    edgenum.push_back( Gid( i , j ) );
                                }
                            RandomBasic.random_shuffle( edgenum.begin() , edgenum.end() );
                            int nownum = int( edgenum.size() );
                            while ( nownum > m ) edgenum.pop_back(), nownum --;
                        } else {
                            while ( m -- ) {
                                int x , y;
                                while ( true ) {
                                    x = random( 1 , n ), y = random( 1 , n );
                                    if ( !directional && x > y ) std::swap( x , y );
                                    if ( !self_rings && x == y ) ( x == n ? x -- : y ++ );
                                    if ( S.find( Gid( x , y ) ) == S.end() ) break;
                                }
                                edgenum.push_back( Gid( x , y ) ), S.insert( Gid( x , y ) );
                            }
                        }
                        for ( long long i : edgenum ) graph.push_back( { Gx( i ) , Gy( i ) } );
                    } else {
                        while ( m -- ) {
                            int x = random( 1 , n ) , y = random( 1 , n );
                            if ( !directional && x > y ) std::swap( x , y );
                            if ( !self_rings && x == y ) ( x == n ? x -- : y ++ );
                            graph.push_back( { x , y } );
                        }
                    }
                }
                if ( vertices_rand ) RandomGraphTool.random_vertices( n , graph );
                return graph;
            }
    };

    static RANDOMGRAPH RandomGraph;

    //卡 spfa 生成函数。
    class RANDOMHACKSPFA {
        public:
            //随机生成一张 n 个点的图来卡 spfa，边的权值在 [l,r] 范围内，边数大约是 1.5*n。
            inline std::vector < std::pair < std::pair < int , int > , int > > hackspfa( int n , int l , int r ) {
                std::vector < std::pair < std::pair < int , int > , int > > graph;
                graph.clear();
                if ( l < 0 ) {
                    int root = n / 2;
                    for ( int i = 1 ; i < root ; i ++ ) graph.push_back( { { i , root } , l } );
                    for ( int i = 1 ; i < root - 1 ; i ++ ) graph.push_back( { { i , i + 1 } , l + 1 } );
                    for ( int i = root + 1 ; i <= n ; i ++ )
                        graph.push_back(
                            { { root , i } , ( int ) random( l , r ) } );
                } else {
                    int nn = sqrt( n ) , mm = n / nn;
                    auto gid = [ & ]( int x , int y ) { return ( x - 1 ) * mm + y; };
                    for ( int x = 1 ; x <= nn ; ++x )
                        for ( int y = 1 ; y <= mm ; ++y ) {
                            if ( x < nn ) {
                                graph.push_back( { { gid( x , y ) , gid( x + 1 , y ) } , l } );
                                if ( y < mm )
                                    graph.push_back( {
                                        { gid( x , y ) , gid( x + 1 , y + 1 ) } ,
                                        ( int ) random( std::min( l + 3 , r ) , r )
                                    } );
                            }
                            if ( y < mm )
                                graph.push_back(
                                    {
                                        { gid( x , y ) , gid( x , y + 1 ) } ,
                                        ( int ) random( std::min( l + 3 , r ) , r )
                                    } );
                        }
                    for ( int i = nn * mm ; i < n ; i ++ )
                        graph.push_back(
                            { { i , i + 1 } , ( int ) random( l , r ) } );
                }
                RandomGraphTool.random_vertices( n , graph );
                return graph;
            }
    };

    static RANDOMHACKSPFA RandomHackSpfa;

    //有向无环连通图生成函数。
    class RANDOMDAG {
        public:
            //检查点数 n 和边数 m 是否合法。
            inline bool check( int n , int m ) {
                if ( m >= n - 1 ) return true;
                std::cerr << "Error: " << n << " " << m << '\n';
                return false;
            }

            /*
            生成一个点数不超过 N，边数不超过 M 的点数和边数，以 {点数，边数} 的形式返回。注意 N 和 M 应大于等于 3。
            opt 在 [0,+infinity]，其越大，生成的点数边数越接近 N 和 M。
            */
            inline std::vector < int > nm( int N , int M , double opt = 1 ) {
                if ( N < 3 || M < 3 ) return std::vector < int >{ -1 , -1 };
                int n = random( 3 , N , opt );
                if ( M < n - 1 ) return { -1 , -1 };
                int m = random( n - 1 , M , opt );
                return std::vector < int >{ n , m };
            }

        private:
            inline std::pair < std::vector < int > , std::vector < int > > outdegree( int n , int m ) {
                n --, m --;
                int num0 = std::min( n - 1 , m - n + 1 ) , numn0 = n - 1 - num0;
                std::vector < int > degree = RandomAnother.random_divide_num( 2 * ( n - 1 ) - numn0 , n - 1 );
                for ( int &i : degree ) i --;
                for ( int i = 0 ; i < numn0 ; i ++ ) degree[i] ++;
                sort( degree.begin() , degree.end() );
                FESDRER_STL::FVector fdegree;
                for ( int i : degree ) fdegree.push_back( i );
                int sum = 0;
                for ( int i = 1 ; i < n ; i ++ ) {
                    int l = i - 1 , r = fdegree.lower_bound( i - sum ) - 1;
                    int id = random( l , r ) , num = fdegree[l];
                    sum += fdegree[id];
                    if ( id > l ) {
                        fdegree[l] = fdegree[id];
                        fdegree.erase( id ), fdegree.insert( fdegree.lower_bound( num + 1 ) , num );
                    }
                }
                n ++, m = m + 1 - n + 1;
                degree.assign( 1 , 1 );
                for ( int i = 0 ; i < fdegree.size() ; i ++ ) degree.push_back( fdegree[i] );
                for ( int i = 0 ; i < n - 1 ; i ++ ) if ( !degree[i] ) m --;
                std::vector < int > nodegree = RandomAnother.random_divide_num( m + n - 1 , n - 1 );
                reverse( nodegree.begin() , nodegree.end() );
                for ( int &i : nodegree ) i --;
                for ( int i = 0 ; i < n - 1 ; i ++ ) if ( !degree[i] ) nodegree[i] ++;
                return make_pair( degree , nodegree );
            }

            inline std::vector < std::pair < int , int > > singlest_dag( int n , int m ) {
                std::pair < std::vector < int > , std::vector < int > > degree = outdegree( n , m );
                FESDRER_STL::FVector end( 1 , 1 );
                std::vector < int > noend( 0 );
                std::vector < std::pair < int , int > > graph( 0 );
                for ( int i = 2 ; i <= n ; i ++ ) {
                    int endde = degree.first[i - 2] , node = degree.second[i - 2] , id;
                    while ( endde -- ) {
                        id = random( 0 , end.size() - 1 );
                        graph.push_back( { end[id] , i } );
                        noend.push_back( end[id] );
                        end.erase( id );
                    }
                    while ( node -- ) {
                        id = random( 0 , int( noend.size() ) - 1 );
                        graph.push_back( { noend[id] , i } );
                    }
                    end.push_back( i );
                }
                return graph;
            }

        public:
            //随机生成一个 n 个点 m 条边的 dag，single_s 和 single_t 控制是否只有一个源点和汇点。若只有一个源点则其为 1，若只有一个汇点则其为 n。
            inline std::vector < std::pair < int , int > > dag( int n , int m , bool single_s , bool single_t ) {
                assert( check( n , m ) );
                if ( n == 1 ) return std::vector < std::pair < int , int > >( 0 );
                bool if_reverse;
                if ( single_t && !single_s ) if_reverse = 1, std::swap( single_s , single_t );
                std::vector < std::pair < int , int > > graph( 0 );
                if ( single_s && !single_t ) graph = RandomGraph.graph( n , m , 1 , 1 , 0 , 0 );
                else if ( single_s && single_t ) graph = singlest_dag( n , m );
                else {
                    std::vector < std::vector < int > > e = RandomGraphTool.get_outedge(
                        n , RandomTree.random_tree( n , 5 ) ,
                        0 );
                    std::vector < int > start = RandomAnother.random_different( 1 , n , random( 1 , n , 0.3 ) ) , vis(
                        n ,
                        0 ) , topo(
                        0 );
                    std::queue < int > q;
                    while ( q.size() ) q.pop();
                    for ( int i : start ) q.push( i );
                    while ( q.size() ) {
                        int x = q.front();
                        q.pop();
                        if ( vis[x - 1] ) continue;
                        vis[x - 1] = 1, topo.push_back( x );
                        for ( int y : e[x - 1] ) if ( !vis[y - 1] ) graph.push_back( { x , y } ), q.push( y );
                    }
                    assert( topo.size() == n );
                    for ( int i = n ; i <= m ; i ++ ) {
                        int s = random( 1 , n ) , t = random( 1 , n );
                        if ( s > t ) std::swap( s , t );
                        if ( s == t ) ( s == 1 ? t ++ : s -- );
                        graph.push_back( { topo[s - 1] , topo[t - 1] } );
                    }
                }
                RandomGraphTool.random_vertices( n , graph );
                if ( if_reverse )
                    for ( std::pair < int , int > &it : graph ) {
                        it.first = n + 1 - it.first, it.second = n + 1 - it.second;
                        std::swap( it.first , it.second );
                    }
                return graph;
            }
    };

    static RANDOMDAG RandomDag;

    //强连通分量生成函数。
    class RANDOMSCC {
        public:
            //检查点数 n 边数 m 强连通分量数 sccn 和缩点后的边数 sccm 是否合法。
            inline bool check( int n , int m , int sccn , int sccm ) {
                bool ret = true;
                ret &= ( m - sccm >= n - sccn + 1 - ( sccn == n ) );
                ret &= ( sccm >= sccn - 1 );
                ret &= ( m - sccm != 1 );
                ret &= ( n != sccn || m == sccm );
                ret &= ( sccn <= n );
                if ( ret ) return true;
                else std::cerr << "Error: " << n << " " << m << " " << sccn << " " << sccm << '\n';
                return false;
            }

            /*
            生成一个点数不超过 N，边数不超过 M 的点数和边数以及强连通分量数 sccn 和缩点后的边数 sccm，以 {n,m,sccn,sccm} 的形式返回。注意 N 和 M 应大于等于 3。
            opt 在 [0,+infinity]，其越大，生成的点数边数越接近 N 和 M。
            rhosccn 表示 sccn 在范围允许的情况下为 n*rhosccn。
            rhosccm 表示 sccm 在范围允许的情况下为 sccn*rhosccm。
            */
            inline std::vector < int > nm( int N , int M , double opt = 1 , double rhosccn = 0.3 ,
                                           double rhosccm = 3 ) {
                if ( N < 3 || M < 3 ) return std::vector < int >{ -1 , -1 , -1 , -1 };
                int n = random( 3 , std::min( N , M ) , opt ) , sccn = std::min( n , std::max( 3 , int( n *
                    rhosccn ) ) ) , m , Lsccm =
                        sccn - 1 , Rsccm = M - n + sccn - 1 + ( sccn == n );
                if ( Lsccm > Rsccm || ( Lsccm >= M - 1 && n != sccn ) ) return std::vector < int >{ -1 , -1 , -1 , -1 };
                int sccm = std::min( Rsccm , std::max( int( sccn * rhosccm ) , Lsccm ) );
                if ( sccm == M - 1 && n != sccn ) sccm --;
                m = ( n == sccn ? sccm : random( n - sccn + 1 - ( n == sccn ) + sccm , M , opt ) );
                if ( m - sccm == 1 ) m ++;
                return { n , m , sccn , sccm };
            }

            /*
            随机生成一个 n 个点 m 条边 sccn 个强连通分量和 sccm 个缩点后的边的连通有向图。
            缩点完后的图为调入的 dag。
            rho 在 [0,+infinity]，表示强连通分量的稠密度。其越大，强连通分量就有越多的环。
            */
            inline std::vector < std::pair < int , int > > scc( int n , int m , int sccn , int sccm ,
                                                                std::vector < std::pair < int , int > > dag ,
                                                                double rho = 1 ) {
                assert( check( n , m , sccn , sccm ) );
                assert( int(dag.size())==sccm );
                int simgle_num = std::max( 0 , n - ( m - sccm ) );
                if ( simgle_num == n ) return dag;
                std::vector < std::vector < int > > scc = RandomAnother.random_divide( 1 , n - simgle_num ,
                    sccn - simgle_num ) , basescc( 0 );
                std::vector < int > simgle_id;
                simgle_id.clear();
                for ( int i = n ; i > n - simgle_num ; i -- ) simgle_id.push_back( i );
                for ( int i = 0 ; i < sccn - simgle_num ; i ++ ) {
                    if ( scc[i].size() == 1 ) simgle_id.push_back( scc[i][0] ), simgle_num ++;
                    else basescc.push_back( scc[i] );
                }
                scc = basescc;
                std::vector < int > edgen = RandomAnother.random_divide_num( m - sccm - n + sccn , sccn - simgle_num );
                std::vector < std::pair < int , int > > graph;
                graph.clear();
                auto Get = [ & ]( const std::vector < int > &vec ) { return vec[random( 0 , vec.size() - 1 )]; };
                for ( int i = 0 ; i < sccn - simgle_num ; i ++ ) {
                    std::vector < int > &vertex = scc[i];
                    int vernum = int( vertex.size() ) , edgenum = vernum - 1 + edgen[i] , top = random( 1 , vernum - 1 ,
                        rho );
                    int cnt = edgen[i] - 1;
                    if ( cnt == 0 ) {
                        for ( int i = 1 ; i < vernum ; i ++ ) graph.push_back( { vertex[i - 1] , vertex[i] } );
                        graph.push_back( { vertex[vernum - 1] , vertex[0] } );
                        continue;
                    }
                    for ( int i = 1 ; i <= top ; i ++ ) graph.push_back( { vertex[i - 1] , vertex[i] } ), edgenum --;
                    graph.push_back( { vertex[top] , vertex[0] } ), edgenum --;
                    while ( top < vernum - 1 ) {
                        int newtop = ( cnt == 1 ? vernum - 1 : random( top + 1 , vernum - 1 , rho ) );
                        int st = vertex[random( 0 , top )] , ed = vertex[random( 0 , top )];
                        cnt --;
                        graph.push_back( { st , vertex[top + 1] } ), edgenum --;
                        for ( int i = top + 2 ; i <= newtop ; i
                              ++ )
                            graph.push_back( { vertex[i - 1] , vertex[i] } ), edgenum --;
                        graph.push_back( { vertex[newtop] , ed } ), edgenum --;
                        top = newtop;
                    }
                    while ( edgenum -- ) {
                        int st = Get( vertex ) , ed = Get( vertex );
                        while ( st == ed ) ed = Get( vertex );
                        graph.push_back( { st , ed } );
                    }
                }
                for ( int i : simgle_id ) scc.push_back( std::vector < int >{ i } );
                for ( std::pair < int , int > i : dag )
                    graph.push_back( {
                        Get( scc[i.first - 1] ) , Get( scc[i.second - 1] )
                    } );
                RandomGraphTool.random_vertices( n , graph );
                return graph;
            }
    };

    static RANDOMSCC RandomScc;

    //边双连通分量生成函数。
    class RANDOMEDCC {
        public:
            //检查点数 n 边数 m 边双连通分量数 edccn 是否合法。
            inline bool check( int n , int m , int edccn ) {
                if ( RandomScc.check( n , m , edccn , edccn - 1 ) ) return true;
                std::cerr << "Error: " << n << " " << m << " " << edccn << '\n';
            }

            /*
            生成一个点数不超过 N，边数不超过 M 的点数和边数以及边双连通分量数 edccn，以 {n,m,edccn} 的形式返回。注意 N 和 M 应大于等于 3。
            opt 在 [0,+infinity]，其越大，生成的点数边数越接近 N 和 M。
            rhoedccn 表示 edccn 在范围允许的情况下为 n*rhoedccn。
            */
            inline std::vector < int > nm( int N , int M , double opt = 1 , double rhoedccn = 0.3 ) {
                if ( N < 3 ) return std::vector < int >{ -1 , -1 , -1 };
                int n = random( 3 , std::min( N , M ) , opt ) , edccn = std::min(
                    std::max( 3 , int( rhoedccn * n ) ) , n );
                if ( n == edccn ) return std::vector < int >{ n , n - 1 , edccn };
                else return std::vector < int >{ n , ( int ) random( n , M , opt ) , edccn };
            }

            /*
            随机生成一个 n 个点 m 条边 edccn 个边双连通分量的连通无向图。
            rho 在 [0,+infinity]，表示边双连通分量的稠密度。其越大，边双连通分量就有越多的环。
            */
            inline std::vector < std::pair < int , int > > edcc( int n , int m , int edccn ,
                                                                 std::vector < std::pair < int , int > > tree ,
                                                                 double rho = 1 ) {
                return RandomScc.scc( n , m , edccn , edccn - 1 , tree , rho );
            }
    };

    static RANDOMEDCC RandomEdcc;

    //点双连通分量生成函数。
    class RANDOMVDCC {
        public:
            //检查点数 n 边数 m 点双连通分量数 vdccn 是否合法。
            inline bool check( int n , int m , int vdccn ) {
                if ( n == 1 ) {
                    if ( m == 0 && vdccn == 1 ) return true;
                    else std::cerr << "Error: " << n << " " << m << " " << vdccn << '\n';
                    return false;
                }
                if ( vdccn <= n - 1 && m >= n - 1 + ( n - 1 != vdccn ) ) return true;
                else std::cerr << "Error: " << n << " " << m << " " << vdccn << '\n';
                return false;
            }

            /*
            生成一个点数不超过 N，边数不超过 M 的点数和边数以及点双连通分量数 vdccn，以 {n,m,vdccn} 的形式返回。注意 N 和 M 应大于等于 3。
            opt 在 [0,+infinity]，其越大，生成的点数边数越接近 N 和 M。
            rhovdccn 表示 vdccn 在范围允许的情况下为 n*rhovdccn。
            */
            inline std::vector < int > nm( int N , int M , double opt = 1 , double rhovdccn = 0.4 ) {
                if ( N < 3 || M < 3 ) return std::vector < int >{ -1 , -1 , -1 };
                if ( random( 1 , 100 ) >= 95 ) return std::vector < int >{ 1 , 0 , 1 };
                int n = random( 3 , std::min( N , M ) , opt ) , vdccn = ( M == n - 1
                                                                              ? n - 1
                                                                              : std::max( 1 , std::min(
                                                                                  int( n * rhovdccn ) , n - 1 ) ) );
                int m = random( n - 1 + ( n - 1 != vdccn ) , M , opt );
                return { n , m , vdccn };
            }

        private:
            //返回每个vdcc的组成，基本思路是模拟vdcc通过一个个割点连成一棵树的过程
            inline std::vector < std::vector < int > > vdcc_spread( int n , int m , int vdccn ) {
                if ( n == 1 ) return std::vector < std::vector < int > >{ std::vector < int >{ 1 } };
                int double_num = std::max( 0 , n - 1 + vdccn - m );
                std::vector < int > degree = RandomAnother.random_divide_num( n - 1 - double_num ,
                                                                              vdccn - double_num ); //每个 vdcc 的边数
                for ( int &i : degree ) i += 1;
                for ( int i = 1 ; i <= double_num ; i ++ ) degree.push_back( 2 );
                sort( degree.begin() , degree.end() );
                FESDRER_STL::FVector top( 1 , 0 ); //当前剩下哪些点没成为vdcc
                std::vector < std::vector < int > > fa( 1 , std::vector < int >{ 0 } ) , vdcc( 0 ); //fa是并查集中的上级
                for ( int i = 1 ; i <= n ; i ++ ) fa.push_back( std::vector < int >{ i } ), top.push_back( i );
                for ( int nowdegree : degree ) {
                    std::vector < int > topid = RandomAnother.random_different( 1 , top.size() - 1 , nowdegree );
                    sort( topid.begin() , topid.end() );
                    for ( int i = 0 ; i < nowdegree ; i ++ ) {
                        int __ = topid[i];
                        topid[i] = top[topid[i] - i];
                        top.erase( __ - i );
                    }
                    int mx = 0;
                    std::vector < int > nowvdcc( 0 );
                    for ( int i : topid ) {
                        if ( !mx || int( fa[i].size() ) > int( fa[mx].size() ) ) mx = i;
                        nowvdcc.push_back( fa[i][random( 0 , int( fa[i].size() ) - 1 )] );
                    }
                    for ( int i : topid )
                        if ( i != mx )
                            while ( fa[i].
                                size() )
                                fa[mx].push_back( fa[i].back() ), fa[i].pop_back();
                    top.push_back( mx );
                    vdcc.push_back( nowvdcc );
                }
                return vdcc;
            }

        public:
            /*
            随机生成一个 n 个点 m 条边 vdccn 个点双连通分量的连通无向图。
            rho 在 [0,+infinity]，表示点双连通分量的稠密度。其越大，点双连通分量就有越多的环。
            */
            inline std::vector < std::pair < int , int > > vdcc( int n , int m , int vdccn , double rho = 1 ) {
                assert( check( n , m , vdccn ) );
                if ( n == 1 ) return std::vector < std::pair < int , int > >( 0 );
                std::vector < std::vector < int > > vdcc = vdcc_spread( n , m , vdccn );
                int sum = 0;
                for ( std::vector < int > i : vdcc ) sum += ( int( i.size() ) == 2 ? 1 : int( i.size() ) );
                std::vector < int > edgen = RandomAnother.random_divide_num( m - sum + vdccn , vdccn );
                std::vector < std::pair < int , int > > graph( 0 );
                for ( int i = 0 ; i < vdccn ; i ++ ) {
                    std::vector < int > &vertex = vdcc[i];
                    int vernum = int( vertex.size() ) , edgenum = edgen[i] + ( vernum == 2 ? 1 : vernum ) - 1;
                    int cnt = edgen[i] - 1 , top = random( 1 , vernum - 1 , rho );;
                    if ( cnt == 0 ) {
                        for ( int i = 1 ; i < vernum ; i ++ ) graph.push_back( { vertex[i - 1] , vertex[i] } );
                        graph.push_back( { vertex[vernum - 1] , vertex[0] } );
                        continue;
                    }
                    for ( int i = 1 ; i <= top ; i ++ ) graph.push_back( { vertex[i - 1] , vertex[i] } ), edgenum --;
                    graph.push_back( { vertex[top] , vertex[0] } ), edgenum --;
                    while ( top < vernum - 1 ) {
                        int newtop = ( cnt == 1 ? vernum - 1 : random( top + 1 , vernum - 1 , rho ) );
                        int st = vertex[random( 0 , top )] , ed = vertex[random( 0 , top )];
                        while ( st == ed ) ed = vertex[random( 0 , top )];
                        cnt --;
                        graph.push_back( { st , vertex[top + 1] } ), edgenum --;
                        for ( int i = top + 2 ; i <= newtop ; i
                              ++ )
                            graph.push_back( { vertex[i - 1] , vertex[i] } ), edgenum --;
                        graph.push_back( { vertex[newtop] , ed } ), edgenum --;
                        top = newtop;
                    }
                    auto Get = [ & ]( const std::vector < int > &vec ) { return vec[random( 0 , vec.size() - 1 )]; };
                    while ( edgenum -- ) {
                        int st = Get( vertex ) , ed = Get( vertex );
                        while ( st == ed ) ed = Get( vertex );
                        graph.push_back( { st , ed } );
                    }
                }
                RandomGraphTool.random_vertices( n , graph );
                return graph;
            }
    };

    static RANDOMVDCC RandomVdcc;
}

namespace FESDRER_CHECK {
    class CHECKTREE {
        private:
            inline void dep_dfs( int x , int fas , std::vector < std::vector < int > > &e , std::vector < int > &dep ) {
                if ( x != 1 ) dep[x - 1] = dep[fas - 1] + 1;
                for ( int y : e[x] ) if ( y != fas ) dep_dfs( y , x , e , dep );
            }

        public:
            //返回以 1 为根的树中每个点的深度，1 的深度为 1。
            inline std::vector < int > Get_dep( std::vector < std::pair < int , int > > tree ) {
                std::vector < int > dep( int( tree.size() ) + 1 , 0 );
                dep[0] = 1;
                std::vector < std::vector < int > > e( int( tree.size() ) + 2 , std::vector < int >{} );
                for ( std::pair < int , int > i : tree ) {
                    e[i.first].push_back( i.second );
                    e[i.second].push_back( i.first );
                }
                dep_dfs( 1 , 0 , e , dep );
                return dep;
            }

            //返回树中每个点的度数。
            inline std::vector < int > Get_degree( std::vector < std::pair < int , int > > tree ) {
                std::vector < int > degree( int( tree.size() ) + 1 , 0 );
                for ( std::pair < int , int > i : tree ) degree[i.first] ++, degree[i.second] ++;
                return degree;
            }
    };

    static CHECKTREE CheckTree;

    class CHECKSCC {
        private:
            inline void tarjan( int x , std::vector < int > &dfn , std::vector < int > &low , std::stack < int > &stk ,
                                std::vector < int > &fst , std::vector < int > &nxt , std::vector < int > &tal ,
                                std::vector < bool > &instk , std::vector < std::vector < int > > &scc , int &tod ) {
                dfn[x] = low[x] = ++tod;
                stk.push( x ), instk[x] = 1;
                for ( int i = fst[x] ; i ; i = nxt[i] ) {
                    int y = tal[i];
                    if ( !dfn[y] )
                        tarjan( y , dfn , low , stk , fst , nxt , tal , instk , scc , tod ), low[x] = std::min( low[x] ,
                            low[y] );
                    else if ( instk[y] ) low[x] = std::min( low[x] , dfn[y] );
                }
                if ( low[x] == dfn[x] ) {
                    int y;
                    std::vector < int > now( 0 );
                    do {
                        y = stk.top();
                        stk.pop();
                        now.push_back( y ), instk[y] = 0;
                    } while ( y != x );
                    scc.push_back( now );
                }
            }

        public:
            //返回有向图中的所有强连通分量。
            inline std::vector < std::vector < int > > check( int n , std::vector < std::pair < int , int > > graph ) {
                int N = n + 5 , m = int( graph.size() ) , M = m + 5;
                std::vector < int > fst( N ) , nxt( M ) , tal( M ) , dfn( N ) , low( N );
                std::vector < bool > instk( N );
                int tot = 1 , tod = 0;
                std::stack < int > stk;
                std::vector < std::vector < int > > scc( 0 );
                while ( stk.size() ) stk.pop();
                auto add = [ & ]( int x , int y ) { tal[++tot] = y, nxt[tot] = fst[x], fst[x] = tot; };
                for ( std::pair < int , int > it : graph ) add( it.first , it.second );
                for ( int i = 1 ; i <= n ; i ++ )
                    if ( !dfn[i] )
                        tarjan(
                            i , dfn , low , stk , fst , nxt , tal , instk , scc , tod );
                return scc;
            }
    };

    static CHECKSCC CheckScc;

    class CHECKEDCC {
        private:
            inline void tarjan( int x , int in_edge , std::vector < int > &fst , std::vector < int > &nxt ,
                                std::vector < int > &tal ,
                                std::vector < int > &dfn , std::vector < int > &low , std::stack < int > &stk ,
                                std::vector < std::vector < int > > &edcc , int &tod ) {
                dfn[x] = low[x] = ++tod;
                stk.push( x );
                for ( int i = fst[x] ; i ; i = nxt[i] ) {
                    int y = tal[i];
                    if ( !dfn[y] )
                        tarjan( y , i , fst , nxt , tal , dfn , low , stk , edcc , tod ), low[x] = std::min( low[x] ,
                            low[y] );
                    else if ( i != ( 1 ^ in_edge ) ) low[x] = std::min( low[x] , dfn[y] );
                }
                if ( dfn[x] == low[x] ) {
                    int y;
                    std::vector < int > now( 0 );
                    do {
                        y = stk.top();
                        stk.pop();
                        now.push_back( y );
                    } while ( y != x );
                    edcc.push_back( now );
                }
            }

        public:
            //返回有向图中的所有边双连通分量。
            inline void check( int n , std::vector < std::pair < int , int > > graph ) {
                int N = n + 5 , m = int( graph.size() ) , M = m * 2 + 5;
                std::vector < int > fst( N ) , nxt( M ) , tal( M ) , dfn( N ) , low( N );
                int tot = 1 , tod = 0;
                std::stack < int > stk;
                std::vector < std::vector < int > > edcc( 0 );
                while ( stk.size() ) stk.pop();
                auto add = [ & ]( int x , int y ) { tal[++tot] = y, nxt[tot] = fst[x], fst[x] = tot; };
                for ( std::pair < int , int > it : graph ) add( it.first , it.second ), add( it.second , it.first );
                for ( int i = 1 ; i <= n ; i ++ )
                    if ( !dfn[i] )
                        tarjan(
                            i , 0 , fst , nxt , tal , dfn , low , stk , edcc , tod );
            }
    };

    static CHECKEDCC CheckEdcc;

    class CHECKVDCC {
        private:
            void tarjan( int x , std::vector < int > &fst , std::vector < int > &nxt , std::vector < int > &tal ,
                         std::vector < int > &dfn , std::vector < int > &low , std::stack < int > &stk ,
                         std::vector < std::vector < int > > &vdcc , int &tod , int &root ) {
                if ( x == root && !fst[x] ) {
                    vdcc.push_back( std::vector < int >{ x } );
                    return;
                }
                dfn[x] = low[x] = ++tod;
                stk.push( x );
                for ( int i = fst[x] ; i ; i = nxt[i] ) {
                    int y = tal[i];
                    if ( !dfn[y] ) {
                        tarjan( y , fst , nxt , tal , dfn , low , stk , vdcc , tod , root );
                        low[x] = std::min( low[x] , low[y] );
                        if ( low[y] >= dfn[x] ) {
                            int z;
                            std::vector < int > now( 1 , x );
                            do {
                                z = stk.top();
                                stk.pop();
                                now.push_back( z );
                            } while ( z != y );
                            vdcc.push_back( now );
                        }
                    } else low[x] = std::min( low[x] , dfn[y] );
                }
            }

        public:
            //返回有向图中的所有边双连通分量。
            inline void check( int n , std::vector < std::pair < int , int > > graph ) {
                int N = n + 5 , m = int( graph.size() ) , M = m * 2 + 5;
                std::vector < int > fst( N ) , nxt( M ) , tal( M ) , dfn( N ) , low( N );
                int tot = 1 , tod = 0 , root = 0;
                std::stack < int > stk;
                std::vector < std::vector < int > > vdcc( 0 );
                while ( stk.size() ) stk.pop();
                auto add = [ & ]( int x , int y ) { tal[++tot] = y, nxt[tot] = fst[x], fst[x] = tot; };
                for ( std::pair < int , int > it : graph ) add( it.first , it.second ), add( it.second , it.first );
                for ( int i = 1 ; i <= n ; i ++ )
                    if ( !dfn[i] )
                        tarjan(
                            ( root = i ) , fst , nxt , tal , dfn , low , stk , vdcc , tod , root );
            }
    };

    static CHECKVDCC CheckVdcc;
}

using namespace std;
//typedef long long ll; //  -9223372036854775807ll - 1 ~ 9223372036854775807ll
typedef unsigned long long ull;
#define int long long
#define _128t __int128_t
#define INF 0X7FFFFFFF
#define nullptr 0
#define PII pair<int,int>
#define PDD pair<double,double>
#define PPI pair< pair<int,int> , int >
#define mmset( x , y ) memset( x , y , sizeof(x) )
#define debug( var , begin , end ) for_each((var)+(begin),(var)+(end),[=](int val){ cout << val << " ";} )
#define br cout << "\n"
const int MAX = 0x3f3f3f3f3f3f3f3f;
const int mod = 1e9 + 7;
const int Max = 998244353;
const double PI = acos( -1 );

// 数据文件地址
const string dataFilePath = "D:\\Desktop\\data\\";
// 输入输出文件类型
const string inFileType = ".in";
const string outFileType = ".out";

// 字符集
const string charNumber = "0123456789";
const string charLowerCase = "abcdefghijklmnopqrstuvwxyz";
const string charUpperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// 数据结构简写
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
/**
 * @brief 快速幂
 * @param x 底数
 * @param pow 指数
 * @param p 模数（模数为 0 时不取模）
 * @return x^pow % p
 */
int QuickPow( int x , int pow , int p );

/**
 * @brief 拓展欧几里得算法求解最大公约数 gcd(a,b)
 * @param a
 * @param b
 * @param x
 * @param y
 * @return 最大公约数 d
 */
int exgcd( int a , int b , int &x , int &y );

/**
 * @brief 随机字符串生成器
 * @param n 字符串长度
 * @param chars 随机字符串内容
 * @param allowZero 是否允许字符串中出现前导零 默认不允许
 * @return 随机字符串
 */
string getRandomString( int n , string chars , bool allowZero = false );

/**
 * @brief 随机字符串生成器
 * @param chars 字符串数组内容
 * @return 字符串数组中随机字符串
 */
string getRandomString( vector < string > chars );

/**
 * @brief 打开输入文件流
 * @param str 文件名
 * @return
 */
ifstream openInputFileStream( const string &str );

/**
 * @brief 打开输出文件流
 * @param str 文件名
 * @return
 */
ofstream openOutputFileStream( const string &str );

/**
 * @brief 高精度模板类 BigIntTiny
 */
struct BigIntTiny {
    int sign;
    std::vector < int > v;

    BigIntTiny() : sign( 1 ) {
    }

    BigIntTiny( const std::string &s ) { *this = s; }

    BigIntTiny( int v ) {
        char buf[21];
        sprintf( buf , "%lld" , v );
        *this = buf;
    }

    void zip( int unzip ) {
        if ( unzip == 0 ) {
            for ( int i = 0 ; i < ( int ) v.size() ; i ++ )
                v[i] = get_pos( i * 4 ) + get_pos( i * 4 + 1 ) * 10 + get_pos( i * 4 + 2 ) * 100 +
                       get_pos( i * 4 + 3 ) * 1000;
        } else
            for ( int i = ( v.resize( v.size() * 4 ), ( int ) v.size() - 1 ) , a ; i >= 0 ; i
                  -- )
                a = ( i % 4 >= 2 ) ? v[i / 4] / 100 : v[i / 4] % 100, v[i] = ( i & 1 ) ? a / 10 : a % 10;
        setsign( 1 , 1 );
    }

    int get_pos( unsigned pos ) const { return pos >= v.size() ? 0 : v[pos]; }

    BigIntTiny &setsign( int newsign , int rev ) {
        for ( int i = ( int ) v.size() - 1 ; i > 0 && v[i] == 0 ; i -- ) v.erase( v.begin() + i );
        sign = ( v.size() == 0 || ( v.size() == 1 && v[0] == 0 ) ) ? 1 : ( rev ? newsign * sign : newsign );
        return *this;
    }

    std::string to_str() const {
        BigIntTiny b = *this;
        std::string s;
        for ( int i = ( b.zip( 1 ), 0 ) ; i < ( int ) b.v.size() ; ++i ) s += char( *( b.v.rbegin() + i ) + '0' );
        return ( sign < 0 ? "-" : "" ) + ( s.empty() ? std::string( "0" ) : s );
    }

    bool absless( const BigIntTiny &b ) const {
        if ( v.size() != b.v.size() ) return v.size() < b.v.size();
        for ( int i = ( int ) v.size() - 1 ; i >= 0 ; i -- ) if ( v[i] != b.v[i] ) return v[i] < b.v[i];
        return false;
    }

    BigIntTiny operator -() const {
        BigIntTiny c = *this;
        c.sign = ( v.size() > 1 || v[0] ) ? -c.sign : 1;
        return c;
    }

    BigIntTiny &operator =( const std::string &s ) {
        if ( s[0] == '-' ) *this = s.substr( 1 );
        else {
            for ( int i = ( v.clear(), 0 ) ; i < ( int ) s.size() ; ++i ) v.push_back( *( s.rbegin() + i ) - '0' );
            zip( 0 );
        }
        return setsign( s[0] == '-' ? -1 : 1 , sign = 1 );
    }

    bool operator <( const BigIntTiny &b ) const {
        return sign != b.sign ? sign < b.sign : ( sign == 1 ? absless( b ) : b.absless( *this ) );
    }

    bool operator ==( const BigIntTiny &b ) const { return v == b.v && sign == b.sign; }

    BigIntTiny &operator +=( const BigIntTiny &b ) {
        if ( sign != b.sign ) return *this = ( *this ) - -b;
        v.resize( std::max( v.size() , b.v.size() ) + 1 );
        for ( int i = 0 , carry = 0 ; i < ( int ) b.v.size() || carry ; i ++ ) {
            carry += v[i] + b.get_pos( i );
            v[i] = carry % 10000, carry /= 10000;
        }
        return setsign( sign , 0 );
    }

    BigIntTiny operator +( const BigIntTiny &b ) const {
        BigIntTiny c = *this;
        return c += b;
    }

    void add_mul( const BigIntTiny &b , int mul ) {
        v.resize( std::max( v.size() , b.v.size() ) + 2 );
        for ( int i = 0 , carry = 0 ; i < ( int ) b.v.size() || carry ; i ++ ) {
            carry += v[i] + b.get_pos( i ) * mul;
            v[i] = carry % 10000, carry /= 10000;
        }
    }

    BigIntTiny operator -( const BigIntTiny &b ) const {
        if ( b.v.empty() || b.v.size() == 1 && b.v[0] == 0 ) return *this;
        if ( sign != b.sign ) return ( *this ) + -b;
        if ( absless( b ) ) return -( b - *this );
        BigIntTiny c;
        for ( int i = 0 , borrow = 0 ; i < ( int ) v.size() ; i ++ ) {
            borrow += v[i] - b.get_pos( i );
            c.v.push_back( borrow );
            c.v.back() -= 10000 * ( borrow >>= 31 );
        }
        return c.setsign( sign , 0 );
    }

    BigIntTiny operator *( const BigIntTiny &b ) const {
        if ( b < *this ) return b * *this;
        BigIntTiny c , d = b;
        for ( int i = 0 ; i < ( int ) v.size() ; i ++, d.v.insert( d.v.begin() , 0 ) ) c.add_mul( d , v[i] );
        return c.setsign( sign * b.sign , 0 );
    }

    BigIntTiny operator /( const BigIntTiny &b ) const {
        BigIntTiny c , d;
        BigIntTiny e = b;
        e.sign = 1;

        d.v.resize( v.size() );
        double db = 1.0 / ( b.v.back() + ( b.get_pos( ( unsigned ) b.v.size() - 2 ) / 1e4 ) +
                            ( b.get_pos( ( unsigned ) b.v.size() - 3 ) + 1 ) / 1e8 );
        for ( int i = ( int ) v.size() - 1 ; i >= 0 ; i -- ) {
            c.v.insert( c.v.begin() , v[i] );
            int m = ( int ) ( ( c.get_pos( ( int ) e.v.size() ) * 10000 + c.get_pos( ( int ) e.v.size() - 1 ) ) * db );
            c = c - e * m, c.setsign( c.sign , 0 ), d.v[i] += m;
            while ( !( c < e ) ) c = c - e, d.v[i] += 1;
        }
        return d.setsign( sign * b.sign , 0 );
    }

    BigIntTiny operator %( const BigIntTiny &b ) const { return *this - *this / b * b; }

    bool operator >( const BigIntTiny &b ) const { return b < *this; }

    bool operator <=( const BigIntTiny &b ) const { return !( b < *this ); }

    bool operator >=( const BigIntTiny &b ) const { return !( *this < b ); }

    bool operator !=( const BigIntTiny &b ) const { return !( *this == b ); }
};

/**
 * @brief 类型安全的内存清零/填充工具
 * 自动区分 POD 类型和复杂类型，选择合适的清零方式
 */
class MemUtil {
    public:
        // ==================== 清零单个变量 ====================
        template < typename T > static void zero( T &var ) {
            if constexpr ( std::is_trivially_copyable_v < T > ) {
                // POD 类型：可以安全使用 memset
                memset( &var , 0 , sizeof( T ) );
            } else {
                // 复杂类型：使用默认构造函数重置
                var = T{};
            }
        }

        // ==================== 清零 C 风格数组 ====================
        template < typename T , size_t N > static void zero( T ( &arr )[N] ) {
            if constexpr ( std::is_trivially_copyable_v < T > ) {
                memset( arr , 0 , sizeof( arr ) );
            } else {
                // 复杂类型：逐个重置
                for ( size_t i = 0 ; i < N ; ++i ) {
                    arr[i] = T{};
                }
            }
        }

        // ==================== 清零 std::array ====================
        template < typename T , size_t N > static void zero( std::array < T , N > &arr ) {
            if constexpr ( std::is_trivially_copyable_v < T > ) {
                memset( arr.data() , 0 , sizeof( T ) * N );
            } else {
                for ( size_t i = 0 ; i < N ; ++i ) {
                    arr[i] = T{};
                }
            }
        }

        // ==================== 清零 std::vector ====================
        template < typename T > static void zero( std::vector < T > &vec ) {
            if ( vec.empty() ) return;

            if constexpr ( std::is_trivially_copyable_v < T > ) {
                memset( vec.data() , 0 , sizeof( T ) * vec.size() );
            } else {
                for ( auto &item : vec ) {
                    item = T{};
                }
            }
        }

        // ==================== 填充特定值（字节级） ====================
        // 仅对 POD 类型安全
        template < typename T > static void fill( T &var , int value ) {
            static_assert( std::is_trivially_copyable_v < T > ,
                           "fill() 只能用于 POD 类型！对于复杂类型请使用 setValue()" );
            memset( &var , value , sizeof( T ) );
        }

        template < typename T , size_t N > static void fill( T ( &arr )[N] , int value ) {
            static_assert( std::is_trivially_copyable_v < T > ,
                           "fill() 只能用于 POD 类型！对于复杂类型请使用 setValue()" );
            memset( arr , value , sizeof( arr ) );
        }

        // ==================== 设置特定值（对象级） ====================
        template < typename T , size_t N > static void setValue( T ( &arr )[N] , const T &value ) {
            for ( size_t i = 0 ; i < N ; ++i ) {
                arr[i] = value;
            }
        }

        template < typename T > static void setValue( std::vector < T > &vec , const T &value ) {
            std::fill( vec.begin() , vec.end() , value );
        }

        template < typename T , size_t N > static void setValue( std::array < T , N > &arr , const T &value ) {
            std::fill( arr.begin() , arr.end() , value );
        }

        // ==================== 清空容器（size 变为 0） ====================
        template < typename T > static void clear( std::vector < T > &vec ) {
            vec.clear();
        }

        // ==================== 重置容器大小并清零 ====================
        template < typename T > static void reset( std::vector < T > &vec , size_t newSize = 0 ) {
            vec.assign( newSize , T{} );
        }
};

/**
 * @brief 树结构
 * @details 树结构的定义
 * @param numNodes 总节点数
 * @param edges [father,son]
 */
struct Tree {
    int numNodes;
    vector < pair < int , int > > edges;
};

/**
 * @brief 随机树结构生成
 * @details 树结构的生成
 * @param n 节点数
 */
Tree getRandomTree( int n );

/**
 * @brief 随机 T 组数据生成
 * @param l 最小值
 * @param r 最大值
 * @param nums 数量
 * @param limit 总和上限
 * @return T 组数据生成结果
 */

vector < int > getRandomTVector( int l , int r , int nums , int limit );

// ==================== 梯度区间结构 ====================
struct GradientRange {
    long long minVal; // 区间最小值
    long long maxVal; // 区间最大值
    int fileCount; // 分配的文件数量
};

/**
 * @brief 梯度配置生成器
 * @details 梯度配置生成器
*/
// ==================== 梯度配置生成器 ====================
class GradientScaleGenerator {
    private:
        vector < GradientRange > gradients_;
        int totalFiles_;
        long long maxScale_;

    public:
        /**
         * 自动生成梯度配置
         * @param maxScale   最大数据规模
         * @param totalFiles 总文件数量（默认20）
         */
        GradientScaleGenerator( long long maxScale , int totalFiles = 20 ) : totalFiles_( totalFiles ),
                                                                             maxScale_( maxScale ) {
            autoGenerateGradients();
        }

        /**
         * 使用自定义梯度配置
         */
        GradientScaleGenerator( const vector < GradientRange > &customGradients ) : gradients_( customGradients ) {
            totalFiles_ = 0;
            maxScale_ = 0;
            for ( const auto &g : gradients_ ) {
                totalFiles_ += g.fileCount;
                maxScale_ = max( maxScale_ , g.maxVal );
            }
        }

    private:
        void autoGenerateGradients() {
            gradients_.clear();

            // Step 1: 生成10的幂次边界点 [1, 10, 100, 1000, ...]
            vector < long long > boundaries = { 1 };
            long long val = 10;
            while ( val < maxScale_ ) {
                boundaries.push_back( val );
                val *= 10;
            }

            // Step 2: 最后区间较大时，增加中间分割点（如 50000）
            if ( !boundaries.empty() && boundaries.back() * 5 <= maxScale_ ) {
                boundaries.push_back( maxScale_ / 2 );
            }
            boundaries.push_back( maxScale_ );

            int numRanges = ( int ) boundaries.size() - 1;
            if ( numRanges == 0 ) {
                gradients_.push_back( { 1 , maxScale_ , totalFiles_ } );
                return;
            }

            // Step 3: 分配文件数量
            // 策略：最后一个区间约占25%，其余区间平均分配
            int lastRangeCount = max( 1ll , totalFiles_ / 4 );
            int remainingFiles = totalFiles_ - lastRangeCount;
            int otherRanges = numRanges - 1;

            for ( int i = 0 ; i < numRanges ; i ++ ) {
                GradientRange g;
                g.minVal = boundaries[i];
                g.maxVal = boundaries[i + 1];

                if ( i < numRanges - 1 && otherRanges > 0 ) {
                    // 前面的区间平均分配
                    g.fileCount = remainingFiles / otherRanges;
                    if ( i < remainingFiles % otherRanges ) {
                        g.fileCount ++;
                    }
                } else {
                    // 最后一个区间
                    g.fileCount = lastRangeCount;
                }

                if ( g.fileCount > 0 ) {
                    gradients_.push_back( g );
                }
            }

            // 修正确保总数正确
            int sum = 0;
            for ( const auto &g : gradients_ ) sum += g.fileCount;
            if ( sum < totalFiles_ && !gradients_.empty() ) {
                gradients_.back().fileCount += ( totalFiles_ - sum );
            }
        }

    public:
        /**
         * 获取第index个文件的数据规模（index从1开始）
         * @param index     文件索引（1-based）
         * @param randomize 是否在子区间内随机化
         * @param opt       随机化时的偏向参数
         */
        long long getScale( int index , bool randomize = false , double opt = 1.0 ) const {
            int cumulative = 0;

            for ( const auto &g : gradients_ ) {
                if ( index <= cumulative + g.fileCount ) {
                    int pos = index - cumulative; // 区间内位置 (1-based)

                    // 计算子区间边界
                    long long rangeSize = g.maxVal - g.minVal;
                    long long subSize = rangeSize / g.fileCount;
                    long long subMin = g.minVal + ( pos - 1 ) * subSize;
                    long long subMax = ( pos == g.fileCount ) ? g.maxVal : ( subMin + subSize );

                    if ( randomize ) {
                        return AMPLE_RAND::random( subMin , subMax , opt );
                    } else {
                        // 确定性：取子区间中点或线性插值
                        return ( subMin + subMax ) / 2;
                    }
                }
                cumulative += g.fileCount;
            }

            return maxScale_;
        }

        /**
         * 获取第index个文件所属的梯度区间
         */
        pair < long long , long long > getGradientRange( int index ) const {
            int cumulative = 0;
            for ( const auto &g : gradients_ ) {
                if ( index <= cumulative + g.fileCount ) {
                    return { g.minVal , g.maxVal };
                }
                cumulative += g.fileCount;
            }
            return { 1 , maxScale_ };
        }

        /**
         * 打印梯度配置信息
         */
        void printConfig() const {
            cout << "================== 梯度配置 ==================\n";
            cout << "最大规模: " << maxScale_ << " | 总文件数: " << totalFiles_ << "\n\n";

            int fileStart = 1;
            for ( size_t i = 0 ; i < gradients_.size() ; i ++ ) {
                const auto &g = gradients_[i];
                int fileEnd = fileStart + g.fileCount - 1;

                printf( "区间 %zu: 文件 %2d - %2d  =>  规模 [%7lld, %7lld]  (%d 个文件)\n" ,
                        i + 1 , fileStart , fileEnd , g.minVal , g.maxVal , g.fileCount );

                fileStart = fileEnd + 1;
            }
            cout << "==============================================\n";
        }

        int getTotalFiles() const { return totalFiles_; }
        long long getMaxScale() const { return maxScale_; }
        const vector < GradientRange > &getGradients() const { return gradients_; }
};

using AMPLE_RAND::random;
using AMPLE_RAND::randomreal;
using AMPLE_RAND::RandomBasic;
using AMPLE_RAND::RandomPrint;
using AMPLE_RAND::RandomAnother;
using AMPLE_RAND::RandomGraphTool;
using AMPLE_RAND::RandomTree;
using AMPLE_RAND::RandomGraph;
using AMPLE_RAND::RandomHackSpfa;
using AMPLE_RAND::RandomDag;
using AMPLE_RAND::RandomScc;
using AMPLE_RAND::RandomEdcc;
using AMPLE_RAND::RandomVdcc;

using FESDRER_CHECK::CheckTree;
using FESDRER_CHECK::CheckScc;
using FESDRER_CHECK::CheckEdcc;
using FESDRER_CHECK::CheckVdcc;

#endif //CLION_DATAGENERATOR_H
