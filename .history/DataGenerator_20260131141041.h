/**
@Author Ample
    @Time 2024/7/11 15:05
    @Description: OI/ACM 题目数据生成头文件
*/

#ifndef CLION_DATAGENERATOR_H
#define CLION_DATAGENERATOR_H

#include <bits/stdc++.h>

namespace AMPLE_RAND {
    // 声明：告诉编译器这个变量在别处定义了
    extern std::mt19937_64 rng_64;

    /**
     * @brief 随机整数生成函数
     * @param l 左区间端点
     * @param r 右区间端点
     * @param opt 偏移因子
     * opt > 1 → 偏向 r;
     * opt < 1 → 偏向 l;
     * opt = 1 → 均匀分布
     * @return 随机整数
     */
    long long random( long long l , long long r , double opt = 1.0 );

    /**
     * @brief 随机实数生成函数
     * @param l 左区间端点
     * @param r 右区间端点
     * @param opt 偏移因子
     * opt > 1 → 偏向 r;
     * opt < 1 → 偏向 l;
     * opt = 1 → 均匀分布
     * @return 随机实数
     */
    double randomDouble( double l , double r , double opt = 1.0 );

    /**
     * @brief 随机字符串生成器
     * @param n 字符串长度
     * @param chars 随机字符串内容
     * @param allowZero 是否允许字符串中出现前导零 默认不允许
     * @return 随机字符串
     */
    std::string randomString( int n , std::string chars , bool allowZero = false );

    /**
     * @brief 随机字符串生成器
     * @param chars 字符串数组内容
     * @return 字符串数组中随机字符串
     */
    std::string randomString( std::vector < std::string > chars );

    /**
     * 随机基础函数类
     */
    class RANDOMBASIC {
        public:
            /**
             * @brief 随机打乱 [first, last) 范围内的元素
             * @tparam RandomIt 随机访问迭代器类型
             * @tparam URBG 均匀随机数生成器类型
             * @param first 起始迭代器
             * @param last 结束迭代器
             * @param rng_64 随机数引擎
             *
             * 时间复杂度: O(n)
             * 空间复杂度: O(1)
             */
            template <
                typename RandomIt ,
                typename URBG ,
                typename = std::enable_if_t <
                    std::is_same_v <
                        typename std::iterator_traits < RandomIt >::iterator_category ,
                        std::random_access_iterator_tag
                    >
                > > void shuffle( RandomIt first , RandomIt last , URBG &&rng_64 ) {
                using diff_t = typename std::iterator_traits < RandomIt >::difference_type;
                const diff_t n = last - first;
                // 边界情况：0 或 1 个元素无需打乱
                if ( n <= 1 ) return;
                // Fisher-Yates 算法（从后向前）
                // 不变量：[i+1, n) 范围内的元素已经确定
                for ( diff_t i = n - 1 ; i > 0 ; --i ) {
                    // 在 [0, i] 中均匀随机选择
                    std::uniform_int_distribution < diff_t > dist( 0 , i );
                    const diff_t j = dist( rng_64 );
                    // 避免自交换（微优化）
                    if ( i != j ) {
                        std::swap( first[i] , first[j] );
                    }
                }
            }

            // ═══════════════════════════════════════════════════════════════
            // 重载 1：迭代器版本（使用默认引擎）
            // ═══════════════════════════════════════════════════════════════
            template <
                typename RandomIt ,
                typename = std::enable_if_t <
                    std::is_same_v <
                        typename std::iterator_traits < RandomIt >::iterator_category ,
                        std::random_access_iterator_tag
                    >
                > > void shuffle( RandomIt first , RandomIt last ) {
                shuffle( first , last , rng_64 );
            }

            // ═══════════════════════════════════════════════════════════════
            // 重载 2：容器版本（vector, deque, string 等）
            // ═══════════════════════════════════════════════════════════════
            template <
                typename Container ,
                typename URBG ,
                typename = decltype(std::declval < Container & >().begin()) ,
                typename = decltype(std::declval < Container & >().end()) > void
            shuffle( Container &c , URBG &&rng_64 ) {
                shuffle( c.begin() , c.end() , std::forward < URBG >( rng_64 ) );
            }

            template <
                typename Container ,
                typename = decltype(std::declval < Container & >().begin()) ,
                typename = decltype(std::declval < Container & >().end()) > void shuffle( Container &c ) {
                shuffle( c.begin() , c.end() , rng_64 );
            }

            // ═══════════════════════════════════════════════════════════════
            // 重载 3：C 风格静态数组
            // ═══════════════════════════════════════════════════════════════
            template < typename T , std::size_t N , typename URBG > void shuffle( T ( &arr )[N] , URBG &&rng_64 ) {
                shuffle( std::begin( arr ) , std::end( arr ) , std::forward < URBG >( rng_64 ) );
            }

            template < typename T , std::size_t N > void shuffle( T ( &arr )[N] ) {
                shuffle( std::begin( arr ) , std::end( arr ) , rng_64 );
            }

            // ═══════════════════════════════════════════════════════════════
            // 重载 4：std::array 显式特化（提高类型推导优先级）
            // ═══════════════════════════════════════════════════════════════
            template < typename T , std::size_t N , typename URBG > void shuffle(
                std::array < T , N > &arr , URBG &&rng_64 ) {
                shuffle( arr.begin() , arr.end() , std::forward < URBG >( rng_64 ) );
            }

            template < typename T , std::size_t N > void shuffle( std::array < T , N > &arr ) {
                shuffle( arr.begin() , arr.end() , rng_64 );
            }

            // ═══════════════════════════════════════════════════════════════
            // 高级版本：返回打乱后的副本（不修改原数组）
            // ═══════════════════════════════════════════════════════════════
            template < typename Container > Container shuffled( Container c ) {
                // 注意：按值传递，自动拷贝
                shuffle( c );
                return c;
            }

            template < typename T , std::size_t N > std::array < T , N > shuffled( const std::array < T , N > &arr ) {
                std::array < T , N > result = arr;
                shuffle( result );
                return result;
            }
    };

    static RANDOMBASIC RandomBasic;

    /**
     * 随机生成树类
     */
    class RANDOMTREE {
        public:
            /**
             * @brief 生成随机树
             *
             * @param n             节点数量 (>= 1)
             * @param base          节点偏移量，默认是 1
             * @param root          根节点编号 [0, n-1]
             * @param rho           深度控制参数 (> 0)
             *                      - rho → 0:  星形树（最浅）
             *                      - rho = 1:  均匀随机树，期望深度 O(log n)
             *                      - rho = 2:  较深的树（默认）
             *                      - rho → ∞:  链（最深）
             * @param vertices_rand 是否随机打乱节点编号
             *                      - false: 保持 父节点编号 < 子节点编号
             *                               （此时 root 必须为 0）
             *                      - true:  随机打乱编号，但 root 保持不变
             *
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_tree( int n , int base = 1 , int root = 0 ,
                                                                 double rho = 2.0 , bool vertices_rand = true );

            /**
             * @brief 生成随机树
             *
             * @param n             节点数量 (>= 1)
             * @param base          节点偏移量，默认是 1
             * @param root          根节点编号 [0, n-1]
             * @param rho           深度控制参数 (> 0)
             *                      - rho → 0:  星形树（最浅）
             *                      - rho = 1:  均匀随机树，期望深度 O(log n)
             *                      - rho = 2:  较深的树（默认）
             *                      - rho → ∞:  链（最深）
             * @param vertices_rand 是否随机打乱节点编号
             *                      - false: 保持 父节点编号 < 子节点编号
             *                               （此时 root 必须为 0）
             *                      - true:  随机打乱编号，但 root 保持不变
             *
             * @return vector< int > 父亲数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < int > random_tree_parent( int n , int base = 1 , int root = 0 , double rho = 2.0 ,
                                                    bool vertices_rand = true );

            /**
             * @brief 随机生成树链
             * @param n 节点数量 (>= 1)
             * @param base 节点偏移量，默认是 1
             * @param root 根节点编号 [0, n-1]
             * @param vertices_rand 是否随机打乱节点编号
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_tree_chain( int n , int base = 1 , int root = 0 ,
                                                                       bool vertices_rand = true );

            /**
             * @brief       随机生成星树（菊花）
             * @param n     节点数量 (>= 1)
             * @param base  节点偏移量，默认是 1
             * @param root  根节点编号 [0, n-1]
             * @param vertices_rand     是否随机打乱节点编号
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_tree_star( int n , int base = 1 , int root = 0 ,
                                                                      bool vertices_rand = true );

            /**
             * @brief       生成随机二叉树
             * @param n     节点数量 (>= 1)
             * @param base  节点偏移量，默认是 1
             * @param root  根节点编号 [0, n-1]
             * @param rho   深度控制参数（同随机树）
             *                      - rho → 0:  浅而宽
             *                      - rho = 1:  均匀随机
             *                      - rho → ∞:  深而窄（趋向链）
             * @param vertices_rand 是否随机打乱节点编号
             * @param balance       平衡度控制 [0, 1]
             *                      - 0: 完全随机选择左/右
             *                      - 1: 尽量平衡（优先填充子节点少的方向）
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_binary_tree( int n , int base = 1 , int root = 0 ,
                                                                        double rho = 1.0 , bool vertices_rand = true ,
                                                                        int balance = 0 );

            /**
             * @brief 生成随机森林
             * @param n 节点数量 (>= 1)
             * @param num_trees 树数量 [1,n]
             * @param base 节点偏移量，默认是 1
             * @param rho 树深度控制参数（同随机树）
             * @param vertices_rand 是否随机打乱节点编号
             * @param size_variance 树大小的方差控制 [0, 1]
             *                      - 0: 所有树大小尽量相等
             *                      - 1: 树大小完全随机
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_forest_tree( int n , int num_trees , int base = 1 ,
                                                                        double rho = 1.0 , bool vertices_rand = true ,
                                                                        double size_variance = 1.0 );
    };

    static RANDOMTREE RandomTree;

    /**
     * 随机生成图类
     */
    class RANDOMGRAPH {
        private:
            struct Edge {
                int u , v;

                bool operator==( const Edge &other ) const {
                    return u == other.u && v == other.v;
                }

                bool operator<( const Edge &other ) const {
                    return u < other.u || ( u == other.u && v < other.v );
                }
            };

            /**
             * @brief 计算节点最小边数
             * @param n 节点数量
             * @param connected 是否连通
             * @param bool 省略参数，图的有向性
             * @return 节点最小边数
             */
            static int compute_min_edges( int n , bool connected , bool /*directional*/ ) {
                if ( !connected ) return 0;
                if ( n <= 1 ) return 0;
                return n - 1; // 生成树
            }

            /**
             * @brief 计算节点最大边数
             * @param n 节点数量
             * @param repeated_edges 是否重边
             * @param self_rings 是否自环
             * @param directional 是否有向
             * @return 节点最大边数
             */
            static long long compute_max_edges( int n , bool repeated_edges , bool self_rings , bool directional ) {
                if ( n <= 0 ) return 0;
                if ( repeated_edges ) {
                    // 允许重边时，设置一个实际上限
                    return static_cast < long long >( n ) * n * 10;
                }
                long long base;
                if ( directional ) {
                    base = static_cast < long long >( n ) * ( n - 1 ); // 有向：n(n-1)
                } else {
                    base = static_cast < long long >( n ) * ( n - 1 ) / 2; // 无向：n(n-1)/2
                }
                if ( self_rings ) {
                    base += n; // 加上 n 个自环
                }
                return base;
            }

        public:
            /**
             * 随机生成图（默认无向连通图（无重边、无自环））{点,边} 校验生成
             * @param n 期望的节点数 (必须 > 0)
             * @param m 期望的边数 (必须 >= 0)
             * @param connected 是否要求连通（无向图为连通，有向图为弱连通）
             * @param repeated_edges 是否允许重边
             * @param self_rings 是否允许自环
             * @param directional 是否为有向图
             * @param opt 接近度参数 ∈ (0, +∞)
             *                        - opt → 0:   返回值可能在合法范围内随机偏移
             *                        - opt → ∞:   返回值尽可能接近输入的 (n, m)
             *                        - opt = 1:   中等程度的随机性
             *                        - opt = 10:  较小的随机波动（推荐默认值）
             * @throws std::invalid_argument 如果参数完全不可行
             * @return std::pair<int, int> {点,边}
             */
            std::pair < int , int > valid_nm( int n , int m , bool connected = true , bool repeated_edges = false ,
                                              bool self_rings = false ,
                                              bool directional = false , double opt = 10.0 );

            /**
             * @brief 生成随机图（默认无向连通图（无重边、无自环））
             * @param n 节点数量
             * @param m 边数
             * @param connected 是否连通
             * @param repeated_edges 是否允许重边
             * @param self_rings 是否允许自环
             * @param directional 是否为有向图
             * @param base 节点偏移量，默认是 1
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_graph( int n , int m , bool connected = true ,
                                                                  bool repeated_edges = false ,
                                                                  bool self_rings = false ,
                                                                  bool directional = false ,
                                                                  int base = 1 );

            /**
             * @brief 生成二分图
             * @param n1 第一部分节点数
             * @param n2 第二部分节点数
             * @param m 边数
             * @param base 节点偏移量，默认是 1
             * @param directional 是否为有向图
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_binary_graph(
                int n1 , int n2 , int m , bool directional = false , int base = 1 );

            /**
             * @brief 生成 DAG（有向无环图）
             * @param n 节点数量
             * @param m 边数
             * @param base 节点偏移量，默认是 1
             * @return vector< pair< int, int > > 边集数组
             *
             * @note 节点编号从 1 开始，即 [1, n]
             */
            std::vector < std::pair < int , int > > random_dag_graph(
                int n , int m , int base = 1 );
    };

    static RANDOMGRAPH RandomGraph;

    /**
     * @brief 快速幂
     * @param x 底数
     * @param pow 指数
     * @param p 模数（模数为 0 时不取模）
     * @return x^pow % p
     */
    long long QuickPow( long long x , long long pow , long long p = 0 );

    /**
     * @brief 拓展欧几里得算法求解最大公约数 gcd(a,b)
     * @param a
     * @param b
     * @param x
     * @param y
     * @return 最大公约数 d
     */
    long long exgcd( long long a , long long b , long long &x , long long &y );

    /**
     * @brief 打开输入文件流
     * @param str 文件名
     * @return
     */
    std::ifstream openInputFileStream( const std::string &str );

    /**
     * @brief 打开输出文件流
     * @param str 文件名
     * @return
     */
    std::ofstream openOutputFileStream( const std::string &str );

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
                int m = ( int ) ( ( c.get_pos( ( int ) e.v.size() ) * 10000 + c.get_pos( ( int ) e.v.size() - 1 ) ) *
                                  db );
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
     * @brief 梯度配置生成器
     * @details 梯度配置生成器
    */
    class GradientScaleGenerator {
        public:
            struct GradientRange {
                long long minVal; // 区间最小值
                long long maxVal; // 区间最大值
                int fileCount; // 分配的文件数量
            };

        private:
            std::vector < GradientRange > gradients_;
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
            GradientScaleGenerator( const std::vector < GradientRange > &customGradients ) : gradients_(
                customGradients ) {
                totalFiles_ = 0;
                maxScale_ = 0;
                for ( const auto &g : gradients_ ) {
                    totalFiles_ += g.fileCount;
                    maxScale_ = std::max( maxScale_ , g.maxVal );
                }
            }

        private:
            void autoGenerateGradients() {
                gradients_.clear();

                // Step 1: 生成10的幂次边界点 [1, 10, 100, 1000, ...]
                std::vector < long long > boundaries = { 1 };
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
                int lastRangeCount = std::max( 1 , totalFiles_ / 4 );
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
            std::pair < long long , long long > getGradientRange( int index ) const {
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
                std::cout << "================== 梯度配置 ==================\n";
                std::cout << "最大规模: " << maxScale_ << " | 总文件数: " << totalFiles_ << "\n\n";

                int fileStart = 1;
                for ( size_t i = 0 ; i < gradients_.size() ; i ++ ) {
                    const auto &g = gradients_[i];
                    int fileEnd = fileStart + g.fileCount - 1;

                    printf( "区间 %zu: 文件 %2d - %2d  =>  规模 [%7lld, %7lld]  (%d 个文件)\n" ,
                            i + 1 , fileStart , fileEnd , g.minVal , g.maxVal , g.fileCount );

                    fileStart = fileEnd + 1;
                }
                std::cout << "==============================================\n";
            }

            int getTotalFiles() const { return totalFiles_; }
            long long getMaxScale() const { return maxScale_; }
            const std::vector < GradientRange > &getGradients() const { return gradients_; }
    };

    /*
     *  梯度递增策略
     *  LINEAR      - 线性递增：均匀分布
     *  EXPONENTIAL - 指数递增：前期慢后期快（适合测试大数据边界）
     *  LOGARITHMIC - 对数递增：前期快后期慢（适合测试小数据细节）
     *  QUADRATIC   - 二次递增：加速递增
     *  SQRT        - 平方根递增：减速递增
     *  SIGMOID     - S 型曲线：两端平缓中间陡峭
     *  UNIFORM     - 所有文件都是均匀分布（opt = 1）
     */
    enum class GradientStrategy {
        LINEAR , // 线性递增：均匀分布
        EXPONENTIAL , // 指数递增：前期慢后期快（适合测试大数据边界）
        LOGARITHMIC , // 对数递增：前期快后期慢（适合测试小数据细节）
        QUADRATIC , // 二次递增：加速递增
        SQRT , // 平方根递增：减速递增
        SIGMOID , // S 型曲线：两端平缓中间陡峭
        UNIFORM // 所有文件都是均匀分布（opt = 1）
    };

    /*
     * @brief 梯度参数结果结构体
     */
    struct GradientOptResult {
        double opt; // 影响因子，用于 random(l, r, opt)
        double target_position; // 目标位置（归一化 0-1）
        double expected_median_ratio; // 预期中位数比例（0-1）
        double expected_mean_ratio; // 预期均值比例（0-1）
        int file_id; // 文件编号
        int total_files; // 总文件数

        // 给定范围 [l, r]，计算预期中位数
        template < typename T > T expected_median( T l , T r ) const {
            return static_cast < T >( l + ( r - l ) * expected_median_ratio );
        }

        // 给定范围 [l, r]，计算预期均值
        template < typename T > T expected_mean( T l , T r ) const {
            return static_cast < T >( l + ( r - l ) * expected_mean_ratio );
        }

        // 给定范围 [l, r]，计算预期分布区间（25% - 75% 分位数）
        template < typename T > std::pair < T , T > expected_iqr( T l , T r ) const {
            double q25 = std::pow( 0.25 , 1.0 / opt );
            double q75 = std::pow( 0.75 , 1.0 / opt );
            return {
                static_cast < T >( l + ( r - l ) * q25 ) ,
                static_cast < T >( l + ( r - l ) * q75 )
            };
        }
    };

    /**
     * @brief 计算用于梯度数据生成的 opt 参数
     *
     * @param total_files   总文件数量 (>= 1)
     * @param file_id       当前文件编号
     * @param zero_indexed  编号是否从 0 开始
     *                      - true:  file_id ∈ [0, total_files - 1]
     *                      - false: file_id ∈ [1, total_files]（默认）
     * @param strategy      递增策略（默认 LINEAR）
     * @param sharpness     锐度参数 ∈ (0, +∞)，默认 1.0
     *                      - > 1: 使不同文件间的分布差异更大
     *                      - < 1: 使不同文件间的分布差异更小
     *                      - = 1: 标准梯度
     *
     * @return GradientOptResult 包含 opt 及预期分布信息
     */
    GradientOptResult compute_gradient_opt(
        int total_files ,
        int file_id ,
        bool zero_indexed = false ,
        GradientStrategy strategy = GradientStrategy::LINEAR ,
        double sharpness = 1.0
    );

    /**
     * @brief 直接获取 opt 值（简化版）
     */
    double getGradientOpt(
        int total_files ,
        int file_id ,
        bool zero_indexed = true ,
        GradientStrategy strategy = GradientStrategy::LINEAR ,
        double sharpness = 1.0 );

    /**
     * @brief 批量获取所有文件的 opt 数组
     */
    std::vector < double > getAllGradientOpts(
        int total_files ,
        GradientStrategy strategy = GradientStrategy::LINEAR ,
        double sharpness = 1.0 );

    /**
     * @brief 多文件多组数据生成
     * @param startIndex 文件起始编号
     * @param endIndex 文件结束编号
     * @param Limit 数据规模上限
     * @param solve 数据生成逻辑代码
     * @param ins 输入文件流
     * @param ous 输出文件流
     * @param multiple 多组数据规模上限
     * @param strategy 递增策略
     * @return 是否生成成功
     */
    bool batchGenerateFiles( int startIndex , int endIndex ,
                             std::function < void( long long , long long , double ) > solve ,
                             long long Limit ,
                             std::stringstream &ins ,
                             std::stringstream &ous ,
                             int multiple = 1 ,
                             GradientStrategy strategy = GradientStrategy::SIGMOID );
}

using AMPLE_RAND::random;
using AMPLE_RAND::randomDouble;
using AMPLE_RAND::randomString;
using AMPLE_RAND::RandomBasic;
using AMPLE_RAND::RandomTree;
using AMPLE_RAND::RandomGraph;
using AMPLE_RAND::QuickPow;
using AMPLE_RAND::exgcd;
using AMPLE_RAND::openInputFileStream;
using AMPLE_RAND::openOutputFileStream;
using AMPLE_RAND::BigIntTiny;
using AMPLE_RAND::MemUtil;
using AMPLE_RAND::GradientStrategy;
using AMPLE_RAND::getGradientOpt;
using AMPLE_RAND::getAllGradientOpts;
using AMPLE_RAND::batchGenerateFiles;

// 数据文件地址
const std::string dataFilePath = "D:\\Desktop\\data\\";

// 输入输出文件类型
const std::string inFileType = ".in";
const std::string outFileType = ".out";

// 字符集
const std::string charNumber = "0123456789";
const std::string charLowerCase = "abcdefghijklmnopqrstuvwxyz";
const std::string charUpperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// 数据结构简写
using vvi = std::vector < std::vector < long long > >;
using vvI = std::vector < std::vector < signed > >;
using vi = std::vector < long long >;
using vI = std::vector < signed >;
using vs = std::vector < std::string >;
using vpii = std::vector < std::pair < long long , long long > >;
using vpII = std::vector < std::pair < signed , signed > >;
using vb = std::vector < bool >;
using vvb = std::vector < vb >;
using vc = std::vector < char >;
using PII = std::pair < long long , long long >;
using PDD = std::pair < double , double >;
using PPI = std::pair < std::pair < long long , long long > , long long >;

// 常用数据
const long long INF = 0X7FFFFFFF;
const long long MAX = 0x3f3f3f3f3f3f3f3f;
const long long mod = 1e9 + 7;
const long long MOD = 998244353;

#endif // CLION_DATAGENERATOR_H
