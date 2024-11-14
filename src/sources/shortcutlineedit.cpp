#include "shortcutlineedit.h"
#include <QFocusEvent>
#include <QDebug>
#include <qapplication.h>

#ifdef Q_OS_WIN
HHOOK ShortcutLineEdit::hook = nullptr;
#endif
ShortcutLineEdit* ShortcutLineEdit::focusedInstance = nullptr;

ShortcutLineEdit::ShortcutLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    initUI();
}

ShortcutLineEdit::ShortcutLineEdit(const QKeySequence &keySequence, QWidget *parent)
    : QLineEdit(parent), m_keySequence(keySequence)
{
    initUI();
}

ShortcutLineEdit::~ShortcutLineEdit()
{
#ifdef Q_OS_WIN
    // Cleanup the hook when no more instances exist
    if (hook && !focusedInstance) {
        UnhookWindowsHookEx(hook);
        hook = nullptr;
    }
#endif
}

void ShortcutLineEdit::setKeySequence(const QKeySequence &keySequence)
{
    if (m_keySequence != keySequence) {
        m_keySequence = keySequence;
        updateText();
        emit sigKeySequenceChanged(m_keySequence);
        qDebug() << "[emit] sigKeySequenceChanged():" << m_keySequence;
    }
}

QKeySequence ShortcutLineEdit::keySequence() const
{
    return m_keySequence;
}

void ShortcutLineEdit::initUI()
{
    setAlignment(Qt::AlignCenter);  // Center-align text
    setReadOnly(true);              // Make the line edit read-only initially
    setPlaceholderText(tr("Press shortcut"));
#ifdef Q_OS_WIN
    if (!hook) {
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    }
#endif
}

void ShortcutLineEdit::updateText()
{
    if (m_keySequence.isEmpty()) {
        setText(QString());
    } else {
        setText(m_keySequence.toString(QKeySequence::NativeText));
    }
}

void ShortcutLineEdit::handlePrintScreen(bool ctrl, bool alt, bool shift, bool meta)
{
    // 获取当前的修饰键状态
    Qt::KeyboardModifiers modifiers = Qt::KeyboardModifier::NoModifier;
    if (ctrl) modifiers |= Qt::ControlModifier;
    if (alt) modifiers |= Qt::AltModifier;
    if (shift) modifiers |= Qt::ShiftModifier;
    if (meta) modifiers |= Qt::MetaModifier;

    QKeySequence keySequence(modifiers | Qt::Key_Print);  // 构建 QKeySequence，包含修饰键和 Qt::Key_Print
    setKeySequence(keySequence);                          // 设置 QKeySequence 到控件
    emit sigKeySequenceChanged(keySequence);                 // 发射带修饰键的信号
}

void ShortcutLineEdit::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "ShortcutLineEdit keyPressEvent:" << event->text() << event->key() << event->modifiers();
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        setKeySequence(QKeySequence()); // Clear key sequence
        setText("");
    } else if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control || event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta) {
        return; // Do not update key sequence for modifier keys only
    } else {
        QKeySequence newSequence(event->key() | event->modifiers());
        setKeySequence(newSequence);
    }

    // Prevent the default behavior of QLineEdit
    event->ignore();
}

void ShortcutLineEdit::keyReleaseEvent(QKeyEvent *event)
{
    event->ignore(); // Ignore to prevent default behavior
}

void ShortcutLineEdit::focusInEvent(QFocusEvent *event)
{
    focusedInstance = this; // Set the static pointer to this instance
    QLineEdit::focusInEvent(event);
}

void ShortcutLineEdit::focusOutEvent(QFocusEvent *event)
{
    if (focusedInstance == this) {
        focusedInstance = nullptr; // Clear the static pointer if this instance loses focus
    }
    QLineEdit::focusOutEvent(event);
}

#ifdef Q_OS_WIN
LRESULT CALLBACK ShortcutLineEdit::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;

        bool ctrlPressed = GetKeyState(VK_CONTROL) & 0x8000;
        bool altPressed = GetKeyState(VK_MENU) & 0x8000;
        bool shiftPressed = GetKeyState(VK_SHIFT) & 0x8000;
        bool winPressed = (GetKeyState(VK_LWIN) & 0x8000) || (GetKeyState(VK_RWIN) & 0x8000);

        if (wParam == WM_KEYDOWN && pKeyboard->vkCode == VK_SNAPSHOT) {
            if (focusedInstance) {
                focusedInstance->handlePrintScreen(ctrlPressed, altPressed, shiftPressed, winPressed);
            }
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}
#endif
