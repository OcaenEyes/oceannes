# OCEANNES

## 名词解释

本篇可能能遇到下列名词:
```
- CPU: 中央处理器, 即2A03
- PPU: 图形处理器, 用来控制/显示图形之类的
- VRAM: 即Video-RAM, 俗称显存
- PRG-ROM: 程序只读储存器: 存储程序代码的存储器. 放入CPU地址空间.
- CHR-ROM: 角色只读储存器, 基本是用来显示图像, 放入PPU地址空间
- VROM: 基本和CHR-ROM同义, 用于理解CHR-ROM
- SRAM: 存档(SAVE)用RAM, 有些卡带额外带了用电池供电的RAM
- WRAM: 工作(WORK)用RAM, 基本和SRAM一样, 不过不是用来存档, 就是拿来一般用的
- Mapper: 由于地址空间最多64KB, 当游戏太大时, 会使用Mapper/MMC用来切换当前使用的'BANK'. 软件(模拟器)上的实现, Mapper会把类似的MMC放在一起实现
- MMC: Memory-Management Controller 硬件(卡带)上的实现, 会有非常多的大类, 甚至还有变种. 在国内为了显示汉字还有魔改版
- CHR-RAM: 基本同CHR-ROM, 只不过可以写
- PRG-RAM: 基本同PRG-ROM, 只不过可以写
```

## 读取ROM
### FC游戏ROM
目前流行的ROM格式是.nes格式的，其中NES2.0的ROM格式：
```
文件头:
 0-3: string    "NES"<EOF>
   4: byte      以16384(0x4000)字节作为单位的PRG-ROM大小数量
   5: byte      以 8192(0x2000)字节作为单位的CHR-ROM大小数量
   6: bitfield  Flags 6
   7: bitfield  Flags 7
8-15: byte      保留用, 应该为0. 其实有些在用了, 目前不管

CHR-ROM - 角色只读存储器(用于图像显示, 暂且不谈)

Flags 6:
7       0
---------
NNNN FTBM

N: Mapper编号低4位
F: 4屏标志位. (如果该位被设置, 则忽略M标志)
T: Trainer标志位.  1表示 $7000-$71FF加载 Trainer
B: SRAM标志位 $6000-$7FFF拥有电池供电的SRAM.
M: 镜像标志位.  0 = 水平, 1 = 垂直.

Byte 7 (Flags 7):
7       0
---------
NNNN xxPV

N: Mapper编号高4位
P: Playchoice 10标志位. 被设置则表示为PC-10游戏
V: Vs. Unisystem标志位. 被设置则表示为Vs.  游戏
x: 未使用

```


## 第三方库 SFML2.6.0
https://www.sfml-dev.org/download/sfml/2.6.0/


