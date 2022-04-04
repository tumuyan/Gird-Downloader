# [Gird Downloader](https://github.com/tumuyan/Gird-Downloader)

这是前几年为了从某网游服务器扒资源，写过的一个批量下载工具。

最近想扒电子书，拿出来改改又能继续用了。

我对C语言了解的并不多，写的非常蹩脚，代码基本没什么参考价值。单纯感觉可能还没有类似的工具，保存到仓库里做一个分享。

本质而言，使用excel+文本编辑器可以实现此程序的全部功能。但是如果使用此了程序，可以更快捷。

## 特点

1. 重复执行同一个下载任务，会自动跳过已经下载的文件（只要相应文件存在，并且文件大小不为0，不对文件进行校验，避免服务器更新的文件覆盖本机文件）
2. 能够自动批量下载
3. 下载时可以按照链接顺序自动命名为数字

## 基本用法

1. 下载本程序，并且在同目录放置aira2c程序

2. 新建一个文本文件，粘贴需要下载的链接，每行为1个链接（程序会自动忽略空字符）。修改文件名为`.gd`后缀。

3. 在cmd或者powershell中输入命令`GirdDownloader.exe gd文件路径  `并回车。当程序中停时，按任意键即可。

   或者直接把gd文件拖拽到程序上，或者关联gd文件格式和GirdDownloader。
   
   下载后的文件保存在gd文件所在的目录，下载过程生成的log在gd程序所在目录（命名为file.log）

## 进阶用法

1. 在gd文件中，支持如下使用方法：

   ```
   gda https://cubecdn.xx.cn/ui/image/clothes/icon10476.jpg
   gdb https://cubecdn.xx.cn/ui/image/clothes/icon10477.jpg
   ```

   程序根据数字规律，自动下载icon10476.jpg ~ icon10477.jpg之间的全部文件。

   gda/gdb成对出现，否则无效。同一个文件允许有多对gda/gdb命令以及普通下载链接

2. 在调用程序时，允许添加额外的参数：

   按照gd文件顺序，对下载到的文件重命名为纯数字

   `GirdDownloader.exe gd文件路径 -r`

## 编译

虽然代码很凑数，但是如果你一定要自己编译或者修改，也不是不行。

~~clone后直接使用[codeblocks-20.03-nosetup](https://sourceforge.net/projects/codeblocks/files/Binaries/20.03/Windows/codeblocks-20.03-nosetup.zip/download)打开即可进行编译。~~

由于未知原因（其实应该还是运行时的问题吧），现在在Code::Blocks中可以编译并运行，但是无法脱离Code::Blocks使用。故改为vs编译（Microsoft Visual Studio Community 2019）。





---

v0.1, by tumuyan







