/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:28:41
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-21 13:04:31
 * @FilePath: \vdesktop\src\Controller.cc
 * @Description: 注释信息
 */
#include "Controller.h"

/*
 * 参考 https://wiki.nesdev.com/w/index.php/Standard_controller
 */

Controller::Controller(/* args */) : m_button_states(0), m_key_bindings(TotalButtons)
{
}

Controller::~Controller()
{
}

/**
 * 7     bit    0
 * xxxx  xxxS
 *          |
 *          +- Controller    shift    register   strobe
 * Strobe , 寄存器写时的回调函数
 * While S(strobe) is high,
 * the shift registers in the controllers are continuously reloaded from the button states,
 *
 *
 * @brief
 *
 * @param b
 */
void Controller::Write(Byte b)
{

    // Input ($4016 write) bit 0 是 S(strobe)位
    m_strobe = (b & 1);
    if (!m_strobe)
    {
        m_button_states = 0;
        int shift = 0;
        for (int btn = A; btn < TotalButtons; btn++)
        {
            // 读按键状态
            m_button_states |= (sf::Keyboard::isKeyPressed(m_key_bindings[static_cast<Buttion>(btn)]) << shift);
            ++shift;
        }
    }
}

/**
 * While S (strobe) is high,
 * the shift registers in the controllers are continuously reloaded from the button states,
 * and reading $4016/$4017 will keep returning the current state of the first button (A).
 * Once S goes low, this reloading will stop. Hence a 1/0 write sequence is required to get the button states,
 * after which the buttons can be read back one at a time.
 *
 * 当 Strobe 位为高时，控制器中的移位寄存器会根据按钮状态不断重新加载
 * 读寄存器时的回调函数
 * 当 Strobe 为 1 时，控制器将会一直返回 A 按键的状态
 * 当 Strobe 为 0 时, 控制器将依次返回各个button的状态
 */
Byte Controller::Read()
{
    Byte ret;
    if (m_strobe)
    {
        ret = sf::Keyboard::isKeyPressed(m_key_bindings[A]);
    }
    else
    {
        ret = (m_button_states & 1);
        m_button_states >>= 1;
    }
    return ret | 0x40;
}

// 建立PC键盘 与红白机模拟器各个Button的映射
void Controller::SetKeyBindings(const std::vector<sf::Keyboard::Key> &keys)
{
    m_key_bindings = keys;
}
