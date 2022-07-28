# SM3-_LenExtenAttack
# 实验原理

我们的实现原理遵从下面两个准则：


![image](https://user-images.githubusercontent.com/75195549/180154943-0f863262-2e4e-4f80-b68d-482bfba35717.png)



同时我们要注意的一点是：


计算H'时，注意此时状态，不只要注意IV，并对其进行修改，还要将已压缩的字节置成正确的值，也就是要计算M1级联上我们所求padding的值，最后对M3的padding，len也是算上前面的len，这个比较简单，仔细想想就能想出来。


更形象的解释就是下面这张图，很好理解：


![image](https://user-images.githubusercontent.com/75195549/181451419-50e58501-2b95-404f-984d-eebebd3d34ff.png)




# 代码书写思路
1.随机选取字符串m，使用SM3加密得到其hash值h。

2.随机选取字符串m2，作为扩展攻击的字符串备用。

3.首先计算出m扩展后的与m2拼接的字符串m1，计算出m1的hash值h1，来作为正确答案以备后用。

4.在m2前面补充数量为M长度的0，之后以h作为初始向量进行加密，即可得到长度扩展攻击的hash值。之后与正确答案对比发现攻击成功。


# 代码解释

# LenExtenAttack
传入原始消息的Hash值，和其长度(如果不能确定原始消息长度，那么请先想办法获取)，然后传入拓展部分的M3，计算出SM3(M||padding||M3)的值



首先计算长为Len的消息H，需要多少个64byte，然后开辟一个char数组，用于表示M' = M||padding||M3,随后置零并将M3拷贝到M的尾部。



![image](https://user-images.githubusercontent.com/75195549/181454014-d81f2b9b-9424-441d-b82a-934736f52f2d.png)


# 修改一下CTX的参数
我们需要给SM3函数一个已经压缩完一个block的状态，也就是说，input是M'=M||M3,但是传入SM3_process的数据长度此时只有M3.len了而且已压缩长度应该是已压缩的block的长度 ctx->msglen = tmp*8

![image](https://user-images.githubusercontent.com/75195549/181454502-875d729a-f7aa-47d1-bd3f-af15f40148e3.png)



# 结果展示

![image](https://user-images.githubusercontent.com/75195549/180384840-bcf2a848-7f00-480b-8493-9307586b5417.png)




