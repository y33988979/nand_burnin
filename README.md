# nand_burnin
A generic nand flash test automation program, base on the hisilicon chipset

一个通用的nand flash自动化测试程序，基于海思芯片，HI3716MV300，HI3716MV310,HI3716CV100 etc.

一、背景
   基于经常更换flash的需求，并考虑到目前存在的问题，进行需求整理，开发了一款专门测试flash的nand flash测试工具：
测试工具分为2个部分：测试程序 + 图形化配置工具。测试程序是真正的flash性能测试程序，代码借鉴了开源的boot程序，去除其中不必要的功能，专门对flash测试，将我们的测试需求全部用软件实现，并对外提供多种接口，移植性，扩展性强。而图形化配置工具是专门供测试人员使用的，运行在PC电脑上，它用来和测试程序进行交互，并展示测试结果。界面简洁，操作方便，测试人员很容易上手。主要用来控制被测机顶盒，发起测试，查看硬件信息、测试进度，设置参数，导出测试结果等等功能。附软件的部分截图：

获取nand flash芯片的硬件信息，块大小，页大小，厂商，ecc等：
-------------
 ![image](https://github.com/y33988979/nand_burnin/blob/master/screenshot/hw_info.png)
 
 
查看nand flash芯片的坏块分布：
-------------
 ![image](https://github.com/y33988979/nand_burnin/blob/master/screenshot/nand_bad.png)
 
  
对nand flash进行读、写、擦除测试，及参数设置：
--------------------
 ![image](https://github.com/y33988979/nand_burnin/blob/master/screenshot/test.png)
