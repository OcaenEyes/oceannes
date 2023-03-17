/*
 * @Author: OCEAN.GZY
 * @Date: 2023-03-17 16:00:07
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-17 17:18:31
 * @FilePath: /oceannes/desktop/src/KeybindingsParser.cpp
 * @Description: 注释信息
 */
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "Controller.h"
#include "LogUtil.h"

inline std::string ltrim(const std::string &str)
{
    std::string s(str);
    /**
     *  earse删除
     *  （1）erase(pos,n); 删除从pos开始的n个字符，比如erase(0,1)就是删除第一个字符
        （2）erase(position);删除position处的一个字符(position是个string类型的迭代器)
        （3）erase(first,last);删除从first到last之间的字符（first和last都是迭代器）
     *
    find_if() 函数用于查找符合谓词函数规则的第一个元素，而 find_if_not() 函数则用于查找第一个不符合谓词函数规则的元素。

    find_if_not() 函数的语法规则如下所示：
            InputIterator find_if_not (InputIterator first, InputIterator last, UnaryPredicate pred);
            其中，first 和 last 都为输入迭代器，[first, last) 用于指定查找范围；pred 用于自定义查找规则。
            和 find_if() 函数一样，find_if_not() 函数也适用于所有的容器，包括所有序列式容器和关联式容器。
            同样，该函数也会返回一个输入迭代器，当 find_if_not() 函数查找成功时，该迭代器指向的是查找到的那个元素；反之，如果查找失败，该迭代器的指向和 last 迭代器相同。


    std::isspace定义于头文件 <cctype>
            int isspace( int ch );  检查给定的字符在当前的 C 本地环境中是否是空白字符。在默认的本地环境中，空白字符是:
                空格 (0x20, ' ')
                换页 (0x0c, '\f')
                换行 (0x0a, '\n')
                回车 (0x0d, '\r')
                水平制表符 (0x09, '\t')
                垂直制表符 (0x0b, '\v')
                如果 ch 的值不能表示为 unsigned char ，并且不等于 EOF ，则函数行为未定义。
            参数：ch	-	要检查的字符字符
            返回值：如果字符是空白字符，返回非零值，否则返回零。

     */

    s.erase(s.begin(), std::find_if_not<decltype(s.begin()), int(int)>(s.begin(), s.end(), std::isspace));
    return s;
}

inline std::string rtrim(const std::string &str)
{
    std::string s(str);
    s.erase(std::find_if_not<decltype(s.rbegin()), int(int)>(s.rbegin(), s.rend(), std::isspace).base(), s.end());
    return s;
}

void parseControllerConf(std::string filepath,
                         std::vector<sf::Keyboard::Key> &p1,
                         std::vector<sf::Keyboard::Key> &p2)
{
    const std::string buttonStrings[] = {"A",
                                         "B",
                                         "Select",
                                         "Start",
                                         "Up",
                                         "Down",
                                         "Left",
                                         "Right"};

    const std::string keys[] = {"A",
                                "B",
                                "C",
                                "D",
                                "E",
                                "F",
                                "G",
                                "H",
                                "I",
                                "J",
                                "K",
                                "L",
                                "M",
                                "N",
                                "O",
                                "P",
                                "Q",
                                "R",
                                "S",
                                "T",
                                "U",
                                "V",
                                "W",
                                "X",
                                "Y",
                                "Z",
                                "Num0",
                                "Num1",
                                "Num2",
                                "Num3",
                                "Num4",
                                "Num5",
                                "Num6",
                                "Num7",
                                "Num8",
                                "Num9",
                                "Escape",
                                "LControl",
                                "LShift",
                                "LAlt",
                                "LSystem",
                                "RControl",
                                "RShift",
                                "RAlt",
                                "RSystem",
                                "Menu",
                                "LBracket",
                                "RBracket",
                                "SemiColon",
                                "Comma",
                                "Period",
                                "Quote",
                                "Slash",
                                "BackSlash",
                                "Tilde",
                                "Equal",
                                "Dash",
                                "Space",
                                "Return",
                                "BackSpace",
                                "Tab",
                                "PageUp",
                                "PageDown",
                                "End",
                                "Home",
                                "Insert",
                                "Delete",
                                "Add",
                                "Subtract",
                                "Multiply",
                                "Divide",
                                "Left",
                                "Right",
                                "Up",
                                "Down",
                                "Numpad0",
                                "Numpad1",
                                "Numpad2",
                                "Numpad3",
                                "Numpad4",
                                "Numpad5",
                                "Numpad6",
                                "Numpad7",
                                "Numpad8",
                                "Numpad9",
                                "F1",
                                "F2",
                                "F3",
                                "F4",
                                "F5",
                                "F6",
                                "F7",
                                "F8",
                                "F9",
                                "F10",
                                "F11",
                                "F12",
                                "F13",
                                "F14",
                                "F15",
                                "Pause"};

    std::ifstream file(filepath);
    std::string line;

    enum
    {
        Player1,
        Player2,
        None
    } pstate;

    unsigned int line_no = 0;

    while (std::getline(file, line))
    {
        /* code */
        line = rtrim(ltrim(line));
        if (line[0] == '#' || line.empty())
        {
            /* code */
            continue;
        }
        else if (line == "[Player1]")
        {
            /* code */
            pstate = Player1;
        }
        else if (line == "[Player2]")
        {
            pstate = Player2;
        }
        else if (pstate == Player1 || pstate == Player2)
        {
            auto divider = line.find("=");
            auto it = std::find(std::begin(buttonStrings), std::end(buttonStrings), ltrim(rtrim(line.substr(0, divider))));
            auto it2 = std::find(std::begin(keys), std::end(keys), ltrim(rtrim(line.substr(divider + 1))));

            if (it == std::end(buttonStrings) || it2 == std::end(keys))
            {
                LOG(Error) << "Invalid key in configuration file at Line " << line_no << std::endl;
                continue;
            }

            auto i = std::distance(std::begin(buttonStrings), it);
            auto key = std::distance(std::begin(keys), it2);

            (pstate == Player1 ? p1 : p2)[i] = static_cast<sf::Keyboard::Key>(key);
        }
        else
        {
            LOG(Error) << "Invalid line in key configuration at line" << line_no << std::endl;
        }

        ++line_no;
    }
}