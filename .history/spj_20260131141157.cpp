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

// 1. 计算标准答案的最小 WPL
long long get_std_wpl( vector < int > weights ) {
    priority_queue < int , vector < int > , greater < int > > pq;
    for ( int w : weights ) pq.push( w );
    long long wpl = 0;
    while ( pq.size() > 1 ) {
        int a = pq.top();
        pq.pop();
        int b = pq.top();
        pq.pop();
        wpl += ( a + b ); // 合并代价即为增加的WPL贡献（非叶子节点权值和 = WPL）
        pq.push( a + b );
    }
    return wpl; // 注意：这是简化的WPL计算方式（所有非叶子节点权值之和）
}

// 2. SPJ 验证函数
int solve() {
    int N;
    FIn >> N;
    vector < int > weights( N );
    // ... 读取 N 个权值到 weights ...
    for ( int i = 0 ; i < N ; i ++ ) {
        FIn >> weights[i];
    }
    // 计算标准 WPL
    long long optimal_wpl = get_std_wpl( weights );

    // 读取用户输出并重建树验证
    // 哈夫曼树总节点数一定是 2*N - 1
    int total_nodes = 2 * N - 1;
    struct UserNode {
        int val;
        int type;
        int depth;
    };
    vector < UserNode > nodes;

    // 读取所有用户输出的节点
    for ( int i = 0 ; i < total_nodes ; i ++ ) {
        int v;
        int t;
        FUser >> v >> t;
        nodes.push_back( { v , t , 0 } );
    }

    // 核心验证逻辑：模拟层序遍历还原父子关系
    // 队列中存储的是节点的索引
    queue < int > q;
    q.push( 0 ); // 根节点是第一个
    nodes[0].depth = 0;

    int current_child_idx = 1; // 下一个待分配的孩子节点索引
    long long user_wpl_calc = 0;
    multiset < int > user_leaves;

    while ( !q.empty() ) {
        int u_idx = q.front();
        q.pop();

        if ( nodes[u_idx].type == 0 ) {
            // 如果是内部节点
            // 必须有两个孩子
            if ( current_child_idx + 1 >= total_nodes ) return WA;

            int left = current_child_idx ++;
            int right = current_child_idx ++;

            // 验证权值约束：父 = 左 + 右
            if ( nodes[u_idx].val != nodes[left].val + nodes[right].val ) return WA;

            // 更新深度并入队
            nodes[left].depth = nodes[u_idx].depth + 1;
            nodes[right].depth = nodes[u_idx].depth + 1;
            q.push( left );
            q.push( right );
        } else {
            // 是叶子节点，记录WPL贡献
            user_wpl_calc += ( long long ) nodes[u_idx].val * nodes[u_idx].depth;
            user_leaves.insert( nodes[u_idx].val );
        }
    }

    // 验证叶子集合是否一致
    if ( user_leaves != multiset < int >( weights.begin() , weights.end() ) ) return WA;

    // 验证 WPL 是否最优
    if ( user_wpl_calc != optimal_wpl ) return WA; // 可能是结构不对导致WPL偏大

    return AC;
}

int main( int argc , char * args[] ) {
    // args[1] = "D:\\Desktop\\Data\\20.in";
    // args[2] = "D:\\Desktop\\Data\\20.out";
    // args[3] = "D:\\Desktop\\Data\\20.out";

    initFiles( argc , args );
    /*****spj????????*******/
    int result = solve();
    /*****spj-end********/
    closeFiles();
    // cout << result << endl;
    return result;
}
