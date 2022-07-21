#include "../1_SM3_implementation/mysm3.h"
#include "../1_SM3_implementation/mysm3.cpp"
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<string>

/**********************************************************
 * 函数名:  LenExtenAttack 
 * 描述：   传入原始消息的Hash值，和其长度(如果不能确定原始消息长度，那么请先想办法获取)，
 *          然后传入拓展部分的M3，计算出SM3(M||padding||M3)的值
 * Input：  string M1,string M2
 *********************************************************/
void LenExtenAttack(uchar* H,int Len,string M3)
{
    uchar H1[SM3_OUTLEN];   
    //计算长为Len的消息H，需要多少个64byte
    int tmp = 64*ceil(Len/64);
    if(Len&63 > 55)
        tmp += 64;

    //开辟一个char数组，用于表示M' = M||padding||M3,
    uchar* M = new uchar[tmp+M3.length()];

    //置零
    memset(M,0,tmp+M3.length());

    //将M3拷贝到M的尾部
    memcpy(M+tmp,(const uchar*)M3.c_str(),M3.length());

    //适用H对ctx的IV进行初始化，单独进行初始化：
    SM3_CTX ctx; //如果用指针，需要new一下分配空间，艹
    ctx.msgLen=ctx.curlen = 0;
    uchar* data;
    data = H;
/**********************************************************
 * H是之前的压缩值，在mysm3.cpp的SM3_paddingpart的最后，是把ctx->state的大端内存转成了小端的，
 * 我们现在要把H写回到int类型的state中，就要转成大端
 * 
 *********************************************************/
    (void)HOST_c2l(data,ctx.state[0]);
    (void)HOST_c2l(data,ctx.state[1]);
    (void)HOST_c2l(data,ctx.state[2]);
    (void)HOST_c2l(data,ctx.state[3]);
    (void)HOST_c2l(data,ctx.state[4]);
    (void)HOST_c2l(data,ctx.state[5]);
    (void)HOST_c2l(data,ctx.state[6]);
    (void)HOST_c2l(data,ctx.state[7]);

/**********************************************************
 * 此处还要修改一下CTX的参数，我们需要给SM3函数一个已经压缩完一个block的状态，
 * 也就是说，input是M'=M||M3,但是传入SM3_process的数据长度此时只有M3.len了
 * 而且已压缩长度应该是已压缩的block的长度 ctx->msglen = tmp*8
 * 
 *********************************************************/
    ctx.msgLen = tmp<<3;
    SM3_process(&ctx,M,M3.length()); 
    SM3_paddingpart(&ctx,H1);
    cout<<"利用H和Len计算SM3(M3) where IV=H="<<endl;
    print_Hashvalue(H1,32);
}

int main()
{
    string M1 = "Hello World";  //原始消息
    string M3 = "AAA";          //拓展消息

    int Len = M1.length();
    int tmp = 64*ceil(Len/64);
    if(Len&63 > 55)
        tmp += 64;

    //计算原始消息M1的SM3值
    uchar H[SM3_OUTLEN];
    SM3(M1,H);
    cout<<"SM3(M)=";
    print_Hashvalue(H,32);
    cout<<endl;
    uchar* M = new uchar[tmp+M3.length()];
    memset(M,0,tmp+M3.length());
    memcpy(M,M1.c_str(),M1.length());
    M[M1.length()] = 0x80;

    uint M1_len = M1.length()<<3;    //M1_len是消息的bit长度,*8
    M[(tmp-64)+63] =  M1_len & 0xff;
    M[(tmp-64)+62] = (M1_len >> 8) & 0xff;
    M[(tmp-64)+61] = (M1_len >> 16) & 0xff;
    M[(tmp-64)+60] = (M1_len >> 24) & 0xff;

    memcpy(M+tmp,(const uchar*)M3.c_str(),M3.length());
    cout<<"级联后消息M=M1||padding||M3:"<<endl;
    print_Hashvalue(M,tmp+M3.length());
    cout<<endl;
    //计算理论级联后的hash值
    uchar H1[SM3_OUTLEN];
    SM3(M,tmp+M3.length(),H1);
    cout<<"级联后SM(M=M1||padding||M3):"<<endl;
    print_Hashvalue(H1,32);

    LenExtenAttack(H,M1.length(),M3);
         
    return 0;
}
