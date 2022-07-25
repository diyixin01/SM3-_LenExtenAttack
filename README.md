# SM3-_LenExtenAttack
我们的实现原理遵从下面两个准则：


![image](https://user-images.githubusercontent.com/75195549/180154943-0f863262-2e4e-4f80-b68d-482bfba35717.png)



同时我们要注意的一点是：


计算H'时，注意此时状态，不只要注意IV，并对其进行修改，还要将已压缩的字节置成正确的值，也就是要计算M1级联上我们所求padding的值，最后对M3的padding，len也是算上前面的len，这个比较简单，仔细想想就能想出来。




# 代码书写思路
1.随机选取字符串m，使用SM3加密得到其hash值h。

2.随机选取字符串m2，作为扩展攻击的字符串备用。

3.首先计算出m扩展后的与m2拼接的字符串m1，计算出m1的hash值h1，来作为正确答案以备后用。

4.在m2前面补充数量为M长度的0，之后以h作为初始向量进行加密，即可得到长度扩展攻击的hash值。之后与正确答案对比发现攻击成功。





![image](https://user-images.githubusercontent.com/75195549/180384840-bcf2a848-7f00-480b-8493-9307586b5417.png)




