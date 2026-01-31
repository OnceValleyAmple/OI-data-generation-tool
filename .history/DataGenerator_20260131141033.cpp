/**
    @Author Ample
    @Time 2024/7/11 15:05
    @Description: OI/ACM 题目数据生成头文件
*/

#include "DataGenerator.h"

namespace AMPLE_RAND {
    // 1. 在这里真正定义随机数引擎（只定义一次）
    std::mt19937_64 rng_64( std::chrono::steady_clock::now().time_since_epoch().count() );

    long long random( long long l , long long r , double opt ) {
        // 参数校验
        if ( l > r ) std::swap( l , r );
        if ( l == r ) return l;
        // opt 必须为正
        if ( opt <= 0.0 ) {
            throw std::invalid_argument( "opt must be positive" );
        }
        // opt ≈ 1 时走均匀分布快速路径
        if ( std::fabs( opt - 1.0 ) <= 1e-8 ) {
            return std::uniform_int_distribution < long long >( l , r )( rng_64 );
        }
        // 避免 static 带来的线程安全问题
        std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
        double x = dist( rng_64 );
        // 处理 x = 0 的边界（虽然罕见，但 0^正数 = 0，无问题）
        // x ∈ [0, 1)，y = x^(1/opt) ∈ [0, 1)
        double y = std::pow( x , 1.0 / opt );
        // 使用 unsigned 避免大区间溢出
        unsigned long long range = static_cast < unsigned long long >( r ) -
                                   static_cast < unsigned long long >( l );
        // 核心映射：y * range ∈ [0, range)，结果 ∈ [l, r]
        long long res = l + static_cast < long long >(
                            static_cast < long double >( range ) * static_cast < long double >( y )
                        );
        // 双边界保护
        res = std::clamp( res , l , r );
        return res;
    }

    double randomDouble( double l , double r , double opt ) {
        if ( l > r ) std::swap( l , r );
        if ( l == r ) return l;
        if ( opt <= 0.0 ) throw std::invalid_argument( "opt must be positive" );
        thread_local std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
        double x = dist( rng_64 );
        // 简单幂变换：只需一次 pow
        // opt > 1 → 偏向 r
        // opt < 1 → 偏向 l
        double y = std::pow( x , 1.0 / opt );

        return l + ( r - l ) * y;
    }

    std::string randomString( int n , std::string chars , bool allowZero ) {
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

    std::string randomString( std::vector < std::string > chars ) {
        int n = chars.size();
        return chars[random( 0 , n - 1 )];
    }

    std::vector < std::pair < int , int > > RANDOMTREE::random_tree( int n , int base , int root , double rho ,
                                                                     bool vertices_rand ) {
        /*
        ┌────────────────────────────────────────────────────────────┐
        │                     rho 参数效果                             │
        ├────────────┬───────────────────────────────────────────────┤
        │   rho      │                  树的形态                       │
        ├────────────┼───────────────────────────────────────────────┤
        │   0.01     │  ★────★────★────★  (星形，深度 1)              │
        │   0.5      │  较浅的树，分支较多                              │
        │   1.0      │  均匀随机树，期望深度 O(log n)                   │
        │   2.0      │  较深的树（默认值）                              │
        │   5.0      │  深树，少量分支                                 │
        │   100+     │  ★─★─★─★─★─★─★  (链，深度 n-1)               │
        └────────────┴───────────────────────────────────────────────
        */
        if ( n <= 0 ) {
            throw std::invalid_argument( "n must be positive" );
        }
        if ( root < 0 || root >= n ) {
            throw std::invalid_argument( "root must be in range [0, n-1]" );
        }
        if ( rho <= 0.0 ) {
            throw std::invalid_argument( "rho must be positive" );
        }
        if ( !vertices_rand && root != 0 ) {
            throw std::invalid_argument(
                "When vertices_rand=false, root must be 0 "
                "to maintain parent < child ordering"
            );
        }
        if ( n == 1 ) {
            return std::vector < std::pair < int , int > >{};
        }
        // ═══════════════════════════════════════════════════════════════════
        // Step 1: 生成基础树结构
        //         节点 0 为临时根，节点 i 的父节点在 [0, i-1] 中
        // ═══════════════════════════════════════════════════════════════════
        std::vector < int > base_parent( n , -1 );
        std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
        // 是否接近均匀分布
        const bool is_uniform = std::abs( rho - 1.0 ) < 1e-9;
        for ( int i = 1 ; i < n ; ++i ) {
            int p;
            if ( is_uniform ) {
                // 均匀随机选择父节点
                std::uniform_int_distribution < int > parent_dist( 0 , i - 1 );
                p = parent_dist( rng_64 );
            } else {
                // 使用幂变换控制深度
                double x = dist( rng_64 );
                // rho 越大，x^(1/rho) 越接近 1，p 越接近 i-1
                double y = std::pow( x , 1.0 / rho );
                p = static_cast < int >( y * i );
                p = std::clamp( p , 0 , i - 1 );
            }
            base_parent[i] = p;
        }
        std::vector < int > parent( n , -1 );
        // ═══════════════════════════════════════════════════════════════════
        // Step 2: 处理节点编号
        // ═══════════════════════════════════════════════════════════════════
        if ( !vertices_rand ) {
            // 不打乱：直接使用原始编号
            // 性质：对于所有边 (u, v)，若 u 是 v 的父亲，则 u < v
            parent = std::move( base_parent );
        } else {
            // 打乱：随机重排节点编号，但保持 root 不变
            // 创建排列: perm[原始编号] = 新编号
            std::vector < int > perm( n );
            std::iota( perm.begin() , perm.end() , 0 );
            // Fisher-Yates 打乱
            for ( int i = n - 1 ; i > 0 ; --i ) {
                std::uniform_int_distribution < int > d( 0 , i );
                int j = d( rng_64 );
                std::swap( perm[i] , perm[j] );
            }
            // 确保原始根 0 映射到目标根 root
            // 找到 perm 中 root 的位置
            int root_pos = -1;
            for ( int i = 0 ; i < n ; ++i ) {
                if ( perm[i] == root ) {
                    root_pos = i;
                    break;
                }
            }
            std::swap( perm[0] , perm[root_pos] );
            // 现在 perm[0] = root，即原始根 0 映射到新根 root
            // 应用排列：new_parent[perm[i]] = perm[base_parent[i]]
            for ( int i = 0 ; i < n ; ++i ) {
                int new_id = perm[i];
                if ( base_parent[i] == -1 ) {
                    parent[new_id] = -1;
                } else {
                    parent[new_id] = perm[base_parent[i]];
                }
            }
        }
        // ═══════════════════════════════════════════════════════════════════
        // Step 3: 构建边列表、子节点列表、邻接表
        // ═══════════════════════════════════════════════════════════════════
        std::vector < std::pair < int , int > > edges;
        for ( int v = 0 ; v < n ; ++v ) {
            int p = parent[v];
            if ( p != -1 ) {
                edges.emplace_back( p + base , v + base );
            }
        }
        return edges;
    }

    std::vector < int >
    RANDOMTREE::random_tree_parent( int n , int base , int root , double rho , bool vertices_rand ) {
        /*
        ┌────────────────────────────────────────────────────────────┐
        │                     rho 参数效果                             │
        ├────────────┬───────────────────────────────────────────────┤
        │   rho      │                  树的形态                       │
        ├────────────┼───────────────────────────────────────────────┤
        │   0.01     │  ★────★────★────★  (星形，深度 1)              │
        │   0.5      │  较浅的树，分支较多                              │
        │   1.0      │  均匀随机树，期望深度 O(log n)                   │
        │   2.0      │  较深的树（默认值）                              │
        │   5.0      │  深树，少量分支                                 │
        │   100+     │  ★─★─★─★─★─★─★  (链，深度 n-1)               │
        └────────────┴───────────────────────────────────────────────
        */
        if ( n <= 0 ) {
            throw std::invalid_argument( "n must be positive" );
        }
        if ( root < 0 || root >= n ) {
            throw std::invalid_argument( "root must be in range [0, n-1]" );
        }
        if ( rho <= 0.0 ) {
            throw std::invalid_argument( "rho must be positive" );
        }
        if ( !vertices_rand && root != 0 ) {
            throw std::invalid_argument(
                "When vertices_rand=false, root must be 0 "
                "to maintain parent < child ordering"
            );
        }
        if ( n == 1 ) {
            return std::vector < int >( n , -1 );
        }
        // ═══════════════════════════════════════════════════════════════════
        // Step 1: 生成基础树结构
        //         节点 0 为临时根，节点 i 的父节点在 [0, i-1] 中
        // ═══════════════════════════════════════════════════════════════════
        std::vector < int > base_parent( n , -1 );
        std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
        // 是否接近均匀分布
        const bool is_uniform = std::abs( rho - 1.0 ) < 1e-9;
        for ( int i = 1 ; i < n ; ++i ) {
            int p;
            if ( is_uniform ) {
                // 均匀随机选择父节点
                std::uniform_int_distribution < int > parent_dist( 0 , i - 1 );
                p = parent_dist( rng_64 );
            } else {
                // 使用幂变换控制深度
                double x = dist( rng_64 );
                // rho 越大，x^(1/rho) 越接近 1，p 越接近 i-1
                double y = std::pow( x , 1.0 / rho );
                p = static_cast < int >( y * i );
                p = std::clamp( p , 0 , i - 1 );
            }
            base_parent[i] = p;
        }
        std::vector < int > parent( n , -1 );
        // ═══════════════════════════════════════════════════════════════════
        // Step 2: 处理节点编号
        // ═══════════════════════════════════════════════════════════════════
        if ( !vertices_rand ) {
            // 不打乱：直接使用原始编号
            // 性质：对于所有边 (u, v)，若 u 是 v 的父亲，则 u < v
            parent = std::move( base_parent );
        } else {
            // 打乱：随机重排节点编号，但保持 root 不变
            // 创建排列: perm[原始编号] = 新编号
            std::vector < int > perm( n );
            std::iota( perm.begin() , perm.end() , 0 );
            // Fisher-Yates 打乱
            for ( int i = n - 1 ; i > 0 ; --i ) {
                std::uniform_int_distribution < int > d( 0 , i );
                int j = d( rng_64 );
                std::swap( perm[i] , perm[j] );
            }
            // 确保原始根 0 映射到目标根 root
            // 找到 perm 中 root 的位置
            int root_pos = -1;
            for ( int i = 0 ; i < n ; ++i ) {
                if ( perm[i] == root ) {
                    root_pos = i;
                    break;
                }
            }
            std::swap( perm[0] , perm[root_pos] );
            // 现在 perm[0] = root，即原始根 0 映射到新根 root
            // 应用排列：new_parent[perm[i]] = perm[base_parent[i]]
            for ( int i = 0 ; i < n ; ++i ) {
                int new_id = perm[i];
                if ( base_parent[i] == -1 ) {
                    parent[new_id] = -1;
                } else {
                    parent[new_id] = perm[base_parent[i]];
                }
            }
            for ( auto &p : parent ) {
                if ( p == -1 ) continue;
                p += base;
            }
        }
        return parent;
    }

    std::vector < std::pair < int , int > > RANDOMTREE::random_tree_chain(
        int n , int base , int root , bool vertices_rand ) {
        return random_tree( n , base , root , 1e9 , vertices_rand );
    }

    std::vector < std::pair < int , int > > RANDOMTREE::random_tree_star(
        int n , int base , int root , bool vertices_rand ) {
        return random_tree( n , base , root , 1e-9 , vertices_rand );
    }

    std::vector < std::pair < int , int > > RANDOMTREE::random_binary_tree(
        int n , int base , int root , double rho , bool vertices_rand , int balance ) {
        if ( n <= 0 ) throw std::invalid_argument( "n must be positive" );
        if ( root < 0 || root >= n ) throw std::invalid_argument( "root out of range" );
        if ( rho <= 0.0 ) throw std::invalid_argument( "rho must be positive" );
        if ( balance < 0.0 || balance > 1.0 ) throw std::invalid_argument( "balance must be in [0, 1]" );
        if ( !vertices_rand && root != 0 ) {
            throw std::invalid_argument( "root must be 0 when vertices_rand=false" );
        }

        // ═══════════════════════════════════════════════════════════════════
        // 槽位管理：slot = (节点编号, 是否为左槽位, 深度)
        // ═══════════════════════════════════════════════════════════════════
        struct Slot {
            int node;
            bool is_left;
            int depth;
        };
        std::vector < int > parent( n , -1 );
        std::vector < std::pair < int , int > > edges;
        std::vector < Slot > available_slots;
        std::vector < int > base_parent( n , -1 );
        std::vector < int > base_left( n , -1 );
        std::vector < int > base_right( n , -1 );
        std::vector < int > node_depth( n , 0 );
        // 根节点有两个可用槽位
        available_slots.push_back( { 0 , true , 1 } ); // 左槽位
        available_slots.push_back( { 0 , false , 1 } ); // 右槽位
        std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
        const bool is_uniform = std::abs( rho - 1.0 ) < 1e-9;
        for ( int i = 1 ; i < n ; ++i ) {
            // ─────────────────────────────────────────────────────────────────
            // Step 1: 选择一个槽位
            // ─────────────────────────────────────────────────────────────────
            int slot_idx;
            int num_slots = available_slots.size();

            if ( is_uniform ) {
                std::uniform_int_distribution < int > slot_dist( 0 , num_slots - 1 );
                slot_idx = slot_dist( rng_64 );
            } else {
                // 按深度排序，rho 越大越倾向选择深的槽位
                // 使用简化的方法：先按深度分组，再用 rho 控制选择深层的概率
                double x = dist( rng_64 );
                double y = std::pow( x , 1.0 / rho );
                slot_idx = static_cast < int >( y * num_slots );
                slot_idx = std::clamp( slot_idx , 0 , num_slots - 1 );

                // 按深度排序槽位（浅 → 深）
                std::sort( available_slots.begin() , available_slots.end() ,
                           []( const Slot &a , const Slot &b ) { return a.depth < b.depth; } );
            }

            Slot selected = available_slots[slot_idx];

            // 从可用槽位中移除
            available_slots.erase( available_slots.begin() + slot_idx );

            // ─────────────────────────────────────────────────────────────────
            // Step 2: 将新节点插入到选中的槽位
            // ─────────────────────────────────────────────────────────────────
            int parent_node = selected.node;
            base_parent[i] = parent_node;
            node_depth[i] = selected.depth;

            if ( selected.is_left ) {
                base_left[parent_node] = i;
            } else {
                base_right[parent_node] = i;
            }
            // ─────────────────────────────────────────────────────────────────
            // Step 3: 添加新节点的槽位
            // ─────────────────────────────────────────────────────────────────
            // 根据 balance 参数决定槽位添加顺序
            if ( dist( rng_64 ) < 0.5 ) {
                available_slots.push_back( { i , true , node_depth[i] + 1 } );
                available_slots.push_back( { i , false , node_depth[i] + 1 } );
            } else {
                available_slots.push_back( { i , false , node_depth[i] + 1 } );
                available_slots.push_back( { i , true , node_depth[i] + 1 } );
            }
        }
        // ═══════════════════════════════════════════════════════════════════
        // 处理节点编号打乱
        // ═══════════════════════════════════════════════════════════════════
        if ( !vertices_rand ) {
            parent = std::move( base_parent );
        } else {
            std::vector < int > perm( n );
            std::iota( perm.begin() , perm.end() , 0 );
            for ( int i = n - 1 ; i > 0 ; --i ) {
                std::uniform_int_distribution < int > d( 0 , i );
                std::swap( perm[i] , perm[d( rng_64 )] );
            }
            // 确保原始根 0 映射到目标根 root
            int root_pos = std::find( perm.begin() , perm.end() , root ) - perm.begin();
            std::swap( perm[0] , perm[root_pos] );

            // perm[old_id] = new_id
            // 需要逆映射：inv_perm[new_id] = old_id
            std::vector < int > inv_perm( n );
            for ( int i = 0 ; i < n ; ++i ) inv_perm[perm[i]] = i;

            auto map_id = [&]( int old_id ) ->int {
                return old_id == -1 ? -1 : perm[old_id];
            };

            for ( int old_id = 0 ; old_id < n ; ++old_id ) {
                int new_id = perm[old_id];
                parent[new_id] = map_id( base_parent[old_id] );
            }
        }
        // 构建边列表
        for ( int v = 0 ; v < n ; ++v ) {
            if ( parent[v] != -1 ) {
                int p = parent[v];
                edges.emplace_back( p + base , v + base );
            }
        }
        return edges;
    }

    std::vector < std::pair < int , int > > RANDOMTREE::random_forest_tree(
        int n , int num_trees , int base , double rho , bool vertices_rand , double size_variance ) {
        if ( n <= 0 ) throw std::invalid_argument( "n must be positive" );
        if ( num_trees <= 0 ) throw std::invalid_argument( "num_trees must be positive" );
        if ( num_trees > n ) throw std::invalid_argument( "num_trees cannot exceed n" );
        if ( rho <= 0.0 ) throw std::invalid_argument( "rho must be positive" );
        if ( size_variance < 0.0 || size_variance > 1.0 ) {
            throw std::invalid_argument( "size_variance must be in [0, 1]" );
        }
        // ═══════════════════════════════════════════════════════════════════
        // Step 1: 分配每棵树的大小
        // ═══════════════════════════════════════════════════════════════════
        std::vector < int > tree_sizes( num_trees , 1 ); // 每棵树至少 1 个节点
        int remaining = n - num_trees;

        if ( remaining > 0 ) {
            if ( size_variance < 1e-9 ) {
                // 尽量均匀分配
                int base = remaining / num_trees;
                int extra = remaining % num_trees;
                for ( int i = 0 ; i < num_trees ; ++i ) {
                    tree_sizes[i] += base + ( i < extra ? 1 : 0 );
                }
            } else {
                // 按方差控制随机分配
                std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
                std::vector < double > weights( num_trees );
                double sum = 0;

                for ( int i = 0 ; i < num_trees ; ++i ) {
                    // 混合均匀分配和随机分配
                    double uniform_part = 1.0 / num_trees;
                    double random_part = dist( rng_64 );
                    weights[i] = ( 1.0 - size_variance ) * uniform_part + size_variance * random_part;
                    sum += weights[i];
                }

                // 归一化并分配
                for ( int i = 0 ; i < num_trees ; ++i ) {
                    int alloc = static_cast < int >( weights[i] / sum * remaining );
                    tree_sizes[i] += alloc;
                }

                // 处理舍入误差
                int allocated = 0;
                for ( int i = 0 ; i < num_trees ; ++i ) allocated += tree_sizes[i];
                int diff = n - allocated;

                for ( int i = 0 ; diff > 0 && i < num_trees ; ++i, --diff ) {
                    ++tree_sizes[i];
                }
            }
        }
        // ═══════════════════════════════════════════════════════════════════
        // Step 2: 为每棵树分配节点编号范围
        // ═══════════════════════════════════════════════════════════════════
        std::vector < int > tree_start( num_trees );
        tree_start[0] = 0;
        for ( int i = 1 ; i < num_trees ; ++i ) {
            tree_start[i] = tree_start[i - 1] + tree_sizes[i - 1];
        }
        // ═══════════════════════════════════════════════════════════════════
        // Step 3: 生成每棵树
        // ═══════════════════════════════════════════════════════════════════
        std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
        const bool is_uniform = std::abs( rho - 1.0 ) < 1e-9;
        std::vector < int > roots( num_trees );
        std::vector < int > tree_id( n );
        std::vector < int > parent( n , -1 );
        std::vector < std::pair < int , int > > edges;
        for ( int t = 0 ; t < num_trees ; ++t ) {
            int start = tree_start[t];
            int sz = tree_sizes[t];
            roots[t] = start;
            tree_id[start] = t;
            parent[start] = -1;
            for ( int i = 1 ; i < sz ; ++i ) {
                int node = start + i;
                tree_id[node] = t;
                int p;
                if ( is_uniform ) {
                    std::uniform_int_distribution < int > pd( 0 , i - 1 );
                    p = pd( rng_64 );
                } else {
                    double x = dist( rng_64 );
                    double y = std::pow( x , 1.0 / rho );
                    p = static_cast < int >( y * i );
                    p = std::clamp( p , 0 , i - 1 );
                }

                int parent_node = start + p;
                parent[node] = parent_node;
                edges.emplace_back( parent_node , node );
            }
        }
        // ═══════════════════════════════════════════════════════════════════
        // Step 4: 可选打乱节点编号
        // ═══════════════════════════════════════════════════════════════════
        if ( vertices_rand ) {
            std::vector < int > perm( n );
            std::iota( perm.begin() , perm.end() , 0 );
            // Fisher-Yates 打乱
            for ( int i = n - 1 ; i > 0 ; --i ) {
                std::uniform_int_distribution < int > d( 0 , i );
                std::swap( perm[i] , perm[d( rng_64 )] );
            }
            // 应用排列
            std::vector < int > new_tree_id( n );
            std::vector < int > new_parent( n );
            std::vector < std::vector < int > > new_children( n );
            std::vector < std::vector < int > > new_adj( n );
            std::vector < std::pair < int , int > > new_edges;
            new_edges.reserve( edges.size() );

            auto map_id = [&]( int old_id ) ->int {
                return old_id == -1 ? -1 : perm[old_id];
            };

            for ( int i = 0 ; i < n ; ++i ) {
                int new_id = perm[i];
                new_tree_id[new_id] = tree_id[i];
                new_parent[new_id] = map_id( parent[i] );
            }
            for ( const auto &[u, v] : edges ) {
                new_edges.emplace_back( perm[u] , perm[v] );
            }
            for ( int t = 0 ; t < num_trees ; ++t ) {
                roots[t] = perm[tree_start[t]];
            }
            tree_id = std::move( new_tree_id );
            parent = std::move( new_parent );
            edges = std::move( new_edges );
        }
        for ( auto &[u,v] : edges ) {
            u += base;
            v += base;
        }
        return edges;
    }

    std::pair < int , int > RANDOMGRAPH::valid_nm( int n , int m , bool connected , bool repeated_edges ,
                                                   bool self_rings , bool directional , double opt ) {
        // ───────────────────────────────────────────────────────────────────
        // 基本参数校验
        // ───────────────────────────────────────────────────────────────────
        if ( n <= 0 ) {
            throw std::invalid_argument(
                "Node count n must be positive, got: " + std::to_string( n )
            );
        }
        if ( m < 0 ) {
            throw std::invalid_argument(
                "Edge count m must be non-negative, got: " + std::to_string( m )
            );
        }
        if ( opt <= 0 ) {
            throw std::invalid_argument(
                "opt must be positive, got: " + std::to_string( opt )
            );
        }
        // ───────────────────────────────────────────────────────────────────
        // 计算边数的合法范围
        // ───────────────────────────────────────────────────────────────────
        int min_edges = compute_min_edges( n , connected , directional );
        long long max_edges = compute_max_edges( n , repeated_edges , self_rings , directional );

        // 实际最大边数不能超过用户输入的 m
        long long effective_max = std::min( max_edges , static_cast < long long >( m ) );
        // ───────────────────────────────────────────────────────────────────
        // 可行性检查
        // ───────────────────────────────────────────────────────────────────
        // 检查：输入的 m 是否满足最小边数要求
        if ( m < min_edges ) {
            std::ostringstream oss;
            oss << "Edge count m=" << m << " is less than minimum required ("
                    << min_edges << ") for "
                    << ( connected ? "connected " : "" )
                    << ( directional ? "directed" : "undirected" ) << " graph with n="
                    << n << " nodes";
            throw std::invalid_argument( oss.str() );
        }
        // 检查：最小边数是否超过最大边数（理论上不应该发生）
        if ( min_edges > effective_max ) {
            std::ostringstream oss;
            oss << "Cannot satisfy constraints: min_edges=" << min_edges
                    << " > effective_max=" << effective_max;
            throw std::invalid_argument( oss.str() );
        }
        // ───────────────────────────────────────────────────────────────────
        // 使用 opt 参数调整返回值
        // ───────────────────────────────────────────────────────────────────
        // 目标值：在 [min_edges, effective_max] 范围内，尽量接近 m
        int target_m = static_cast < int >( std::min( static_cast < long long >( m ) , effective_max ) );
        // 计算调整后的 m
        int adjusted_m;
        if ( min_edges == effective_max ) {
            // 只有一个合法值
            adjusted_m = min_edges;
        } else {
            std::uniform_real_distribution < double > dist( 0.0 , 1.0 );
            // opt 越大，噪声越小
            // noise_scale ∈ (0, 1)，opt → ∞ 时趋近于 0
            double noise_scale = 1.0 / ( 1.0 + opt );
            // 在 [min_edges, effective_max] 范围内添加噪声
            long long range = effective_max - min_edges;
            double noise = noise_scale * range * ( 2.0 * dist( rng_64 ) - 1.0 );
            adjusted_m = static_cast < int >( std::clamp(
                static_cast < long long >( target_m + static_cast < long long >( noise ) ) ,
                static_cast < long long >( min_edges ) ,
                effective_max
            ) );
        }
        // ───────────────────────────────────────────────────────────────────
        // 填充结果
        // ───────────────────────────────────────────────────────────────────
        // n = n;
        // m = adjusted_m;
        // is_valid = true;
        // std::ostringstream oss;
        // oss << "Valid params: n=" << n << ", m=" << m
        //         << " (range: [" << min_edges << ", " << max_edges << "])";
        // std::string message = oss.str();

        return { n , adjusted_m };
    }

    std::vector < std::pair < int , int > > RANDOMGRAPH::random_graph( int n , int m , bool connected ,
                                                                       bool repeated_edges , bool self_rings ,
                                                                       bool directional , int base ) {
        // ───────────────────────────────────────────────────────────────────
        // 基本校验
        // ───────────────────────────────────────────────────────────────────
        if ( n <= 0 ) {
            throw std::invalid_argument( "n must be positive" );
        }
        if ( m < 0 ) {
            throw std::invalid_argument( "m must be non-negative" );
        }
        std::vector < Edge > edges;
        edges.reserve( m );
        if ( m == 0 || n == 0 ) {
            return std::vector < std::pair < int , int > >();
        }
        // ───────────────────────────────────────────────────────────────────
        // 边去重集合（当不允许重边时使用）
        // ───────────────────────────────────────────────────────────────────
        struct PairHash {
            size_t operator()( const std::pair < int , int > &p ) const {
                return std::hash < long long >()(
                    static_cast < long long >( p.first ) * 1000000007LL + p.second
                );
            }
        };
        std::unordered_set < std::pair < int , int > , PairHash > edge_set;
        // ───────────────────────────────────────────────────────────────────
        // 添加边的辅助函数
        // ───────────────────────────────────────────────────────────────────
        auto try_add_edge = [&]( int u , int v ) ->bool {
            // 自环检查
            if ( !self_rings && u == v ) {
                return false;
            }
            // 标准化边（无向图时保证 u <= v）
            std::pair < int , int > e = { u , v };
            if ( !directional && u > v ) {
                std::swap( e.first , e.second );
            }
            // 重边检查
            if ( !repeated_edges ) {
                if ( edge_set.count( e ) ) {
                    return false;
                }
                edge_set.insert( e );
            }
            edges.push_back( { u , v } );
            return true;
        };
        // ───────────────────────────────────────────────────────────────────
        // Step 1: 如果需要连通，先生成基础连通结构
        // ───────────────────────────────────────────────────────────────────
        if ( connected && n > 1 ) {
            // 生成随机排列用于构建生成树/路径
            std::vector < int > perm( n );
            std::iota( perm.begin() , perm.end() , 0 );
            // Fisher-Yates 打乱
            for ( int i = n - 1 ; i > 0 ; --i ) {
                std::uniform_int_distribution < int > dist( 0 , i );
                std::swap( perm[i] , perm[dist( rng_64 )] );
            }
            // 每个节点连接到前面的某个节点，形成生成树
            std::uniform_real_distribution < double > real_dist( 0.0 , 1.0 );
            for ( int i = 1 ; i < n ; ++i ) {
                // 随机选择 [0, i-1] 中的一个节点作为父节点
                std::uniform_int_distribution < int > parent_dist( 0 , i - 1 );
                int parent_idx = parent_dist( rng_64 );
                int u = perm[parent_idx];
                int v = perm[i];
                // 有向图时随机决定方向
                if ( directional && real_dist( rng_64 ) < 0.5 ) {
                    std::swap( u , v );
                }
                try_add_edge( u , v );
            }
        }
        // ───────────────────────────────────────────────────────────────────
        // Step 2: 随机添加剩余的边
        // ───────────────────────────────────────────────────────────────────
        int remaining = m - static_cast < int >( edges.size() );
        if ( remaining > 0 ) {
            std::uniform_int_distribution < int > node_dist( 0 , n - 1 );
            // 计算可能的边总数
            long long max_possible = compute_max_edges( n , repeated_edges , self_rings , directional );
            // 根据稀疏程度选择算法
            double density = static_cast < double >( m ) / std::max( 1LL , max_possible );
            if ( density < 0.5 || repeated_edges ) {
                // ─────────────────────────────────────────────────────────────
                // 稀疏图：随机采样法
                // ─────────────────────────────────────────────────────────────
                int max_attempts = remaining * 100 + 10000;
                int attempts = 0;
                while ( remaining > 0 && attempts < max_attempts ) {
                    int u = node_dist( rng_64 );
                    int v = node_dist( rng_64 );
                    if ( try_add_edge( u , v ) ) {
                        --remaining;
                    }
                    ++attempts;
                }
                // 如果随机采样失败，使用枚举法补充
                if ( remaining > 0 && !repeated_edges ) {
                    for ( int u = 0 ; u < n && remaining > 0 ; ++u ) {
                        int v_start = directional ? 0 : u;
                        for ( int v = v_start ; v < n && remaining > 0 ; ++v ) {
                            if ( try_add_edge( u , v ) ) {
                                --remaining;
                            }
                        }
                    }
                }
            } else {
                // ─────────────────────────────────────────────────────────────
                // 稠密图：枚举所有可能的边，然后随机选择
                // ─────────────────────────────────────────────────────────────
                std::vector < std::pair < int , int > > all_edges;
                for ( int u = 0 ; u < n ; ++u ) {
                    int v_start = directional ? 0 : u;
                    for ( int v = v_start ; v < n ; ++v ) {
                        if ( !self_rings && u == v ) continue;
                        if ( directional && u == v && !self_rings ) continue;
                        std::pair < int , int > e = { u , v };
                        if ( !directional && u > v ) std::swap( e.first , e.second );
                        if ( !edge_set.count( e ) ) {
                            all_edges.push_back( { u , v } );
                        }
                    }
                    // 有向图需要考虑反向边
                    if ( directional ) {
                        for ( int v = 0 ; v < u ; ++v ) {
                            if ( !self_rings && u == v ) continue;
                            std::pair < int , int > e = { u , v };
                            if ( !edge_set.count( e ) ) {
                                all_edges.push_back( { u , v } );
                            }
                        }
                    }
                }
                // Fisher-Yates 打乱并取前 remaining 条
                for ( size_t i = all_edges.size() ; i > 0 && remaining > 0 ; --i ) {
                    std::uniform_int_distribution < size_t > dist( 0 , i - 1 );
                    size_t j = dist( rng_64 );
                    std::swap( all_edges[i - 1] , all_edges[j] );
                    auto &e = all_edges[i - 1];
                    if ( try_add_edge( e.first , e.second ) ) {
                        --remaining;
                    }
                }
            }
        }
        // ───────────────────────────────────────────────────────────────────
        // Step 3: 打乱边的顺序和端点顺序
        // ───────────────────────────────────────────────────────────────────
        // 打乱边的顺序
        for ( int i = static_cast < int >( edges.size() ) - 1 ; i > 0 ; --i ) {
            std::uniform_int_distribution < int > dist( 0 , i );
            std::swap( edges[i] , edges[dist( rng_64 )] );
        }
        // 对于无向图，随机交换每条边的端点
        if ( !directional ) {
            std::bernoulli_distribution coin( 0.5 );
            for ( auto &e : edges ) {
                if ( coin( rng_64 ) ) {
                    std::swap( e.u , e.v );
                }
            }
        }
        std::vector < std::pair < int , int > > e;
        for ( const auto &[u,v] : edges ) {
            e.emplace_back( u + base , v + base );
        }
        return e;
    }

    std::vector < std::pair < int , int > > RANDOMGRAPH::random_binary_graph(
        int n1 , int n2 , int m , bool directional ,
        int base ) {
        if ( n1 <= 0 || n2 <= 0 ) {
            throw std::invalid_argument( "n1 and n2 must be positive" );
        }
        long long max_edges = static_cast < long long >( n1 ) * n2;
        if ( directional ) max_edges *= 2;
        m = static_cast < int >( std::min( static_cast < long long >( m ) , max_edges ) );
        std::vector < Edge > edges;
        edges.reserve( m );
        if ( m <= max_edges / 2 ) {
            // 稀疏：随机采样
            std::unordered_set < long long > edge_set;
            std::uniform_int_distribution < int > dist1( 0 , n1 - 1 );
            std::uniform_int_distribution < int > dist2( n1 , n1 + n2 - 1 );
            std::bernoulli_distribution coin( 0.5 );
            while ( static_cast < int >( edges.size() ) < m ) {
                int u = dist1( rng_64 );
                int v = dist2( rng_64 );
                if ( directional && coin( rng_64 ) ) {
                    std::swap( u , v );
                }
                long long key = static_cast < long long >( u ) * ( n1 + n2 ) + v;
                if ( !edge_set.count( key ) ) {
                    edge_set.insert( key );
                    edges.push_back( { u , v } );
                }
            }
        } else {
            // 稠密：枚举后随机选择
            std::vector < std::pair < int , int > > all_edges;
            for ( int u = 0 ; u < n1 ; ++u ) {
                for ( int v = n1 ; v < n1 + n2 ; ++v ) {
                    all_edges.push_back( { u , v } );
                    if ( directional ) {
                        all_edges.push_back( { v , u } );
                    }
                }
            }
            // 打乱并取前 m 条
            for ( size_t i = all_edges.size() - 1 ; i > 0 ; --i ) {
                std::uniform_int_distribution < size_t > dist( 0 , i );
                std::swap( all_edges[i] , all_edges[dist( rng_64 )] );
            }
            for ( int i = 0 ; i < m ; ++i ) {
                edges.push_back( { all_edges[i].first , all_edges[i].second } );
            }
        }
        std::vector < std::pair < int , int > > e;
        for ( const auto &[u,v] : edges ) {
            e.emplace_back( u + base , v + base );
        }
        return e;
    }

    std::vector < std::pair < int , int > > RANDOMGRAPH::random_dag_graph( int n , int m , int base ) {
        if ( n <= 0 ) throw std::invalid_argument( "n must be positive" );
        long long max_edges = static_cast < long long >( n ) * ( n - 1 ) / 2;
        m = static_cast < int >( std::min( static_cast < long long >( m ) , max_edges ) );
        std::vector < Edge > edges;
        edges.reserve( m );
        // 生成拓扑序
        std::vector < int > topo_order( n );
        std::iota( topo_order.begin() , topo_order.end() , 0 );
        for ( int i = n - 1 ; i > 0 ; --i ) {
            std::uniform_int_distribution < int > dist( 0 , i );
            std::swap( topo_order[i] , topo_order[dist( rng_64 )] );
        }
        // 逆映射：topo_pos[v] = v 在拓扑序中的位置
        std::vector < int > topo_pos( n );
        for ( int i = 0 ; i < n ; ++i ) {
            topo_pos[topo_order[i]] = i;
        }
        // 生成边：保证 u 在拓扑序中在 v 之前
        struct PairHash {
            size_t operator()( const std::pair < int , int > &p ) const {
                return std::hash < long long >()(
                    static_cast < long long >( p.first ) * 1000000007LL + p.second
                );
            }
        };
        std::unordered_set < std::pair < int , int > , PairHash > edge_set;
        if ( m <= max_edges / 2 ) {
            // 稀疏图：随机采样
            int attempts = 0;
            int max_attempts = m * 100 + 10000;
            while ( static_cast < int >( edges.size() ) < m && attempts < max_attempts ) {
                std::uniform_int_distribution < int > dist( 0 , n - 1 );
                int a = dist( rng_64 );
                int b = dist( rng_64 );
                if ( a == b ) {
                    ++attempts;
                    continue;
                }
                // 确保边的方向符合拓扑序
                int u = ( topo_pos[a] < topo_pos[b] ) ? a : b;
                int v = ( topo_pos[a] < topo_pos[b] ) ? b : a;
                std::pair < int , int > e = { u , v };
                if ( !edge_set.count( e ) ) {
                    edge_set.insert( e );
                    edges.push_back( { u , v } );
                }
                ++attempts;
            }
        } else {
            // 稠密图：枚举所有可能的边
            std::vector < std::pair < int , int > > all_edges;
            for ( int i = 0 ; i < n ; ++i ) {
                for ( int j = i + 1 ; j < n ; ++j ) {
                    all_edges.push_back( { topo_order[i] , topo_order[j] } );
                }
            }
            // 打乱并取前 m 条
            for ( size_t i = all_edges.size() - 1 ; i > 0 ; --i ) {
                std::uniform_int_distribution < size_t > dist( 0 , i );
                std::swap( all_edges[i] , all_edges[dist( rng_64 )] );
            }
            for ( int i = 0 ; i < m && i < static_cast < int >( all_edges.size() ) ; ++i ) {
                edges.push_back( { all_edges[i].first , all_edges[i].second } );
            }
        }
        // 打乱边的顺序
        for ( int i = static_cast < int >( edges.size() ) - 1 ; i > 0 ; --i ) {
            std::uniform_int_distribution < int > dist( 0 , i );
            std::swap( edges[i] , edges[dist( rng_64 )] );
        }
        std::vector < std::pair < int , int > > e;
        for ( const auto &[u,v] : edges ) {
            e.emplace_back( u + base , v + base );
        }
        return e;
    }

    long long QuickPow( long long x , long long pow , long long p ) {
        long long res = 1;
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

    long long exgcd( long long a , long long b , long long &x , long long &y ) {
        if ( !b ) {
            x = 1;
            y = 0;
            return a;
        }
        long long d = exgcd( b , a % b , x , y );
        long long tmp = x;
        x = y;
        y = tmp - a / b * y;
        return d;
    }

    std::ifstream openInputFileStream( const std::string &str ) {
        return std::ifstream( str.c_str() , std::ios::in | std::ios::binary );
    }

    std::ofstream openOutputFileStream( const std::string &str ) {
        return std::ofstream( str.c_str() , std::ios::out | std::ios::binary );
    }

    GradientOptResult compute_gradient_opt( int total_files , int file_id , bool zero_indexed ,
                                            GradientStrategy strategy , double sharpness ) {
        // ─────────────────────────────────────────────────────────────────────
        // 参数校验
        // ─────────────────────────────────────────────────────────────────────
        if ( total_files <= 0 ) {
            throw std::invalid_argument(
                "total_files must be positive, got: " + std::to_string( total_files )
            );
        }
        if ( sharpness <= 0.0 ) {
            throw std::invalid_argument(
                "sharpness must be positive, got: " + std::to_string( sharpness )
            );
        }
        // 统一转换为 1-indexed 进行计算
        int idx = zero_indexed ? file_id + 1 : file_id;
        if ( idx < 1 || idx > total_files ) {
            throw std::out_of_range(
                "file_id " + std::to_string( file_id ) + " out of range [" +
                ( zero_indexed ? "0" : "1" ) + ", " +
                std::to_string( zero_indexed ? total_files - 1 : total_files ) + "]"
            );
        }
        // ─────────────────────────────────────────────────────────────────────
        // 计算归一化位置 t ∈ (0, 1)
        // 使用 (idx - 0.5) / total_files 确保：
        //   - 第一个文件的目标不是 0（会导致极端值）
        //   - 最后一个文件的目标不是 1（会导致极端值）
        // ─────────────────────────────────────────────────────────────────────
        double t = ( static_cast < double >( idx ) - 0.5 ) / total_files;
        // ─────────────────────────────────────────────────────────────────────
        // 根据策略变换 t → target
        // ─────────────────────────────────────────────────────────────────────
        double target;

        switch ( strategy ) {
            case GradientStrategy::LINEAR :
                // 线性：直接使用
                target = t;
                break;
            case GradientStrategy::EXPONENTIAL :
                // 指数：前期慢后期快
                // f(t) = (e^(kt) - 1) / (e^k - 1), k = 3
            {
                constexpr double k = 3.0;
                double exp_kt = std::exp( k * t );
                double exp_k = std::exp( k );
                target = ( exp_kt - 1.0 ) / ( exp_k - 1.0 );
            }
            break;

            case GradientStrategy::LOGARITHMIC :
                // 对数：前期快后期慢
                // f(t) = ln(1 + kt) / ln(1 + k), k = 9
            {
                constexpr double k = 9.0;
                target = std::log1p( k * t ) / std::log1p( k );
            }
            break;

            case GradientStrategy::QUADRATIC :
                // 二次：加速递增
                target = t * t;
                break;

            case GradientStrategy::SQRT :
                // 平方根：减速递增
                target = std::sqrt( t );
                break;

            case GradientStrategy::SIGMOID :
                // S 型曲线：两端平缓中间陡峭
                // 使用 logistic 函数并归一化到 [0, 1]
            {
                constexpr double k = 10.0;
                auto sigmoid = [k]( double x ) {
                    return 1.0 / ( 1.0 + std::exp( -k * ( x - 0.5 ) ) );
                };
                double sig_t = sigmoid( t );
                double sig_0 = sigmoid( 0.0 );
                double sig_1 = sigmoid( 1.0 );
                target = ( sig_t - sig_0 ) / ( sig_1 - sig_0 );
            }
            break;

            case GradientStrategy::UNIFORM :
                // 均匀：所有文件都使用 opt = 1
                target = 0.5;
                break;

            default : target = t;
        }
        // ─────────────────────────────────────────────────────────────────────
        // 边界保护：避免 log(0) 或 log(1) 问题
        // ─────────────────────────────────────────────────────────────────────
        constexpr double eps = 1e-9;
        target = std::clamp( target , eps , 1.0 - eps );
        // ─────────────────────────────────────────────────────────────────────
        // 计算基础 opt
        //
        // 中位数公式：median = 0.5^{1/opt}
        // 若要 median = target，则：
        //   0.5^{1/opt} = target
        //   1/opt = log(target) / log(0.5)
        //   opt = log(0.5) / log(target)
        // ─────────────────────────────────────────────────────────────────────
        double base_opt = std::log( 0.5 ) / std::log( target );
        // ─────────────────────────────────────────────────────────────────────
        // 应用锐度参数
        //
        // 锐度通过调整 opt 远离 1 的程度来实现：
        // - sharpness > 1: opt 更极端（更远离 1）
        // - sharpness < 1: opt 更接近 1（更均匀）
        //
        // 使用对数空间进行调整：
        //   log(adjusted_opt) = log(base_opt) * sharpness
        // ─────────────────────────────────────────────────────────────────────
        double adjusted_opt;
        if ( std::fabs( sharpness - 1.0 ) < 1e-9 ) {
            adjusted_opt = base_opt;
        } else {
            // 在对数空间中应用锐度
            double log_opt = std::log( base_opt );
            adjusted_opt = std::exp( log_opt * sharpness );
        }
        // 保护 opt 在合理范围内
        adjusted_opt = std::clamp( adjusted_opt , 1e-6 , 1e9 );
        // ─────────────────────────────────────────────────────────────────────
        // 计算预期统计量
        // ─────────────────────────────────────────────────────────────────────
        // 中位数：0.5^{1/opt}
        double expected_median = std::pow( 0.5 , 1.0 / adjusted_opt );
        // 均值：E[X^{1/opt}] = opt / (opt + 1)
        double expected_mean = adjusted_opt / ( adjusted_opt + 1.0 );
        // ─────────────────────────────────────────────────────────────────────
        // 构建结果
        // ─────────────────────────────────────────────────────────────────────
        GradientOptResult result;
        result.opt = adjusted_opt;
        result.target_position = target;
        result.expected_median_ratio = expected_median;
        result.expected_mean_ratio = expected_mean;
        result.file_id = file_id;
        result.total_files = total_files;
        return result;
    }

    double getGradientOpt( int total_files , int file_id , bool zero_indexed ,
                           GradientStrategy strategy , double sharpness ) {
        return compute_gradient_opt( total_files , file_id , zero_indexed , strategy , sharpness ).opt;
    }

    std::vector < double > getAllGradientOpts( int total_files , GradientStrategy strategy ,
                                               double sharpness ) {
        std::vector < double > opts;
        opts.reserve( total_files );
        for ( int i = 0 ; i < total_files ; ++i ) {
            opts.push_back( getGradientOpt( total_files , i , true , strategy , sharpness ) );
        }
        return opts;
    }

    bool batchGenerateFiles( int startIndex , int endIndex ,
                             std::function < void( long long , long long , double ) > solve ,
                             long long Limit , std::stringstream &ins , std::stringstream &ous ,
                             int multiple , GradientStrategy strategy ) {
        // 输入数据存储集合
        std::vector < std::string > INPUT_DATA;
        // 输出数据存储集合
        std::vector < std::string > OUTPUT_DATA;
        // 初始化梯度配置生成器
        GradientScaleGenerator gen( Limit );
        // 初始化影响因子生成
        auto opts = getAllGradientOpts( endIndex - startIndex + 1 , strategy );
        for ( int index = startIndex ; index <= endIndex ; ++index ) {
            std::ofstream writeInputFile =
                    openOutputFileStream( dataFilePath + std::to_string( index ) + inFileType );
            if ( writeInputFile.is_open() ) {
                std::cout << "成功打开文件 " << index << "\n";
                auto [DATAL , DATAR] = gen.getGradientRange( index );
                double OFFSET = opts[index - startIndex];
                int _ = random( 1 , multiple , OFFSET );
                while ( _ -- ) {
                    solve( DATAL , DATAR , OFFSET );
                    INPUT_DATA.push_back( ins.str() );
                    OUTPUT_DATA.push_back( ous.str() );
                    MemUtil::zero( ins );
                    MemUtil::zero( ous );
                }
                // -------- 输出 Start --------
                for ( const auto &item : INPUT_DATA ) {
                    writeInputFile << item;
                }
                INPUT_DATA.clear();
                // -------- 输出 End --------
                writeInputFile.close();
                std::cout << "数据写入成功 " << index << "\n";
            } else {
                std::cerr << "写入文件失败 " << index << "\n";
                return false;
            }
            std::ofstream writeOutputFile =
                    openOutputFileStream( dataFilePath + std::to_string( index ) + outFileType );
            if ( writeOutputFile.is_open() ) {
                std::cout << "成功打开文件 " << index << "\n";
                // -------- 输出 Start --------
                for ( const auto &item : OUTPUT_DATA ) {
                    writeOutputFile << item;
                }
                OUTPUT_DATA.clear();
                // -------- 输出 End --------
                writeOutputFile.close();
                std::cout << "数据写入成功 " << index << "\n";
            } else {
                std::cerr << "写入文件失败 " << index << "\n";
                return false;
            }
        }
        return true;
    }
}
