# SM3-_LenExtenAttack
内含实验报告
我们的实现原理遵从下面两个准则：


![image](https://user-images.githubusercontent.com/75195549/180154943-0f863262-2e4e-4f80-b68d-482bfba35717.png)



同时我们要注意的一点是：


计算H'时，注意此时状态，不只要注意IV，并对其进行修改，还要将已压缩的字节置成正确的值，也就是要计算M1级联上我们所求padding的值，最后对M3的padding，len也是算上前面的len，这个比较简单，仔细想想就能想出来。
