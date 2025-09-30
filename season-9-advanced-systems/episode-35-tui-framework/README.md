# Episode 35: "TUI Framework" 💻
## Season 9: Advanced Systems | Episode 35/42

> *"Build beautiful interfaces. In the terminal."*

---

## 📋 Briefing

Создать TUI (Text User Interface) framework с ncurses.

**Задачи:**
1. ncurses basics
2. Widget system (buttons, inputs, lists)
3. Event handling
4. Layout engine

---

## 📚 Теория

### ncurses Basics

```c
#include <ncurses.h>

void init_tui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void draw_window(const char *title, int x, int y, int w, int h) {
    WINDOW *win = newwin(h, w, y, x);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " %s ", title);
    wrefresh(win);
}
```

---

## 🛠 Практика

**Задачи:**
1. Widget library
2. Dashboard layout
3. Real-time updates
4. Mouse support

---

**Next:** [Episode 36: Military Systems & Radar →](../episode-36-ballistics/)
