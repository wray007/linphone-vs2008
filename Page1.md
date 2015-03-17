**linphone-vs2008 在vs2010上的编译问题解决**

  * 用vs2010打开 built/win32/libsipua3.sln
  * 会转换成 vs2010的解决方案，转换完成后，发现有两个工程unavailable：libsipua3和libspeex
  * 查看convert报告，libsipua3是由于mobile sdk的问题(可能vs2010不支持mobile sdk，未求证)，解决办法：关闭工程，用文本查看工具打开 libsipua3.vcproj，删除 177~485行；重新打开解决方案，发现libsipua3已经正常了
  * libspeex也是同样的问题，打开codecs\speex\win32\VS2008\libspeex\libspeex.vcproj 删除以下行：87~162行 232~307 364~447 504~587 644~733 792~867 924~1007 1064~1140 1193~1276 1333~1408；如果还是提示unavailable，则remove掉，然后再手动添加，就可以了