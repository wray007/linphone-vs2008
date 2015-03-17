**编译顺序**

  * 编译Audio下的6个工程:libbroadvoice, libg726, libgsm, libilbc, libspeex, libspeexdsp
  * 编译 osipparse2 -> osip2 -> eXosip2 -> oRTP -> mediasteamer2 ->libsipua3
  * 最后编译 MicroVoiceLite
  * 编译完成后在build\win32\Debug 目录下将生成 1个exe文件，7个dll，将这些文件复制到最上层的Bin目录下
  * 再到codecs\ffmpeg-i386\bin 下，将里面的dll也复制到 Bin目录下
  * 可以运行MicroVoiceLite.exe了